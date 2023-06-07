#include "cmd_sql_rename.h"

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

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SQL RENAME" << RESET_FONT << " aborted" << endl;
    return false;
}
