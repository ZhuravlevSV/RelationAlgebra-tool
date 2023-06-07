#include "cmd_ra_natural_join.h"

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
