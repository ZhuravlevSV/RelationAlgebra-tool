#include "cmd_sql_join.h"

cmd_sql_join_c::cmd_sql_join_c(){}

cmd_sql_join_c::cmd_sql_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_join_c::run(tables_c& tables){
    regex join_regex("\\s*JOIN\\s*(\\w+)\\s*FROM\\s*(\\w+)\\s*,\\s*(\\w+)\\s*ON\\s*(\\w+)\\s*=\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, join_regex) && match.size() == 6){
        string table_name_dst = match[1];
        string table_name_src_1 = match[2];
        string table_name_src_2 = match[3];
        string table_att_src_1 = match[4];
        string table_att_src_2 = match[5];

        // print SQL
        cout << endl << "CREATE TABLE " << table_name_dst << " AS" << endl;
        cout << "FROM * " << table_name_src_1 << endl;
        cout << "JOIN " << table_name_src_2 << " ON " << table_name_src_1 << "." << table_att_src_1 << " = " << table_name_src_2 << "." << table_att_src_2 << ";" << endl << endl;

        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SQL JOIN" << RESET_FONT << " aborted" << endl;
    return false;
}
