#include "cmd_ra_multitask.h"

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
