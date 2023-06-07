#include "cmd_sql_natural_join.h"

cmd_sql_natural_join_c::cmd_sql_natural_join_c(){}

cmd_sql_natural_join_c::cmd_sql_natural_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_natural_join_c::run(tables_c& tables){
    regex natural_join_regex("\\s*NATURAL\\s*JOIN\\s*(\\w+)\\s*FROM\\s*(\\w+)\\s*,\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, natural_join_regex) && match.size() == 4){
        string table_name_dst = match[1];
        string table_name_src_1 = match[2];
        string table_name_src_2 = match[3];

        // print SQL
        cout << endl << "CREATE TABLE " << table_name_dst << " AS" << endl;
        cout << "FROM * " << table_name_src_1 << endl;
        cout << "NATURAL JOIN " << table_name_src_2 << ";" << endl << endl;

        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SQL NATURAL JOIN" << RESET_FONT << " aborted" << endl;
    return false;
}
