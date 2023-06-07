#include "cmd_ra_join.h"

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
