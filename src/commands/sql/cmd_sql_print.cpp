#include "cmd_sql_print.h"

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
