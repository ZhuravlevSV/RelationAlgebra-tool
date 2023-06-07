#include "cmd_ra_selection.h"

cmd_ra_selection_c::cmd_ra_selection_c(){}

cmd_ra_selection_c::cmd_ra_selection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_selection_c::run(tables_c& tables){
    regex selection_regex("\\s*SELECT\\s*(\\w+)\\s*OF\\s*(\\w+)\\s*\\((\\w+)\\s*([=<>])\\s*([\\d.]+|[\"\\w]+)\\)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_match(cmd_text, match, selection_regex) && match.size() == 6){
        string table_dst_name = match[1];
        string table_src_name = match[2];
        string attribute = match[3];
        string operator_symbol = match[4];
        string value = match[5];

        // try to find tables
        shared_ptr<table_c> table_src = tables.get_table(table_src_name);
        shared_ptr<table_c> table_dst = tables.get_table(table_dst_name);
        if(table_src == nullptr){
            cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_src_name << RESET_FONT << " doesn't exist" << endl;
            return false;
        }
        if(table_dst != nullptr){
            cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " already exist" << endl;
            return false;
        }

        // create new table header and find attribute index to compare
        vector<header_cell_c> header_dst = table_src->get_header();
        int att_index = -1;
        header_cell_c::type_e att_type = header_cell_c::type_e::UNDEFINED;
        for(vector<header_cell_c>::iterator it_header = header_dst.begin(); it_header != header_dst.end(); it_header++){
            if(it_header->name == attribute){
                if(it_header->type == header_cell_c::type_e::UNDEFINED){
                    cout << WARNING_TEXT << ": this attribute is UNDEFINED" << endl;
                    return false;
                }
                att_index = distance(header_dst.begin(), it_header);
                att_type = it_header->type;
                break;
            }
        }
        if(att_index == -1){
            cout << WARNING_TEXT << ": this attribute doesn't exist" << endl;
            return false;
        }

        // open files
        ifstream ifs(BUFFER_FOLDER + table_src_name);
        if(!ifs.is_open()){
            cout << ERROR_TEXT << ": can't open input file" << endl;
            return false;
        }
        ofstream ofs(BUFFER_FOLDER + table_dst_name);
        if(!ofs.is_open()){
            ifs.close();
            cout << ERROR_TEXT << ": can't open output file" << endl;
            return false;
        }

        // create new table body
        string ifs_line;
        while(getline(ifs, ifs_line)){
            vector<string> words;
            stringstream ss_ifs(ifs_line);
            string word;
            while(getline(ss_ifs, word, ','))
                words.push_back(word);
            
            // fill output file with new data
            if(operator_symbol == "="){
                // is =
                if(att_type == header_cell_c::type_e::STRING){
                    // is string =
                    if(value == words.at(att_index)) ofs << ifs_line << endl;
                    else continue;
                }else {
                    // is float =
                    if(stof(value) == stof(words.at(att_index))) ofs << ifs_line << endl;
                    else continue;
                }
            }else if(operator_symbol == ">"){
                // is <
                if(att_type == header_cell_c::type_e::STRING){
                    // is string <
                    if(value < words.at(att_index)) ofs << ifs_line << endl;
                    else continue;
                }else {
                    // is float <
                    if(stof(value) < stof(words.at(att_index))) ofs << ifs_line << endl;
                    else continue;
                }
            }else {
                // is >
                if(att_type == header_cell_c::type_e::STRING){
                    // is string >
                    if(value > words.at(att_index)) ofs << ifs_line << endl;
                    else continue;
                }else {
                    // is float >
                    if(stod(value) > stof(words.at(att_index))) ofs << ifs_line << endl;
                    else continue;
                }
            }

            words.clear();
        }

        ifs.close();
        ofs.close();

        // add new table to tables
        if(!tables.add_table(table_dst_name, header_dst)){
            cout << WARNING_TEXT << ": can't add " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " to tables" << endl;
            return false;
        }

        cout << SUCCESS_TEXT << ": table " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " was selected from " << UNDERLINE_FONT << table_src_name << RESET_FONT << " table!" << endl;
        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SELECT" << RESET_FONT << " aborted" << endl;
    return false;
}
