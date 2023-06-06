#include "cmd.h"

cmd_c::cmd_c(){}

cmd_c::cmd_c(const string& new_cmd_text)
    : cmd_text(new_cmd_text) {}

// ======================================================================
// Relation Algebra

cmd_ra_import_c::cmd_ra_import_c(){}

cmd_ra_import_c::cmd_ra_import_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_import_c::run(tables_c& tables){
    regex import_regex("\\s*IMPORT\\s*(\\w+)\\s*\"(.+)\"\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, import_regex) && match.size() == 3){
        string table_dst_name = match[1];
        string table_dst_file_address = match[2];

        if(!tables.add_table_from_workspace(table_dst_name, table_dst_file_address)){
            cout << WARNING_TEXT << ": " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " can't be added" << endl;
            return false;
        }

        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "IMPORT" << RESET_FONT << " aborted" << endl;
    return false;
}

// ==========================================

cmd_ra_export_c::cmd_ra_export_c(){}

cmd_ra_export_c::cmd_ra_export_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_export_c::run(tables_c& tables) {
    regex export_regex("\\s*EXPORT\\s*(\\w+)\\s*\"(\\w+)\\.(\\w+)\"\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if (regex_search(cmd_text, match, export_regex) && match.size() == 4) {
        string table_src_name = match[1];
        string file_dst_name = match[2];
        string file_dst_format = match[3];

        // find table
        shared_ptr<table_c> table_src = tables.get_table(table_src_name);
        if (table_src == nullptr) {
            cout << WARNING_TEXT << ": requested " << UNDERLINE_FONT << table_src_name << RESET_FONT << " table doesn't exist" << endl;
            return false;
        }

        // check correct format
        unique_ptr<file_export_c> file_exporter = file_factory::create_file_export(file_dst_format, file_dst_name, table_src);
        if (file_exporter == nullptr) {
            cout << WARNING_TEXT << ": unknown file format" << endl;
            return false;
        }

        file_exporter->export_file();
        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "EXPORT" << RESET_FONT << " aborted" << endl;
    return false;
}

// ==========================================

cmd_ra_help_c::cmd_ra_help_c(){}

cmd_ra_help_c::cmd_ra_help_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_help_c::run(tables_c& tables){
    regex help_regex("\\s*HELP\\s*");
    match_results<string::const_iterator> match;

    if(regex_search(cmd_text, match, help_regex) && match.size() == 1){
        cout << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add a table write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "IMPORT" << RESET_FONT << BOLD_FONT << " table_name " << YELLOW_COLOR << "\"" << RESET_FONT << BOLD_FONT << "file_name.csv" << YELLOW_COLOR << "\""<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To export a table in CSV or XML format write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "EXPORT" << RESET_FONT << BOLD_FONT << " table_name " << YELLOW_COLOR << "\"" << RESET_FONT << BOLD_FONT << "file_name.csv/.xml" << YELLOW_COLOR << "\""<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To print out the table, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "PRINT" << RESET_FONT << BOLD_FONT << " table_name"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To rename the name of a table attribute, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "RENAME" << RESET_FONT << BOLD_FONT << " table_name old_attribute " << YELLOW_COLOR << "AS" << RESET_FONT << BOLD_FONT << " new_attribute"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add new table by projection with certain attributes from other table, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "PROJECTION" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "OF" << RESET_FONT << BOLD_FONT << " source_table_name " << YELLOW_COLOR << "[" << RESET_FONT << BOLD_FONT << "att1, att2, ... , attN" << YELLOW_COLOR << "]"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add new table by selection with certain values of attributes from other table, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "SELECT" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "OF" << RESET_FONT << BOLD_FONT << " source_table_name " << YELLOW_COLOR << "(" << RESET_FONT << BOLD_FONT << "att =/</> value" << YELLOW_COLOR << ")"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add new table by NATURAL JOIN of two other tables, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "NATURAL JOIN" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "FROM" << RESET_FONT << BOLD_FONT << " source_table_name_1, source_table_name_2"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add new table by JOIN of two other tables via some attribute, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "JOIN" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "FROM" << RESET_FONT << BOLD_FONT << " source_table_name_1, source_table_name_2 " << YELLOW_COLOR << "ON" << RESET_FONT << BOLD_FONT << " table_1_att " << YELLOW_COLOR << "=" << RESET_FONT << BOLD_FONT << " table_2_att"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add a table by running a multi-query, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "MULTITASK" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "FROM" << RESET_FONT << BOLD_FONT << " task" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Also commands for multitask" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Cartesian multiplication" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> table_name_1 " << YELLOW_COLOR << "CROSS" << RESET_FONT << BOLD_FONT << " table_name_2" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Intersection" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> table_name_1 " << YELLOW_COLOR << "INTERSECT" << RESET_FONT << BOLD_FONT << " table_name_2" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Exception" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> table_name_1 " << YELLOW_COLOR << "EXCEPT" << RESET_FONT << BOLD_FONT << " table_name_2" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Union" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> table_name_1 " << YELLOW_COLOR << "UNION" << RESET_FONT << BOLD_FONT << " table_name_2" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To convert a query to SQL format, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "TO SQL" << RESET_FONT << BOLD_FONT << " request" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To quit from application" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "CTRL" << RESET_FONT << BOLD_FONT << " + " << YELLOW_COLOR << "D" << RESET_FONT << endl;
        cout << endl;
        return true;
    }

    return false;
}

// ==========================================

cmd_ra_print_c::cmd_ra_print_c(){}

cmd_ra_print_c::cmd_ra_print_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_print_c::run(tables_c& tables){
    regex print_regex("\\s*PRINT\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, print_regex) && match.size() == 2){
        string table_name = match[1];

        if(cmd_text.substr(match[0].length()).find_first_not_of(" ") == string::npos){  

            // try to print
            if(!tables.print_table(table_name)){
                cout << WARNING_TEXT << ": table " << UNDERLINE_FONT << table_name << RESET_FONT << " doesn't exist" << endl;
                return false;
            }

            return true;
        }
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "PRINT" << RESET_FONT << " aborted" << endl;
    return false;
}

// ==========================================

cmd_ra_rename_c::cmd_ra_rename_c(){}

cmd_ra_rename_c::cmd_ra_rename_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_rename_c::run(tables_c& tables){
    regex rename_regex("\\s*RENAME\\s*(\\w+)\\s*(\\w+)\\s*AS\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, rename_regex) && match.size() == 4){
        string table_name = match[1];
        string table_attribute_old = match[2];
        string table_attribute_new = match[3];

        // try to find table
        shared_ptr<table_c> table = tables.get_table(table_name);
        if(table == nullptr){
            cout << WARNING_TEXT << ": table " << UNDERLINE_FONT << table_name << RESET_FONT << " doesn't exist" << endl;
            return false;
        }

        // change attribute name
        vector<header_cell_c> header = table->get_header();
        for(size_t index = 0; index < header.size(); index++){
            if(header.at(index).name == table_attribute_old){
                header.at(index).name = table_attribute_new;
                break;
            }
            if(index == header.size() - 1){
                cout << WARNING_TEXT << ": " << UNDERLINE_FONT << table_name << RESET_FONT << " table hasn't " << UNDERLINE_FONT << table_attribute_old << RESET_FONT << " attribute" << endl;
                return false;
            }
        }

        table->set_header(header);
        cout << SUCCESS_TEXT << ": renamed " << UNDERLINE_FONT << table_attribute_old << RESET_FONT << " as " << UNDERLINE_FONT << table_attribute_new << RESET_FONT << " in " << UNDERLINE_FONT << table_name << RESET_FONT << " table" << endl;
        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "RENAME" << RESET_FONT << " aborted" << endl;
    return false;
}

// ==========================================

cmd_ra_projection_c::cmd_ra_projection_c(){}

cmd_ra_projection_c::cmd_ra_projection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_projection_c::run(tables_c& tables){
    regex projection_regex("\\s*PROJECTION\\s*(\\w+)\\s*OF\\s*(\\w+)\\s*\\[([^\\]]+)\\]\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, projection_regex) && match.size() == 4){
        string table_dst_name = match[1];
        string table_src_name = match[2];
        string attributes = match[3];

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

        // find order what we need to create
        vector<header_cell_c> header_src = table_src->get_header();
        vector<size_t> indices_columns;

        istringstream iss_attributes(attributes);
        string attribute;

        while(getline(iss_attributes, attribute, ',')){
            bool match_found = false;
            for(size_t index = 0; index < header_src.size(); index++){
                if(attribute == header_src.at(index).name){
                    // requested attribute got same name as fonunded column

                    // if it isn't in indices_column already add it
                    if(find(indices_columns.begin(), indices_columns.end(), index) != indices_columns.end()){
                        // it is already in indices_column
                        continue;
                    }else {
                        // it isn't in indices_column
                        indices_columns.push_back(index);
                        match_found = true;
                        break;
                    }
                }
            }

            // check dublicate requested attribute
            if(!match_found){
                cout << WARNING_TEXT << ": unknown attribute" << endl;
                return false;
            }
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

        // create new table header
        vector<header_cell_c> header_dst;
        for(auto index : indices_columns)
            header_dst.emplace_back(header_src.at(index).name, header_src.at(index).type);
        
        // create new table body
        string ifs_line;
        while(getline(ifs, ifs_line)){
            // read words
            vector<string> words;
            stringstream ss_ifs(ifs_line);
            string word;
            while(getline(ss_ifs, word, ','))
                words.push_back(word);
            
            // fill output file with new data
            vector<size_t>::iterator it_indices = indices_columns.begin();
            while(it_indices != indices_columns.end()){
                ofs << words.at((*it_indices));
                it_indices++;
                if(it_indices != indices_columns.end())
                    ofs << ",";
            }
            ofs << endl;
            words.clear();
        }
        ifs.close();
        ofs.close();

        // add new table to tables
        if(!tables.add_table(table_dst_name, header_dst)){
            cout << WARNING_TEXT << ": can't add " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " to tables" << endl;
            return false;
        }

        cout << SUCCESS_TEXT << ": table " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " was projected from " << UNDERLINE_FONT << table_src_name << RESET_FONT << " table!" << endl;
        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "PROJECTION" << RESET_FONT << " aborted" << endl;
    return false;
}

// ==========================================

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

// ==========================================

cmd_ra_natural_join_c::cmd_ra_natural_join_c(){}

cmd_ra_natural_join_c::cmd_ra_natural_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_natural_join_c::run(tables_c& tables){
    regex natural_join_regex("\\s*NATURAL\\s*JOIN\\s*(\\w+)\\s*FROM\\s*(\\w+)\\s*,\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, natural_join_regex) && match.size() == 4){
        string table_dst_name = match[1];
        string table_src_1_name = match[2];
        string table_src_2_name = match[3];

        // try to find tables
        shared_ptr<table_c> table_src_1 = tables.get_table(table_src_1_name);
        shared_ptr<table_c> table_src_2 = tables.get_table(table_src_2_name);
        shared_ptr<table_c> table_dst = tables.get_table(table_dst_name);
        if(table_src_1 == nullptr){
            cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_src_1_name << RESET_FONT << " doesn't exist" << endl;
            return false;
        }
        if(table_src_2 == nullptr){
            cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_src_2_name << RESET_FONT << " doesn't exist" << endl;
            return false;
        }
        if(table_dst != nullptr){
            cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " already exist" << endl;
            return false;
        }

        // find columns indexes to compare elements
        vector<size_t> column_indices_src_1;
        vector<size_t> column_indices_src_2;
        vector<header_cell_c> header_src_1 = table_src_1->get_header();
        vector<header_cell_c> header_src_2 = table_src_2->get_header();
        for(size_t header_cell_index_src_1 = 0; header_cell_index_src_1 < header_src_1.size(); header_cell_index_src_1++){
            for(size_t header_cell_index_src_2 = 0; header_cell_index_src_2 < header_src_2.size(); header_cell_index_src_2++){
                if(header_src_1.at(header_cell_index_src_1).name == header_src_2.at(header_cell_index_src_2).name &&
                   find(column_indices_src_1.begin(), column_indices_src_1.end(), header_cell_index_src_1) == column_indices_src_1.end() &&
                   find(column_indices_src_2.begin(), column_indices_src_2.end(), header_cell_index_src_2) == column_indices_src_2.end()){
                    if(header_src_1.at(header_cell_index_src_1).type != header_src_2.at(header_cell_index_src_2).type){
                        cout << WARNING_TEXT << ": in requested tables " << UNDERLINE_FONT << table_src_1_name << ", " << table_src_2_name << RESET_FONT << " same named columns got different format" << endl;
                        return false;
                    }
                    column_indices_src_1.push_back(header_cell_index_src_1);
                    column_indices_src_2.push_back(header_cell_index_src_2);
                    break;
                }
            }
        }
        if(column_indices_src_1.size() == 0){
            cout << WARNING_TEXT << ": in requested tables" << UNDERLINE_FONT << table_src_1_name << ", " << table_src_2_name << RESET_FONT << " no any same named column" << endl;
            return false;
        }

        // open files
        ifstream ifs_1(BUFFER_FOLDER + table_src_1_name);
        if(!ifs_1.is_open()){
            cout << ERROR_TEXT << ": can't open input file" << endl;
            return false;
        }
        ifstream ifs_2(BUFFER_FOLDER + table_src_2_name);
        if(!ifs_2.is_open()){
            ifs_2.close();
            cout << ERROR_TEXT << ": can't open input file" << endl;
            return false;
        }
        ofstream ofs(BUFFER_FOLDER + table_dst_name);
        if(!ofs.is_open()){
            ifs_1.close();
            ifs_2.close();
            cout << ERROR_TEXT << ": can't open output file" << endl;
            return false;
        }

        // create new table header
        vector<header_cell_c> header_dst;
        for(auto index : column_indices_src_1){
            header_dst.emplace_back(header_src_1.at(index).name, header_src_1.at(index).type);
        }
        for(size_t header_cell_index_1 = 0; header_cell_index_1 < header_src_1.size(); header_cell_index_1++){
            if(find(column_indices_src_1.begin(), column_indices_src_1.end(), header_cell_index_1) != column_indices_src_1.end()) continue;
            header_dst.emplace_back(header_src_1.at(header_cell_index_1).name, header_src_1.at(header_cell_index_1).type);
        }
        for(size_t header_cell_index_2 = 0; header_cell_index_2 < header_src_2.size(); header_cell_index_2++){
            if(find(column_indices_src_2.begin(), column_indices_src_2.end(), header_cell_index_2) != column_indices_src_2.end()) continue;
            header_dst.emplace_back(header_src_2.at(header_cell_index_2).name, header_src_2.at(header_cell_index_2).type);
        }

        // create new table body
        string ifs_1_line;
        while(getline(ifs_1, ifs_1_line)){
            // read line
            vector<string> words_ifs_1_line;
            stringstream ss_1(ifs_1_line);
            string word;
            while(getline(ss_1, word, ',')) words_ifs_1_line.push_back(word);

            // read lines from other table
            ifs_2.clear();
            ifs_2.seekg(0);
            string ifs_2_line;
            while(getline(ifs_2, ifs_2_line)){
                vector<string> words_ifs_2_line;
                stringstream ss_2(ifs_2_line);
                while(getline(ss_2, word, ',')) words_ifs_2_line.push_back(word);

                /* OLD NATURAL JOIN COMPARE
                // compare words what we need to compare
                bool success_compare = true;
                for(size_t index = 0; index < column_indices_src_1.size() && success_compare; index++)
                    if(words_ifs_1_line.at(column_indices_src_1.at(index)) != words_ifs_2_line.at(column_indices_src_2.at(index))) success_compare = false;
                if(success_compare == false){words_ifs_2_line.clear(); continue;}
                */

                // compare worde what we need to compare
                bool success_compare = true;
                for(size_t index = 0; index < column_indices_src_1.size() && success_compare; index++){
                    if(header_src_1.at(column_indices_src_1.at(index)).type == header_cell_c::type_e::FLOAT &&
                       header_src_2.at(column_indices_src_2.at(index)).type == header_cell_c::type_e::FLOAT){
                        // compare like float
                        if(stof(words_ifs_1_line.at(column_indices_src_1.at(index))) != stof(words_ifs_2_line.at(column_indices_src_2.at(index)))) success_compare = false;
                    }else {
                        // compare like string, undefined
                        if(words_ifs_1_line.at(column_indices_src_1.at(index)) != words_ifs_2_line.at(column_indices_src_2.at(index))) success_compare = false;
                    }
                }
                if(success_compare == false){words_ifs_2_line.clear(); continue;}

                // add compared lines to body table
                // add key elements from first and second tables
                for(size_t index = 0; index < column_indices_src_1.size(); index++){
                    ofs << words_ifs_1_line.at(column_indices_src_1.at(index));
                    if(index != column_indices_src_1.size() - 1)
                        ofs << ",";
                }
                // add other elements from first table
                for(size_t index = 0; index < words_ifs_1_line.size(); index++){
                    if(find(column_indices_src_1.begin(), column_indices_src_1.end(), index) != column_indices_src_1.end()) continue;
                    ofs << ",";
                    ofs << words_ifs_1_line.at(index);
                }
                // add other elements from second table
                for(size_t index = 0; index < words_ifs_2_line.size(); index++){
                    if(find(column_indices_src_2.begin(), column_indices_src_2.end(), index) != column_indices_src_2.end()) continue;
                    ofs << ",";
                    ofs << words_ifs_2_line.at(index);
                }
                ofs << endl;
            }
        }
        ifs_1.close();
        ifs_2.close();
        ofs.close();

        // add new table to tables
        if(!tables.add_table(table_dst_name, header_dst)){
            cout << WARNING_TEXT << ": can't add " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " to tables" << endl;
            return false;
        }

        cout << SUCCESS_TEXT << ": table " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " has been created by NATURAL JOIN by " << UNDERLINE_FONT << table_src_1_name << ", " << table_src_2_name << RESET_FONT << " tables!" << endl;
        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "NATURAL JOIN" << RESET_FONT << " aborted" << endl;
    return false;
}

// ==========================================

cmd_ra_join_c::cmd_ra_join_c(){}

cmd_ra_join_c::cmd_ra_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_join_c::run(tables_c& tables){
    regex join_regex("\\s*JOIN\\s*(\\w+)\\s*FROM\\s*(\\w+)\\s*,\\s*(\\w+)\\s*ON\\s*(\\w+)\\s*=\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, join_regex) && match.size() == 6){
        string table_dst_name = match[1];
        string table_src_1_name = match[2];
        string table_src_2_name = match[3];
        string table_src_1_att = match[4];
        string table_src_2_att = match[5];

        // try to find tables
        shared_ptr<table_c> table_src_1 = tables.get_table(table_src_1_name);
        shared_ptr<table_c> table_src_2 = tables.get_table(table_src_2_name);
        shared_ptr<table_c> table_dst = tables.get_table(table_dst_name);
        if(table_src_1 == nullptr){
            cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_src_1_name << RESET_FONT << " doesn't exist" << endl;
            return false;
        }
        if(table_src_2 == nullptr){
            cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_src_2_name << RESET_FONT << " doesn't exist" << endl;
            return false;
        }
        if(table_dst != nullptr){
            cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " already exist" << endl;
            return false;
        }

        // find column requested indices
        size_t column_index_src_1 = 0;
        size_t column_index_src_2 = 0;
        vector<header_cell_c> header_src_1 = table_src_1->get_header();
        vector<header_cell_c> header_src_2 = table_src_2->get_header();
        bool found_attribute = false;
        for(size_t index = 0; index < header_src_1.size(); index++){
            if(header_src_1.at(index).name == table_src_1_att){column_index_src_1 = index; found_attribute = true; break;}
        }
        if(found_attribute == false){
            cout << WARNING_TEXT << ": requested attribute " << UNDERLINE_FONT << table_src_1_att << RESET_FONT << " for table " << UNDERLINE_FONT << table_src_1_name << RESET_FONT << " doesn't exisits" << endl;
            return false;
        }
        found_attribute = false;
        for(size_t index = 0; index < header_src_2.size(); index++){
            if(header_src_2.at(index).name == table_src_2_att){column_index_src_2 = index; found_attribute = true; break;}
        }
        if(found_attribute == false){
            cout << WARNING_TEXT << ": requested attribute " << UNDERLINE_FONT << table_src_2_att << RESET_FONT << " for table " << UNDERLINE_FONT << table_src_2_name << RESET_FONT << " doesn't exisits" << endl;
            return false;
        }
        if(header_src_1.at(column_index_src_1).type != header_src_2.at(column_index_src_2).type){
            cout << WARNING_TEXT << ": requested attributes " << UNDERLINE_FONT << table_src_1_att << ", " << table_src_2_att << RESET_FONT << " are in different formats" << endl;
            return false;
        }

        // open files
        ifstream ifs_1(BUFFER_FOLDER + table_src_1_name);
        if(!ifs_1.is_open()){
            cout << ERROR_TEXT << ": can't open input file" << endl;
            return false;
        }
        ifstream ifs_2(BUFFER_FOLDER + table_src_2_name);
        if(!ifs_2.is_open()){
            ifs_2.close();
            cout << ERROR_TEXT << ": can't open input file" << endl;
            return false;
        }
        ofstream ofs(BUFFER_FOLDER + table_dst_name);
        if(!ofs.is_open()){
            ifs_1.close();
            ifs_2.close();
            cout << ERROR_TEXT << ": can't open output file" << endl;
            return false;
        }

        // create new table header
        vector<header_cell_c> header_dst;
        for(auto cell : header_src_1) header_dst.emplace_back(cell.name, cell.type);
        for(size_t index = 0; index < header_src_2.size(); index++){
            if(index == column_index_src_2){
                header_dst.emplace_back(header_src_2.at(index).name, header_src_2.at(index).type);
            }else {
                header_dst.emplace_back(header_src_2.at(index).name, header_src_2.at(index).type);
            }
        }

        // create new table body
        string ifs_1_line;
        while(getline(ifs_1, ifs_1_line)){
            // read line
            vector<string> words_ifs_1_line;
            stringstream ss_1(ifs_1_line);
            string word;
            while(getline(ss_1, word, ',')) words_ifs_1_line.push_back(word);

            // read lines from other table
            ifs_2.clear();
            ifs_2.seekg(0);
            string ifs_2_line;
            while(getline(ifs_2, ifs_2_line)){
                vector<string> words_ifs_2_line;
                stringstream ss_2(ifs_2_line);
                while(getline(ss_2, word, ',')) words_ifs_2_line.push_back(word);

                // compare word what we need to compare
                bool success_compare = true;
                if(header_src_1.at(column_index_src_1).type == header_cell_c::type_e::FLOAT && header_src_2.at(column_index_src_2).type == header_cell_c::type_e::FLOAT){
                    // compare like float
                    if(stof(words_ifs_1_line.at(column_index_src_1)) != stof(words_ifs_2_line.at(column_index_src_2))) success_compare = false;
                }else {
                    // compare like string, undefined
                    if(words_ifs_1_line.at(column_index_src_1) != words_ifs_2_line.at(column_index_src_2)) success_compare = false;
                }
                if(success_compare == false){words_ifs_2_line.clear(); continue;}

                // add compared lines body table
                for(size_t index = 0; index < words_ifs_1_line.size(); index++){
                    ofs << words_ifs_1_line.at(index);
                    if(index != words_ifs_1_line.size() - 1)
                        ofs << ",";
                }
                for(size_t index = 0; index < words_ifs_2_line.size(); index++){
                    ofs << ",";
                    ofs << words_ifs_2_line.at(index);
                }
                ofs << endl;
            }
        }
        ifs_1.close();
        ifs_2.close();
        ofs.close();

        // add new table to tables
        if(!tables.add_table(table_dst_name, header_dst)){
            cout << WARNING_TEXT << ": can't add " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " to tables" << endl;
            return false;
        }

        cout << SUCCESS_TEXT << ": table " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " has been created by JOIN by " << UNDERLINE_FONT << table_src_1_name << ", " << table_src_2_name << RESET_FONT << " tables!" << endl;
        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "JOIN" << RESET_FONT << " aborted" << endl;
    return false;
}

// ==========================================

cmd_ra_multitask_c::cmd_ra_multitask_c(){}

cmd_ra_multitask_c::cmd_ra_multitask_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_multitask_c::run(tables_c& tables){
    regex multitask_regex("\\s*MULTITASK\\s*(\\w+)\\s*FROM\\s*(.+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, multitask_regex) && match.size() == 3){
        string table_dst_name = match[1];
        string request = match[2];

        for(size_t index = 0; index < request.length(); index++){
            if(request[index] != ' ' && request[index] != '\t' && request[index] != '\n' && request[index] != '\r') break;
            if(index == request.length() - 1){
                cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "MULTITASK" << RESET_FONT << " aborted" << endl;
                return false;
            }
        }
        
        // try to find tables
        shared_ptr<table_c> table_dst = tables.get_table(table_dst_name);
        if(table_dst != nullptr){
            cout << WARNING_TEXT << ": this " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " already exist" << endl;
            return false;
        }

        // try to solve multitask
        multitask_ra_c multitask;
        if(!multitask.solve(table_dst_name, request, tables)){
            cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "MULTITASK" << RESET_FONT << " aborted" << endl;
            return false;
        }

        cout << SUCCESS_TEXT << ": table " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " has been created by multitask" << endl;
        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "MULTITASK" << RESET_FONT << " aborted" << endl;
    return false;
}

// ======================================================================
// SQL

cmd_sql_print_c::cmd_sql_print_c(){}

cmd_sql_print_c::cmd_sql_print_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_print_c::run(tables_c& tables){
    regex print_regex("\\s*PRINT\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, print_regex) && match.size() == 2){
        string table_name = match[1];

        // check if in request are 2 tables
        if(cmd_text.substr(match[0].length()).find_first_not_of(" ") == string::npos){        
            // print SQL
            cout << endl << "SELECT * FROM " << table_name << ";" << endl << endl;
            return true;
        }
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SQL PRINT" << RESET_FONT << " aborted" << endl;
    return false;
}

// ==========================================

cmd_sql_rename_c::cmd_sql_rename_c(){}

cmd_sql_rename_c::cmd_sql_rename_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_rename_c::run(tables_c& tables){
    regex rename_regex("\\s*RENAME\\s*(\\w+)\\s*(\\w+)\\s*AS\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, rename_regex) && match.size() == 4){
        string table_name = match[1];
        string old_att = match[2];
        string new_att = match[3];

        if(cmd_text.substr(match[0].length()).find_first_not_of(" ") == string::npos){
            // print SQL
            cout << endl << "ALTER TABLE " << table_name << " RENAME COLUMN " << old_att << " TO " << new_att << ";" << endl << endl;
            return true;
        }
    }
    return false;
}

// ==========================================

cmd_sql_projection_c::cmd_sql_projection_c(){}

cmd_sql_projection_c::cmd_sql_projection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_projection_c::run(tables_c& tables){
    cout << "SQL PROJECTION" << endl;

    return true;
}

// ==========================================

cmd_sql_selection_c::cmd_sql_selection_c(){}

cmd_sql_selection_c::cmd_sql_selection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_selection_c::run(tables_c& tables){
    cout << "SQL SELECT" << endl;

    return true;
}

// ==========================================

cmd_sql_natural_join_c::cmd_sql_natural_join_c(){}

cmd_sql_natural_join_c::cmd_sql_natural_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_natural_join_c::run(tables_c& tables){
    cout << "SQL NATURAL JOIN" << endl;

    return true;
}

// ==========================================

cmd_sql_join_c::cmd_sql_join_c(){}

cmd_sql_join_c::cmd_sql_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_join_c::run(tables_c& tables){
    cout << "SQL JOIN" << endl;

    return true;
}

// ==========================================

cmd_sql_multitask_c::cmd_sql_multitask_c(){}

cmd_sql_multitask_c::cmd_sql_multitask_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_multitask_c::run(tables_c& tables){
    cout << "SQL MULTITASK" << endl;

    return true;
}

// ==========================================