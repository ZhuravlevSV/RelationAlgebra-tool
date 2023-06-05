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
    regex import_regex("\\s*PRINT\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, import_regex) && match.size() == 2){
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
    cout << "RA SELECTION" << endl;

    return true;
}

// ==========================================

cmd_ra_natural_join_c::cmd_ra_natural_join_c(){}

cmd_ra_natural_join_c::cmd_ra_natural_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_natural_join_c::run(tables_c& tables){
    cout << "RA NATURAL JOIN" << endl;

    return true;
}

// ==========================================

cmd_ra_join_c::cmd_ra_join_c(){}

cmd_ra_join_c::cmd_ra_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_join_c::run(tables_c& tables){
    cout << "RA JOIN" << endl;

    return true;
}

// ==========================================

cmd_ra_multitask_c::cmd_ra_multitask_c(){}

cmd_ra_multitask_c::cmd_ra_multitask_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_multitask_c::run(tables_c& tables){
    cout << "RA MULTITASK" << endl;

    return true;
}

// ======================================================================
// SQL

cmd_sql_print_c::cmd_sql_print_c(){}

cmd_sql_print_c::cmd_sql_print_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_print_c::run(tables_c& tables){
    cout << "SQL PRINT" << endl;

    return true;
}

// ==========================================

cmd_sql_rename_c::cmd_sql_rename_c(){}

cmd_sql_rename_c::cmd_sql_rename_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_rename_c::run(tables_c& tables){
    cout << "SQL RENAME" << endl;

    return true;
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