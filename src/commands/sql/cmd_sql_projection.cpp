#include "cmd_sql_projection.h"

cmd_sql_projection_c::cmd_sql_projection_c(){}

cmd_sql_projection_c::cmd_sql_projection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_projection_c::run(tables_c& tables){
    regex projection_regex("\\s*PROJECTION\\s*(\\w+)\\s*OF\\s*(\\w+)\\s*\\[(\\w+(?:,\\s*\\w+)*)\\]\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, projection_regex) && match.size() == 4){
        string table_name_dst = match[1];
        string table_name_src = match[2];
        string attributes = match[3];

        // print SQL
        cout << endl << "CREATE TABLE " << table_name_dst << " AS" << endl;
        cout << "SELECT " << attributes << endl;
        cout << "FROM " << table_name_src << ";" << endl << endl;
        
        return true;
    }
    
    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SQL PROJECTION" << RESET_FONT << " aborted" << endl;
    return false;
}
