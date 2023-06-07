#include "cmd_ra_import.h"

cmd_ra_import_c::cmd_ra_import_c(){}

cmd_ra_import_c::cmd_ra_import_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_import_c::run(tables_c& tables){
    regex import_regex("\\s*IMPORT\\s*(\\w+)\\s*\"(.+)\"\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, import_regex) && match.size() == 3){
        string table_dst_name = match[1];
        string table_dst_file_address = match[2];

        if(!tables.add_table_from_workspace(table_dst_name, table_dst_file_address)){
            cout << WARNING_TEXT << ": " << UNDERLINE_FONT << table_dst_name << RESET_FONT << " can't be added" << endl;
            return false;
        }

        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "IMPORT" << RESET_FONT << " aborted" << endl;
    return false;
}

