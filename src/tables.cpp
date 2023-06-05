#include "tables.h"


header_cell_c::header_cell_c(const string& new_name, const type_e& new_type)
    : name(new_name), type(new_type) {}

// ==================================================

table_c::table_c() {}

table_c::table_c(const string& new_name, const string& file_address){
    // check csv file valid
    if(!is_valid_csv(file_address)){
        cout << ERROR_TEXT << ": the file doesn't exist or the file is not in CSV format" << endl;
        throw logic_error("the file is not in CSV format");
        return;
    }
    if(new_name == "CROSS" || new_name == "INTERSECT" || new_name == "EXCEPT" || new_name == "UNION" || new_name == "(" || new_name == ")" ||
       new_name == "IMPORT" || new_name == "EXPORT" || new_name == "HELP" || new_name == "PROJECTION" || new_name == "SELECT" || new_name == "MULTITASK" || 
       new_name == "NATURAL" || new_name == "JOIN"){
        cout << WARNING_TEXT << ": table name can't be used as a keyword " << UNDERLINE_FONT << new_name << RESET_FONT << endl;
        throw logic_error("used keyword as table name");
        return;
    }

    // open input file and create output file
    ifstream ifs(WORKSPACE_FOLDER + file_address);
    if(!ifs.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        throw logic_error("can't open input file");
        return;
    }
    ofstream ofs(BUFFER_FOLDER + new_name);
    if(!ofs.is_open()){
        ifs.close();
        cout << ERROR_TEXT << ": can't open output file" << endl;
        throw logic_error("can't open output file");
        return;
    }

    // start create new table
    name = new_name;

    // read header line
    string ifs_line;
    if(getline(ifs, ifs_line)){
        // clear the line of extra characters
        ifs_line.erase(remove_if(ifs_line.begin(), ifs_line.end(), [](char c) { return c == '\r' || c == '\n'; }), ifs_line.end());

        // fill header of current table
        stringstream ifs_ss(ifs_line);
        string header_cell_name;
        while(getline(ifs_ss, header_cell_name, ',')){
            // add header cell, format header cell default is NONE
            header.emplace_back(header_cell_name, header_cell_c::type_e::NONE);
        }
    }else {
        ifs.close();
        ofs.close();
        cout << ERROR_TEXT << ": empty input file" << endl;
        throw logic_error("empty input file");
        return;
    }

    // read body lines
    while(getline(ifs, ifs_line)){
        // clear the line of extra characters
        ifs_line.erase(remove_if(ifs_line.begin(), ifs_line.end(), [](char c) { return c == '\r' || c == '\n'; }), ifs_line.end());
        ofs << ifs_line << endl;
    }
    ofs.close();

    // update format in table header (by output file in .buffer)
    ifs.close();
    ifs.open(BUFFER_FOLDER + name);
    while(getline(ifs, ifs_line)){
        // clear the line of extra characters
        ifs_line.erase(remove_if(ifs_line.begin(), ifs_line.end(), [](char c) { return c == '\r' || c == '\n'; }), ifs_line.end());

        // define formats of current table
        stringstream ifs_ss(ifs_line);
        string body_cell_element;
        vector<header_cell_c>::iterator it_header = header.begin();
        while(getline(ifs_ss, body_cell_element, ',')){
            switch (it_header->type){
            case header_cell_c::type_e::UNDEFINED: it_header++; continue;
            case header_cell_c::type_e::NONE: it_header->type = (is_number(body_cell_element) ? header_cell_c::type_e::FLOAT : header_cell_c::type_e::STRING); it_header++; continue;
            case header_cell_c::type_e::STRING: it_header->type = (is_number(body_cell_element) ? header_cell_c::type_e::UNDEFINED : header_cell_c::type_e::STRING); it_header++; continue;
            case header_cell_c::type_e::FLOAT: it_header->type = (is_number(body_cell_element) ? header_cell_c::type_e::FLOAT : header_cell_c::type_e::UNDEFINED); it_header++; continue;
            default: break;
            }
        }
    }
    ifs.close();

    // new table created
    cout << SUCCESS_TEXT << ": table " << UNDERLINE_FONT << name << RESET_FONT << " is created!" << endl;
}

table_c::~table_c() {
    // delete file in .buffer if it exits
    string file_address = BUFFER_FOLDER + name;
    if(filesystem::exists(file_address)){
        filesystem::remove(file_address);
        cout << NOTE_TEXT << ": deleting " << UNDERLINE_FONT << file_address << RESET_FONT << " file" << endl;
    }
}

string table_c::get_name() const{
    return name;
}

void table_c::set_name(const string& new_name){
    name = new_name;
}

vector<header_cell_c> table_c::get_header() const{
    return header;
}

void table_c::set_header(const vector<header_cell_c>& new_header){
    header = new_header;
}

void table_c::print() const{
    // print table header
    cout << name << endl;
    cout << UNDERLINE_FONT;
    for(size_t index = 0; index < header.size(); index++){
        cout << header.at(index).name;
        if(index == header.size() - 1)
            cout << RESET_FONT << endl;
        else
            cout << ",";
    }

    // open input file
    ifstream ifs(BUFFER_FOLDER + name);
    if(!ifs.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return;
    }

    // print table body
    string ifs_line;
    while(getline(ifs, ifs_line)){
        cout << ifs_line << endl;
    }

    ifs.close();
}

// ==================================================

tables_c::tables_c() {}

bool tables_c::add_table_from_workspace(const string& new_name, const string& file_address){
    // try find same table name
    for(const auto& table : tables_map){
        if (table.first.size() != new_name.size())
            continue;
        for (std::size_t i = 0; i < table.first.size(); ++i) {
            if (std::tolower(table.first[i]) != std::tolower(new_name[i]))
                break;
            if(i == table.first.size() - 1){
                cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << new_name << RESET_FONT << " already exists" << endl;
                return false;
            }
        }
    }

    // add new table
    try{
        shared_ptr<table_c> new_table = make_shared<table_c>(new_name, file_address);
        tables_map.insert(make_pair(new_name, new_table));
    }catch(const logic_error& ex){
        return false;
    }

    return true;
}

bool tables_c::add_table(const string& table_name, const vector<header_cell_c>& table_header){
    // find same table
    if(get_table(table_name) != nullptr){
        cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_name << RESET_FONT << " table already exist" << endl;
        return false;
    }

    // create and add table to tables
    shared_ptr<table_c> new_table = make_shared<table_c>();
    new_table->set_name(table_name);
    new_table->set_header(table_header);
    tables_map.insert(make_pair(table_name, new_table));

    return true;
}

const shared_ptr<table_c> tables_c::get_table(const string& name){
    map<string, shared_ptr<table_c>>::const_iterator cit_table = tables_map.find(name);
    if(cit_table == tables_map.end())
        return nullptr;
    return cit_table->second;
}

bool tables_c::print_table(const string& name) const{
    map<string, shared_ptr<table_c>>::const_iterator cit_table = tables_map.find(name);
    if(cit_table == tables_map.end())
        return false;
    
    cit_table->second->print();
    return true;
}

// ==================================================

bool is_valid_csv(const string& file_address){
    // create ifs
    ifstream ifs(WORKSPACE_FOLDER + file_address);
    if(!ifs.is_open()) return false;

    // compare number of ',' in all rows
    size_t compare_count = 0;
    string ifs_line;

    while(getline(ifs, ifs_line)){
        size_t pos = 0, count = 0;
        // cycle while npos - not finded symbol ','
        while((pos = ifs_line.find(',', pos)) != string::npos){
            count++;
            pos++;
        }
        if(count <= 0){
            ifs.close();
            return false;
        }
        if(compare_count == 0){
            compare_count = count;
            continue;
        }
        if(compare_count != count){
            ifs.close();
            return false;
        }
    }
    if(compare_count == 0){
        ifs.close();
        return false;
    }

    // csv file is valid
    ifs.close();
    return true;
}

bool is_number(const string& text){
    try{
        stof(text);
        return true;
    }catch (const invalid_argument&){
        return false;
    }
}
