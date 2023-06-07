#include "cmd_sql_selection.h"

cmd_sql_selection_c::cmd_sql_selection_c(){}

cmd_sql_selection_c::cmd_sql_selection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_selection_c::run(tables_c& tables){
    regex selection_regex("\\s*SELECT\\s*(\\w+)\\s*OF\\s*(\\w+)\\s*\\((\\w+)\\s*(=|<|>)\\s*(\\w+)\\)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, selection_regex) && match.size() == 6){
        string table_name_dst = match[1];
        string table_name_src = match[2];
        string att = match[3];
        string op = match[4];
        string value = match[5];

        // print SQL
        cout << endl << "CREATE TABLE " << table_name_dst << " AS" << endl;
        cout << "SELECT * FROM " << table_name_src << endl;
        cout << "WHERE " << att << " " << op << " ";
        try{
            cout << stof(value) << ";" << endl << endl;
        }catch (const invalid_argument&){
            cout << "\"" << value << "\";" << endl << endl;
        }

        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "SQL SELECT" << RESET_FONT << " aborted" << endl;
    return false;
}
