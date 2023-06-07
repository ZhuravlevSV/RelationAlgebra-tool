#include "cmd_sql_multitask.h"

cmd_sql_multitask_c::cmd_sql_multitask_c(){}

cmd_sql_multitask_c::cmd_sql_multitask_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_multitask_c::run(tables_c& tables){
    regex multitask_regex("\\s*MULTITASK\\s*(\\w+)\\s*FROM\\s*(.+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, multitask_regex) && match.size() > 1){
        string table_dst_name = match[1];
        string request = match[2];

        for(size_t index = 0; index < request.length(); index++){
            if(request[index] != ' ' && request[index] != '\t' && request[index] != '\n' && request[index] != '\r') break;
            if(index == request.length() - 1){
                cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SQL MULTITASK" << RESET_FONT << " aborted" << endl;
                return false;
            }
        }

        // try to solve multitask
        multitask_sql_c multitask;
        if(!multitask.solve(table_dst_name, request)){
            cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SQL MULTITASK" << RESET_FONT << " aborted" << endl;
            return false;
        }

        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SQL MULTITASK" << RESET_FONT << " aborted" << endl;
    return false;
}
