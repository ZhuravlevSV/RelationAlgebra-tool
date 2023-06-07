#include "cmd_ra_help.h"

cmd_ra_help_c::cmd_ra_help_c(){}

cmd_ra_help_c::cmd_ra_help_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_help_c::run(tables_c& tables){
    regex help_regex("\\s*HELP\\s*");
    match_results<string::const_iterator> match;

    if(regex_search(cmd_text, match, help_regex) && match.size() == 1){
        cout << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add a table write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "IMPORT" << RESET_FONT << BOLD_FONT << " table_name " << YELLOW_COLOR << "\"" << RESET_FONT << BOLD_FONT << "file_name.csv" << YELLOW_COLOR << "\""<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To export a table in CSV or XML format write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "EXPORT" << RESET_FONT << BOLD_FONT << " table_name " << YELLOW_COLOR << "\"" << RESET_FONT << BOLD_FONT << "file_name.csv/.xml" << YELLOW_COLOR << "\""<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To print out the table, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "PRINT" << RESET_FONT << BOLD_FONT << " table_name"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To rename the name of a table attribute, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "RENAME" << RESET_FONT << BOLD_FONT << " table_name old_attribute " << YELLOW_COLOR << "AS" << RESET_FONT << BOLD_FONT << " new_attribute"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add new table by projection with certain attributes from other table, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "PROJECTION" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "OF" << RESET_FONT << BOLD_FONT << " source_table_name " << YELLOW_COLOR << "[" << RESET_FONT << BOLD_FONT << "att1, att2, ... , attN" << YELLOW_COLOR << "]"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add new table by selection with certain values of attributes from other table, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "SELECT" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "OF" << RESET_FONT << BOLD_FONT << " source_table_name " << YELLOW_COLOR << "(" << RESET_FONT << BOLD_FONT << "att =/</> value" << YELLOW_COLOR << ")"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add new table by NATURAL JOIN of two other tables, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "NATURAL JOIN" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "FROM" << RESET_FONT << BOLD_FONT << " source_table_name_1, source_table_name_2"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add new table by JOIN of two other tables via some attribute, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "JOIN" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "FROM" << RESET_FONT << BOLD_FONT << " source_table_name_1, source_table_name_2 " << YELLOW_COLOR << "ON" << RESET_FONT << BOLD_FONT << " table_1_att " << YELLOW_COLOR << "=" << RESET_FONT << BOLD_FONT << " table_2_att"<< RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To add a table by running a multi-query, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "MULTITASK" << RESET_FONT << BOLD_FONT << " new_table_name " << YELLOW_COLOR << "FROM" << RESET_FONT << BOLD_FONT << " task" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Also commands for multitask" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Cartesian multiplication" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> table_name_1 " << YELLOW_COLOR << "CROSS" << RESET_FONT << BOLD_FONT << " table_name_2" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Intersection" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> table_name_1 " << YELLOW_COLOR << "INTERSECT" << RESET_FONT << BOLD_FONT << " table_name_2" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Exception" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> table_name_1 " << YELLOW_COLOR << "EXCEPT" << RESET_FONT << BOLD_FONT << " table_name_2" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    Union" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    " << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> table_name_1 " << YELLOW_COLOR << "UNION" << RESET_FONT << BOLD_FONT << " table_name_2" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To convert a query to SQL format, write the command" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "TO SQL" << RESET_FONT << BOLD_FONT << " request" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << "    To quit from application" << endl;
        cout << BOLD_FONT << BLUE_COLOR << "|" << CYAN_COLOR << "|" << RESET_FONT << " " << BOLD_FONT << ">> " << YELLOW_COLOR << "CTRL" << RESET_FONT << BOLD_FONT << " + " << YELLOW_COLOR << "D" << RESET_FONT << endl;
        cout << endl;
        return true;
    }

    return false;
}
