#include "cmd_ra_projection.h"

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
