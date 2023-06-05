#include "file_export.h"

unique_ptr<file_export_c> file_factory::create_file_export(const string& file_format, const string& file_name, shared_ptr<table_c> table_src) {
    map<string, formats> text_formats = {{"csv", formats::CSV}, {"xml", formats::XML}, {"json", formats::JSON}};
    auto it_format = text_formats.find(file_format);

    switch (it_format->second) {
        case formats::CSV:
            return make_unique<file_CSV_c>(file_name, table_src);
        case formats::XML:
            return make_unique<file_XML_c>(file_name, table_src);
        case formats::JSON:
            return make_unique<file_JSON_c>(file_name, table_src);
        default:
            return nullptr;
    }
}

// ===================================================================

file_export_c::file_export_c(){}

file_export_c::file_export_c(const string& new_file_name, shared_ptr<table_c> new_table_src)
    : file_name(new_file_name), table_src(new_table_src) {}

// ===================================================================

file_CSV_c::file_CSV_c(){}

file_CSV_c::file_CSV_c(const string& new_file_name, shared_ptr<table_c> new_table_src)
    : file_export_c(new_file_name, new_table_src) {}

void file_CSV_c::export_file(){

    // rewrite if exist file
    if(filesystem::exists(SAVED_FILES_FOLDER + file_name + ".csv"))
        cout << NOTE_TEXT << ": " << UNDERLINE_FONT << SAVED_FILES_FOLDER + file_name + ".csv" << RESET_FONT << " already exist, rewrite..." << endl;

    // open files
    ifstream ifs(BUFFER_FOLDER + table_src->get_name());
    if(!ifs.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return;
    }
    ofstream ofs(SAVED_FILES_FOLDER + file_name + ".csv");
    if(!ofs.is_open()){
        ifs.close();
        cout << ERROR_TEXT << ": can't open output file" << endl;
        return;
    }

    // fill header in CSV file
    vector<header_cell_c> header_src = table_src->get_header();
    for(size_t index = 0; index < header_src.size(); index++){
        ofs << header_src.at(index).name;
        if(index == header_src.size() - 1)
            ofs << endl;
        else
            ofs << ",";
    }

    // fill body in CSV file
    string ifs_line;
    while(getline(ifs, ifs_line))
        ofs << ifs_line << endl;
    
    ifs.close();
    ofs.close();

    cout << SUCCESS_TEXT << ": table has been exported to " << UNDERLINE_FONT << file_name << RESET_FONT << endl;
}

// ===================================================================

file_XML_c::file_XML_c(){}

file_XML_c::file_XML_c(const string& new_file_name, shared_ptr<table_c> new_table_src)
    : file_export_c(new_file_name, new_table_src) {}

void file_XML_c::export_file(){
    // rewrite if exist file
    if(filesystem::exists(SAVED_FILES_FOLDER + file_name + ".xml"))
        cout << NOTE_TEXT << ": " << UNDERLINE_FONT << SAVED_FILES_FOLDER + file_name + ".xml" << RESET_FONT << " already exist, rewrite..." << endl;
    
    // open files
    ifstream ifs(BUFFER_FOLDER + table_src->get_name());
    if(!ifs.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return;
    }
    ofstream ofs(SAVED_FILES_FOLDER + file_name + ".xml");
    if(!ofs.is_open()){
        ifs.close();
        cout << ERROR_TEXT << ": can't open output file" << endl;
        return;
    }

    // fill XML file
    ofs << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    ofs << "<" << table_src->get_name() << ">" << endl;

    vector<header_cell_c> src_header = table_src->get_header();
    string ifs_line;
    while(getline(ifs, ifs_line)){
        ofs << "  <item>" << endl;
        vector<header_cell_c>::iterator it_src_header = src_header.begin();
        stringstream ss_line(ifs_line);
        string word;
        while(getline(ss_line, word, ',')){
            ofs << "    <" << (*it_src_header).name << ">" << word << "</" << (*it_src_header).name << ">" << endl;
            it_src_header++;
        }
        ofs << "  </item>" << endl;
    }
    ofs << "</" << table_src->get_name() << ">" << endl;


    ifs.close();
    ofs.close();

    cout << SUCCESS_TEXT << ": table has been exported to " << UNDERLINE_FONT << SAVED_FILES_FOLDER + file_name + ".xml" << RESET_FONT << endl;
}

// ===================================================================

file_JSON_c::file_JSON_c(){}

file_JSON_c::file_JSON_c(const string& new_file_name, shared_ptr<table_c> new_table_src)
    : file_export_c(new_file_name, new_table_src) {}

void file_JSON_c::export_file(){
    cout << "EXPORT JSON FILE" << endl;
}