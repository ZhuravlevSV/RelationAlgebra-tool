#include "multitask.h"

token_s::token_s(){}

token_s::token_s(const string& new_name, const type_e& new_type, const associative_e& new_associative, const size_t& new_priority)
    : name(new_name), type(new_type), associative(new_associative), priority(new_priority) {}

// =============================================================================

multitask_ra_c::multitask_ra_c(){
    id_tables_stack = 0;
}

shared_ptr<table_c> multitask_ra_c::create_tmp_table(const string& table_src_name, tables_c& tables){
    
    // create tmp table
    shared_ptr<table_c> tmp_table = make_shared<table_c>();

    // try to find source table
    shared_ptr<table_c> table_src = tables.get_table(table_src_name);
    if(table_src == nullptr){
        cout << WARNING_TEXT << ": requested table " << UNDERLINE_FONT << table_src_name << RESET_FONT << " doesn't exist" << endl;
        return nullptr;
    }

    // set name and header
    tmp_table->set_name("MULTITASK_" + to_string(id_tables_stack++));
    tmp_table->set_header(table_src->get_header());

    // open files
    ifstream ifs(BUFFER_FOLDER + table_src_name);
    if(!ifs.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return nullptr;
    }
    ofstream ofs(BUFFER_FOLDER + tmp_table->get_name());
    if(!ofs.is_open()){
        ifs.close();
        cout << ERROR_TEXT << ": can't open output file" << endl;
        return nullptr;
    }

    // fill ofs with ifs data
    string ifs_line;
    while(getline(ifs, ifs_line)){
        ofs << ifs_line << endl;
    }
    ifs.close();
    ofs.close();

    return tmp_table;
}

shared_ptr<table_c> multitask_ra_c::cross_tmp_tables(shared_ptr<table_c> left_table, shared_ptr<table_c> right_table){

    // create tmp table
    shared_ptr<table_c> tmp_table = make_shared<table_c>();

    // set name and header
    tmp_table->set_name("MULTITASK_" + to_string(id_tables_stack++));
    vector<header_cell_c> new_tmp_header;
    vector<header_cell_c> left_header = left_table->get_header();
    vector<header_cell_c> right_header = right_table->get_header();
    new_tmp_header.insert(new_tmp_header.end(), left_header.begin(), left_header.end());
    new_tmp_header.insert(new_tmp_header.end(), right_header.begin(), right_header.end());
    tmp_table->set_header(new_tmp_header);

    // open files
    ifstream ifs_left(BUFFER_FOLDER + left_table->get_name());
    if(!ifs_left.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return nullptr;
    }
    ifstream ifs_right(BUFFER_FOLDER + right_table->get_name());
    if(!ifs_right.is_open()){
        ifs_left.close();
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return nullptr;
    }
    ofstream ofs(BUFFER_FOLDER + tmp_table->get_name());
    if(!ofs.is_open()){
        ifs_left.close();
        ifs_right.close();
        cout << ERROR_TEXT << ": can't open output file" << endl;
        return nullptr;
    }

    // fill ofs with ifs_left CROSS ifs_right
    string ifs_left_line;
    while(getline(ifs_left, ifs_left_line)){
        ifs_right.clear();
        ifs_right.seekg(0);
        string ifs_right_line;
        while(getline(ifs_right, ifs_right_line)){
            ofs << ifs_left_line << "," << ifs_right_line << endl;
        }
    }
    ifs_left.close();
    ifs_right.close();
    ofs.close();

    return tmp_table;
}

shared_ptr<table_c> multitask_ra_c::intersect_tmp_tables(shared_ptr<table_c> left_table, shared_ptr<table_c> right_table){
    
    // find column indices to compare elements
    vector<header_cell_c> left_header = left_table->get_header();
    vector<header_cell_c> right_header = right_table->get_header();
    if(left_header.size() != right_header.size()){
        cout << WARNING_TEXT << ": INTERSECT operation cannot be performed because the number of columns in one table does not match the number of columns in another table" << endl;
        return nullptr;
    }
    vector<size_t> column_indices_src_left;
    vector<size_t> column_indices_src_right;
    for(size_t header_cell_index_src_left = 0; header_cell_index_src_left < left_header.size(); header_cell_index_src_left++){
        for(size_t header_cell_index_src_right = 0; header_cell_index_src_right < right_header.size(); header_cell_index_src_right++){
            if(left_header.at(header_cell_index_src_left).name == right_header.at(header_cell_index_src_right).name &&
                find(column_indices_src_left.begin(), column_indices_src_left.end(), header_cell_index_src_left) == column_indices_src_left.end() &&
                find(column_indices_src_right.begin(), column_indices_src_right.end(), header_cell_index_src_right) == column_indices_src_right.end()){
                if(left_header.at(header_cell_index_src_left).type != right_header.at(header_cell_index_src_right).type){
                    cout << WARNING_TEXT << ": in INTERSECT operation same named columns got different formats" << endl;
                    return nullptr;
                }
                column_indices_src_left.push_back(header_cell_index_src_left);
                column_indices_src_right.push_back(header_cell_index_src_right);
                break;
            }
        }
    }
    if(column_indices_src_left.size() == 0){
        cout << WARNING_TEXT << ": in INTERSECT operation tables do not have columns with the same name" << endl;
        return nullptr;
    }

    // create tmp table
    shared_ptr<table_c> tmp_table = make_shared<table_c>();

    // set name and header
    tmp_table->set_name("MULTITASK_" + to_string(id_tables_stack++));
    tmp_table->set_header(left_header);

    // open files
    ifstream ifs_left(BUFFER_FOLDER + left_table->get_name());
    if(!ifs_left.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return nullptr;
    }
    ifstream ifs_right(BUFFER_FOLDER + right_table->get_name());
    if(!ifs_right.is_open()){
        ifs_left.close();
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return nullptr;
    }
    ofstream ofs(BUFFER_FOLDER + tmp_table->get_name());
    if(!ofs.is_open()){
        ifs_left.close();
        ifs_right.close();
        cout << ERROR_TEXT << ": can't open output file" << endl;
        return nullptr;
    }

    // fill ofs with ifs_left INTERSECT ifs_right
    string ifs_left_line;
    while(getline(ifs_left, ifs_left_line)){
        // read left table line
        vector<string> words_ifs_left_line;
        stringstream ss_left(ifs_left_line);
        string word;
        while(getline(ss_left, word, ',')) words_ifs_left_line.push_back(word);

        // read right table lines
        ifs_right.clear();
        ifs_right.seekg(0);
        string ifs_right_line;
        while(getline(ifs_right, ifs_right_line)){
            // read right table line            
            vector<string> words_ifs_right_line;
            stringstream ss_right(ifs_right_line);
            while(getline(ss_right, word, ',')) words_ifs_right_line.push_back(word);
            
            // compare lines same columns elements
            bool success_compare = true;
            for(size_t index = 0; index < column_indices_src_left.size() && success_compare; index++){
                if(left_header.at(column_indices_src_left.at(index)).type == header_cell_c::type_e::FLOAT && right_header.at(column_indices_src_right.at(index)).type == header_cell_c::type_e::FLOAT){
                    // compare like float
                    if(stof(words_ifs_left_line.at(column_indices_src_left.at(index))) != stof(words_ifs_right_line.at(column_indices_src_right.at(index))))
                        success_compare = false;
                }else {
                    // compare like string, undefined
                    if(words_ifs_left_line.at(column_indices_src_left.at(index)) != words_ifs_right_line.at(column_indices_src_right.at(index)))
                        success_compare = false;
                }
            }
            if(success_compare == false){words_ifs_right_line.clear(); continue;}

            // compare others column elements
            for(size_t index = 0; index < words_ifs_left_line.size() && success_compare; index++){
                if(left_header.at(index).type == header_cell_c::type_e::FLOAT && right_header.at(index).type == header_cell_c::type_e::FLOAT){
                    // compare like float
                    if(stof(words_ifs_left_line.at(index)) != stof(words_ifs_right_line.at(index)))
                        success_compare = false;
                }else {
                    // compare like string, undefined
                    if(words_ifs_left_line.at(index) != words_ifs_right_line.at(index))
                        success_compare = false;
                }
            }
            if(success_compare == false){words_ifs_right_line.clear(); continue;}

            // add line to tmp_table
            ofs << ifs_left_line << endl;
        }
    }
    ifs_left.close();
    ifs_right.close();
    ofs.close();
    
    return tmp_table;
}

shared_ptr<table_c> multitask_ra_c::except_tmp_tables(shared_ptr<table_c> left_table, shared_ptr<table_c> right_table){
    
    // find column indices to compare elements
    vector<header_cell_c> left_header = left_table->get_header();
    vector<header_cell_c> right_header = right_table->get_header();
    if(left_header.size() != right_header.size()){
        cout << WARNING_TEXT << ": EXCEPTION operation cannot be performed because the number of columns in one table does not match the number of columns in another table" << endl;
        return nullptr;
    }
    vector<size_t> column_indices_src_left;
    vector<size_t> column_indices_src_right;
    for(size_t header_cell_index_src_left = 0; header_cell_index_src_left < left_header.size(); header_cell_index_src_left++){
        for(size_t header_cell_index_src_right = 0; header_cell_index_src_right < right_header.size(); header_cell_index_src_right++){
            if(left_header.at(header_cell_index_src_left).name == right_header.at(header_cell_index_src_right).name &&
                find(column_indices_src_left.begin(), column_indices_src_left.end(), header_cell_index_src_left) == column_indices_src_left.end() &&
                find(column_indices_src_right.begin(), column_indices_src_right.end(), header_cell_index_src_right) == column_indices_src_right.end()){
                if(left_header.at(header_cell_index_src_left).type != right_header.at(header_cell_index_src_right).type){
                    cout << WARNING_TEXT << ": in EXCEPTION operation same named columns got different formats" << endl;
                    return nullptr;
                }
                column_indices_src_left.push_back(header_cell_index_src_left);
                column_indices_src_right.push_back(header_cell_index_src_right);
                break;
            }
        }
    }
    if(column_indices_src_left.size() == 0){
        cout << WARNING_TEXT << ": in EXCEPTION operation tables do not have columns with the same name" << endl;
        return nullptr;
    }

    // create tmp table
    shared_ptr<table_c> tmp_table = make_shared<table_c>();

    // set name and header
    tmp_table->set_name("MULTITASK_" + to_string(id_tables_stack++));
    tmp_table->set_header(left_header);

    // open files
    ifstream ifs_left(BUFFER_FOLDER + (*left_table).get_name());
    if(!ifs_left.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return nullptr;
    }
    ifstream ifs_right(BUFFER_FOLDER + (*right_table).get_name());
    if(!ifs_right.is_open()){
        ifs_left.close();
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return nullptr;
    }
    ofstream ofs(BUFFER_FOLDER + tmp_table->get_name());
    if(!ofs.is_open()){
        ifs_left.close();
        ifs_right.close();
        cout << ERROR_TEXT << ": can't open output file" << endl;
        return nullptr;
    }

    // fill ofs with ifs_left EXCEPT ifs_right

    // read left table
    string ifs_left_line;
    while(getline(ifs_left, ifs_left_line)){
        // read left table line
        vector<string> words_ifs_left_line;
        stringstream ss_left(ifs_left_line);
        string word;
        while(getline(ss_left, word, ',')) words_ifs_left_line.push_back(word);

        // read right table
        bool it_unique_line = true;
        ifs_right.clear();
        ifs_right.seekg(0);
        string ifs_right_line;
        while(getline(ifs_right, ifs_right_line) && it_unique_line){
            // read right table line            
            vector<string> words_ifs_right_line;
            stringstream ss_right(ifs_right_line);
            while(getline(ss_right, word, ',')) words_ifs_right_line.push_back(word);
            
            // compare words in left and right lines
            for(size_t index = 0; index < words_ifs_left_line.size(); index++){
                if(left_header.at(index).type == header_cell_c::type_e::FLOAT && right_header.at(index).type == header_cell_c::type_e::FLOAT){
                    // it is float
                    if(stof(words_ifs_left_line.at(index)) != stof(words_ifs_right_line.at(index))) break;
                }else {
                    // it is string, undefined
                    if(words_ifs_left_line.at(index) != words_ifs_right_line.at(index)) break;
                }
                if(index == words_ifs_left_line.size() - 1) it_unique_line = false;
            }
            words_ifs_right_line.clear();
            if(!it_unique_line) break;
        }
        if(it_unique_line)
            ofs << ifs_left_line << endl;
    }
    ifs_left.close();
    ifs_right.close();
    ofs.close();
    
    return tmp_table;
}

shared_ptr<table_c> multitask_ra_c::union_tmp_tables(shared_ptr<table_c> left_table, shared_ptr<table_c> right_table){

    // find column indices to compare elements
    vector<header_cell_c> left_header = left_table->get_header();
    vector<header_cell_c> right_header = right_table->get_header();
    if(left_header.size() != right_header.size()){
        cout << WARNING_TEXT << ": UNION operation cannot be performed because the number of columns in one table does not match the number of columns in another table" << endl;
        return nullptr;
    }
    vector<size_t> column_indices_src_left;
    vector<size_t> column_indices_src_right;
    for(size_t header_cell_index_src_left = 0; header_cell_index_src_left < left_header.size(); header_cell_index_src_left++){
        for(size_t header_cell_index_src_right = 0; header_cell_index_src_right < right_header.size(); header_cell_index_src_right++){
            if(left_header.at(header_cell_index_src_left).name == right_header.at(header_cell_index_src_right).name &&
                find(column_indices_src_left.begin(), column_indices_src_left.end(), header_cell_index_src_left) == column_indices_src_left.end() &&
                find(column_indices_src_right.begin(), column_indices_src_right.end(), header_cell_index_src_right) == column_indices_src_right.end()){
                if(left_header.at(header_cell_index_src_left).type != right_header.at(header_cell_index_src_right).type){
                    cout << WARNING_TEXT << ": in UNION operation same named columns got different formats" << endl;
                    return nullptr;
                }
                column_indices_src_left.push_back(header_cell_index_src_left);
                column_indices_src_right.push_back(header_cell_index_src_right);
                break;
            }
        }
    }
    if(column_indices_src_left.size() == 0){
        cout << WARNING_TEXT << ": in UNION operation tables do not have columns with the same name" << endl;
        return nullptr;
    }

    // create tmp table
    shared_ptr<table_c> tmp_table = make_shared<table_c>();

    // set name and header
    tmp_table->set_name("MULTITASK_" + to_string(id_tables_stack++));
    tmp_table->set_header(left_header);

    // open files
    ifstream ifs_left(BUFFER_FOLDER + (*left_table).get_name());
    if(!ifs_left.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return nullptr;
    }
    ifstream ifs_right(BUFFER_FOLDER + (*right_table).get_name());
    if(!ifs_right.is_open()){
        ifs_left.close();
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return nullptr;
    }
    ofstream ofs(BUFFER_FOLDER + tmp_table->get_name());
    if(!ofs.is_open()){
        ifs_left.close();
        ifs_right.close();
        cout << ERROR_TEXT << ": can't open output file" << endl;
        return nullptr;
    }

    // fill ofs with ifs_left lines
    string ifs_left_line;
    while(getline(ifs_left, ifs_left_line)){
        ofs << ifs_left_line << endl;
    }

    // fill ofs with ifs_right lines, thats aren't in ifs_left
    
    // read right table
    string ifs_right_line;
    while(getline(ifs_right, ifs_right_line)){
        // read right table line
        vector<string> words_ifs_right_line;
        stringstream ss_right(ifs_right_line);
        string word;
        while(getline(ss_right, word, ',')) words_ifs_right_line.push_back(word);

        // read left table
        bool it_unique_line = true;
        ifs_left.clear();
        ifs_left.seekg(0);
        string ifs_left_line;
        while(getline(ifs_left, ifs_left_line)){
            // read left table line            
            vector<string> words_ifs_left_line;
            stringstream ss_left(ifs_left_line);
            while(getline(ss_left, word, ',')) words_ifs_left_line.push_back(word);
            
            // compare words in left and right lines
            for(size_t index = 0; index < words_ifs_right_line.size(); index++){
                if(left_header.at(index).type == header_cell_c::type_e::FLOAT && right_header.at(index).type == header_cell_c::type_e::FLOAT){
                    // it is float
                    if(stof(words_ifs_left_line.at(index)) != stof(words_ifs_right_line.at(index))) break;
                }else {
                    // it is string, undefined
                    if(words_ifs_left_line.at(index) != words_ifs_right_line.at(index)) break;
                }
                if(index == words_ifs_left_line.size() - 1) it_unique_line = false;
            }
            words_ifs_left_line.clear();
            if(!it_unique_line) break;
        }
        if(it_unique_line)
            ofs << ifs_right_line << endl;
    }
    ifs_left.close();
    ifs_right.close();
    ofs.close();
    
    return tmp_table;
}

bool multitask_ra_c::find_tokens(const string& request, tables_c& tables){

    // parse request
    for(const char* symbol_ptr = request.c_str(); *symbol_ptr; symbol_ptr++){
        // skip SPACE
        if((*symbol_ptr) == ' ' || (*symbol_ptr) == '\t') continue;

        // check if it is brackets
        if((*symbol_ptr) == '('){ tokens.push_back(token_s{"(", token_s::type_e::LEFT_BKT, token_s::associative_e::UNKNOWN, 0}); continue; }
        if((*symbol_ptr) == ')'){ tokens.push_back(token_s{")", token_s::type_e::RIGHT_BKT, token_s::associative_e::UNKNOWN, 0}); continue; }

        // check if it is word
        
        if(((*symbol_ptr) >= '0' && (*symbol_ptr) <= '9') || ((*symbol_ptr) >= 'A' && (*symbol_ptr) <= 'Z') || ((*symbol_ptr) >= 'a' && (*symbol_ptr) <= 'z') || (*symbol_ptr) == '_'){
            string word;
            while(((*symbol_ptr) >= '0' && (*symbol_ptr) <= '9') || ((*symbol_ptr) >= 'A' && (*symbol_ptr) <= 'Z') || ((*symbol_ptr) >= 'a' && (*symbol_ptr) <= 'z') || (*symbol_ptr) == '_')
                word += (*symbol_ptr++);
            symbol_ptr--;
            
            // check if it is operator
            if(word == "CROSS"){
                tokens.push_back(token_s{"CROSS", token_s::type_e::OPERATOR, token_s::associative_e::LEFT, 4});
            }else if(word == "INTERSECT"){
                tokens.push_back(token_s{"INTERSECT", token_s::type_e::OPERATOR, token_s::associative_e::LEFT, 3});
            }else if(word == "EXCEPT"){
                tokens.push_back(token_s{"EXCEPT", token_s::type_e::OPERATOR, token_s::associative_e::RIGHT, 2});
            }else if(word == "UNION"){
                tokens.push_back(token_s{"UNION", token_s::type_e::OPERATOR, token_s::associative_e::LEFT, 1});
            }else {
                // it is table
                // check in tables
                if(tables.get_table(word) == nullptr){
                    // this table doesn't exist
                    tokens.clear();
                    cout << WARNING_TEXT << ": requested table " << UNDERLINE_FONT << word << RESET_FONT << " doesn't exist" << endl;
                    return false;
                }else {
                    // add this table to tokens
                    tokens.push_back(token_s{word, token_s::type_e::TABLE, token_s::associative_e::UNKNOWN, 0});
                }
            }
        }
    }

    return true;
}

bool multitask_ra_c::make_queue(){

    // create stack for tokens
    vector<token_s> stack;

    // fill queue
    for(auto token : tokens){
        switch(token.type){
            case token_s::type_e::TABLE:
                tokens_queue.push_back(token);
                break;
            case token_s::type_e::OPERATOR:
                while(stack.empty() == false){
                    if((token.associative == token_s::associative_e::LEFT && token.priority <= stack.back().priority) ||
                       (token.associative == token_s::associative_e::RIGHT && token.priority < stack.back().priority)){
                        tokens_queue.push_back(stack.back());
                        stack.pop_back();
                        continue;
                    }
                    break;
                }
                stack.push_back(token);
                break;
            case token_s::type_e::LEFT_BKT:
                stack.push_back(token);
                break;
            case token_s::type_e::RIGHT_BKT:
            {
                bool found_pair_backet = false;
                while(stack.empty() == false && stack.back().type != token_s::type_e::LEFT_BKT){
                    tokens_queue.push_back(stack.back());
                    stack.pop_back();
                    found_pair_backet = true;
                }

                if(found_pair_backet == false || stack.empty() == true){
                    // Right backet is alone
                    cout << WARNING_TEXT << ": there is one extra RIGHT bracket in the request" << endl;
                    stack.clear();
                    tokens_queue.clear();
                    return false;
                }
                stack.pop_back();
            }
                break;
            default:
                cout << WARNING_TEXT << ": UNDEFINED type token" << endl;
                stack.clear();
                tokens_queue.clear();
                return false;
        }
    }

    // check if stack isn't empty
    while(stack.empty() == false){
        if(stack.back().type == token_s::type_e::LEFT_BKT){
            // Left backet is alone
            cout << WARNING_TEXT << ": there is one extra LEFT bracket in the request" << endl;
            stack.clear();
            tokens_queue.clear();
            return false;
        }

        tokens_queue.push_back(move(stack.back()));
        stack.pop_back();
    }

    return true;
}

bool multitask_ra_c::solve(const string& table_dst_name, const string& request, tables_c& tables){
    
    // create tokens
    if(!find_tokens(request, tables)){
        cout << WARNING_TEXT << ": something is wrong with finding tokens" << endl;
        return false;
    }

    // print tokens
    /*
    {
    cout << "===============================" << endl;
    cout << "TOKENS" << endl;
    for(auto token : tokens){
        cout << token.name << endl;
        cout << "\ttoken priority:\t" << token.priority << endl;
        cout << "\ttoken type:\t";
        switch(token.type){
            case token_s::type_e::UNKNOWN: cout << "UNKNOWN" << endl; break;
            case token_s::type_e::TABLE: cout << "TABLE" << endl; break;
            case token_s::type_e::OPERATOR: cout << "OPERATOR" << endl; break;
            case token_s::type_e::LEFT_BKT: cout << "LEFT_BKT" << endl; break;
            case token_s::type_e::RIGHT_BKT: cout << "RIGHT_BKT" << endl; break;
            default: break;
        }
        cout << "\ttoken assoc:\t";
        switch(token.associative){
            case token_s::associative_e::UNKNOWN: cout << "UNKNOWN" << endl; break;
            case token_s::associative_e::LEFT: cout << "LEFT" << endl; break;
            case token_s::associative_e::RIGHT: cout << "RIGHT" << endl; break;
            default: break;
        }
    }
    }
    */
    
    // create queue from tokens
    if(!make_queue()){
        cout << WARNING_TEXT << ": something is wrong with making queue from tokens" << endl;
        return false;
    }
    
    // print tokens queue
    /*
    {
    cout << "===============================" << endl;
    cout << "TOKENS QUEUE" << endl;
    for(auto token : tokens_queue){
        cout << token.name << endl;
        cout << "\ttoken queue priority:\t" << token.priority << endl;
        cout << "\token queue type:\t";
        switch(token.type){
            case token_s::type_e::UNKNOWN: cout << "UNKNOWN" << endl; break;
            case token_s::type_e::TABLE: cout << "TABLE" << endl; break;
            case token_s::type_e::OPERATOR: cout << "OPERATOR" << endl; break;
            case token_s::type_e::LEFT_BKT: cout << "LEFT_BKT" << endl; break;
            case token_s::type_e::RIGHT_BKT: cout << "RIGHT_BKT" << endl; break;
            default: break;
        }
        cout << "\token queue assoc:\t";
        switch(token.associative){
            case token_s::associative_e::UNKNOWN: cout << "UNKNOWN" << endl; break;
            case token_s::associative_e::LEFT: cout << "LEFT" << endl; break;
            case token_s::associative_e::RIGHT: cout << "RIGHT" << endl; break;
            default: break;
        }
    }
    cout << "===============================" << endl;
    }
    */

    // solve
    while(!tokens_queue.empty()){
        token_s token = tokens_queue.front();
        tokens_queue.pop_front();
        switch(token.type){
            case token_s::type_e::TABLE:
            {
                shared_ptr<table_c> new_table = create_tmp_table(token.name, tables);
                if(new_table == nullptr){
                    cout << WARNING_TEXT << ": something is wrong with making tmp table for multitasking" << endl;
                    return false;
                }
                tables_stack.push_back(new_table);
                break;
            }
            case token_s::type_e::OPERATOR:
            {
                // get right and left tables for operation
                if(tables_stack.empty()){
                    cout << WARNING_TEXT << ": RIGHT table is missing" << endl;
                    return false;
                }
                shared_ptr<table_c> right_table = tables_stack.back();
                tables_stack.pop_back();
                if(tables_stack.empty()){
                    cout << WARNING_TEXT << ": LEFT table is missing" << endl;
                    return false;
                }
                shared_ptr<table_c> left_table = tables_stack.back();
                tables_stack.pop_back();

                // provide operation
                if(token.name == "CROSS"){
                    // CROSS
                    shared_ptr<table_c> new_table = cross_tmp_tables(left_table, right_table);
                    if(new_table == nullptr){
                        cout << WARNING_TEXT << ": something is wrong with CROSS tables" << endl;
                        return false;
                    }
                    tables_stack.push_back(new_table);
                }else if(token.name == "INTERSECT"){
                    // INTERSECTION
                    shared_ptr<table_c> new_table = intersect_tmp_tables(left_table, right_table);
                    if(new_table == nullptr){
                        cout << WARNING_TEXT << ": something is wrong with INTERECTION tables" << endl;
                        return false;
                    }
                    tables_stack.push_back(new_table);
                }else if(token.name == "EXCEPT"){
                    // EXCEPTION
                    shared_ptr<table_c> new_table = except_tmp_tables(left_table, right_table);
                    if(new_table == nullptr){
                        cout << WARNING_TEXT << ": something is wrong with EXCEPTION tables" << endl;
                        return false;
                    }
                    tables_stack.push_back(new_table);
                }else {
                    // UNION
                    shared_ptr<table_c> new_table = union_tmp_tables(left_table, right_table);
                    if(new_table == nullptr){
                        cout << WARNING_TEXT << ": something is wrong with EXCEPTION tables" << endl;
                        return false;
                    }
                    tables_stack.push_back(new_table);
                }
                break;
            }
            default:
                cout << WARNING_TEXT << ": undefined token type" << endl;
                return false;
        }
    }

    // open files
    ifstream ifs(BUFFER_FOLDER + (*tables_stack.back()).get_name());
    if(!ifs.is_open()){
        cout << ERROR_TEXT << ": can't open input file" << endl;
        return true;
    }
    ofstream ofs(BUFFER_FOLDER + table_dst_name);
    if(!ofs.is_open()){
        ifs.close();
        cout << ERROR_TEXT << ": can't open output file" << endl;
        return true;
    }

    // fill ofs file
    string ifs_line;
    while(getline(ifs, ifs_line)){
        ofs << ifs_line << endl;
    }
    ifs.close();
    ofs.close();

    // add new table to tables
    if(!tables.add_table(table_dst_name, tables_stack.back()->get_header())){
        cout << WARNING_TEXT << ": can't add " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " to tables" << endl;
        return false;
    }

    tables_stack.pop_back();
    cout << SUCCESS_TEXT << ": multitask has been solved" << endl;
    
    return true;
}

// =============================================================================

multitask_sql_c::multitask_sql_c(){}

bool multitask_sql_c::find_tokens(const string& request){

    // parse request
    for(const char* symbol_ptr = request.c_str(); *symbol_ptr; symbol_ptr++){
        // skip SPACE
        if((*symbol_ptr) == ' ' || (*symbol_ptr) == '\t') continue;

        // check if it is brackets
        if((*symbol_ptr) == '('){ tokens.push_back(token_s{"(", token_s::type_e::LEFT_BKT, token_s::associative_e::UNKNOWN, 0}); continue; }
        if((*symbol_ptr) == ')'){ tokens.push_back(token_s{")", token_s::type_e::RIGHT_BKT, token_s::associative_e::UNKNOWN, 0}); continue; }

        // check if it is word
        
        if(((*symbol_ptr) >= '0' && (*symbol_ptr) <= '9') || ((*symbol_ptr) >= 'A' && (*symbol_ptr) <= 'Z') || ((*symbol_ptr) >= 'a' && (*symbol_ptr) <= 'z') || (*symbol_ptr) == '_'){
            string word;
            while(((*symbol_ptr) >= '0' && (*symbol_ptr) <= '9') || ((*symbol_ptr) >= 'A' && (*symbol_ptr) <= 'Z') || ((*symbol_ptr) >= 'a' && (*symbol_ptr) <= 'z') || (*symbol_ptr) == '_')
                word += (*symbol_ptr++);
            symbol_ptr--;
            
            // check if it is operator
            if(word == "CROSS"){
                tokens.push_back(token_s{"CROSS", token_s::type_e::OPERATOR, token_s::associative_e::LEFT, 4});
            }else if(word == "INTERSECT"){
                tokens.push_back(token_s{"INTERSECT", token_s::type_e::OPERATOR, token_s::associative_e::LEFT, 3});
            }else if(word == "EXCEPT"){
                tokens.push_back(token_s{"EXCEPT", token_s::type_e::OPERATOR, token_s::associative_e::RIGHT, 2});
            }else if(word == "UNION"){
                tokens.push_back(token_s{"UNION", token_s::type_e::OPERATOR, token_s::associative_e::LEFT, 1});
            }else {
                // it is table
                tokens.push_back(token_s{word, token_s::type_e::TABLE, token_s::associative_e::UNKNOWN, 0});
            }
        }
    }

    return true;
}

bool multitask_sql_c::make_queue(){

    // create stack for tokens
    vector<token_s> stack;

    // fill queue
    for(auto token : tokens){
        switch(token.type){
            case token_s::type_e::TABLE:
                tokens_queue.push_back(token);
                break;
            case token_s::type_e::OPERATOR:
                while(stack.empty() == false){
                    if((token.associative == token_s::associative_e::LEFT && token.priority <= stack.back().priority) ||
                       (token.associative == token_s::associative_e::RIGHT && token.priority < stack.back().priority)){
                        tokens_queue.push_back(stack.back());
                        stack.pop_back();
                        continue;
                    }
                    break;
                }
                stack.push_back(token);
                break;
            case token_s::type_e::LEFT_BKT:
                stack.push_back(token);
                break;
            case token_s::type_e::RIGHT_BKT:
            {
                bool found_pair_backet = false;
                while(stack.empty() == false && stack.back().type != token_s::type_e::LEFT_BKT){
                    tokens_queue.push_back(stack.back());
                    stack.pop_back();
                    found_pair_backet = true;
                }

                if(found_pair_backet == false || stack.empty() == true){
                    // Right backet is alone
                    cout << WARNING_TEXT << ": there is one extra RIGHT bracket in the request" << endl;
                    stack.clear();
                    tokens_queue.clear();
                    return false;
                }
                stack.pop_back();
            }
                break;
            default:
                cout << WARNING_TEXT << ": UNDEFINED type token" << endl;
                stack.clear();
                tokens_queue.clear();
                return false;
        }
    }

    // check if stack isn't empty
    while(stack.empty() == false){
        if(stack.back().type == token_s::type_e::LEFT_BKT){
            // Left backet is alone
            cout << WARNING_TEXT << ": there is one extra LEFT bracket in the request" << endl;
            stack.clear();
            tokens_queue.clear();
            return false;
        }

        tokens_queue.push_back(move(stack.back()));
        stack.pop_back();
    }

    return true;
}

bool multitask_sql_c::solve(const string& table_dst_name, const string& request){

    // create tokens
    if(!find_tokens(request)){
        cout << WARNING_TEXT << ": something is wrong with finding tokens" << endl;
        return false;
    }

    // create queue from tokens
    if(!make_queue()){
        cout << WARNING_TEXT << ": something is wrong with making queue from tokens" << endl;
        return false;
    }

    // create stack tables
    string result;

    // solve
    result += "CREATE TABLE " + table_dst_name + " AS\n";
    result += "SELECT * FROM (";

    while(!tokens_queue.empty()){
        token_s token = tokens_queue.front();
        tokens_queue.pop_front();
        switch(token.type){
            case token_s::type_e::TABLE:
            {
                tables_stack.push_back(token.name);
                break;
            }
            case token_s::type_e::OPERATOR:
            {
                // get right and left tables for operation
                if(tables_stack.empty()){
                    cout << WARNING_TEXT << ": RIGHT table is missing" << endl;
                    return false;
                }
                string right_table = tables_stack.back();
                tables_stack.pop_back();
                if(tables_stack.empty()){
                    cout << WARNING_TEXT << ": LEFT table is missing" << endl;
                    return false;
                }
                string left_table = tables_stack.back();
                tables_stack.pop_back();

                // provide operation
                if(token.name == "CROSS"){
                    // CROSS
                    string new_table;
                    new_table = "(SELECT * FROM (" + left_table + ") CROSS JOIN (SELECT * FROM (" + right_table + "))";
                    tables_stack.push_back(new_table);
                }else if(token.name == "INTERSECT"){
                    // INTERSECTION
                    string new_table;
                    new_table += "(SELECT * FROM (" + left_table + ") INTERSECT SELECT * FROM (" + right_table + "))";
                    tables_stack.push_back(new_table);
                }else if(token.name == "EXCEPT"){
                    // EXCEPTION
                    string new_table;
                    new_table += "(SELECT * FROM (" + left_table + ") EXCEPT SELECT * FROM (" + right_table + "))";
                    tables_stack.push_back(new_table);
                }else {
                    // UNION
                    string new_table;
                    new_table += "(SELECT * FROM (" + left_table + ") UNION SELECT * FROM (" + right_table + "))";
                    tables_stack.push_back(new_table);
                }
                break;
            }
            default:
                cout << WARNING_TEXT << ": undefined token type" << endl;
                return false;
        }
        
    }
    result += tables_stack.back();
    result += ")";

    cout << endl << result << endl << endl;
    return true;
}