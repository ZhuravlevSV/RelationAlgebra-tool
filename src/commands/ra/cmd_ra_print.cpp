#include "cmd_ra_print.h"

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
