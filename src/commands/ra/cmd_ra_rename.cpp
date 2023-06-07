#include "cmd_ra_rename.h"

cmd_ra_rename_c::cmd_ra_rename_c(){}

cmd_ra_rename_c::cmd_ra_rename_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_rename_c::run(tables_c& tables){
    regex rename_regex("\\s*RENAME\\s*(\\w+)\\s*(\\w+)\\s*AS\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, rename_regex) && match.size() == 4){
        string table_name = match[1];
        string table_attribute_old = match[2];
        string table_attribute_new = match[3];

        // try to find table
        shared_ptr<table_c> table = tables.get_table(table_name);
        if(table == nullptr){
            cout << WARNING_TEXT << ": table " << UNDERLINE_FONT << table_name << RESET_FONT << " doesn't exist" << endl;
            return false;
        }

        // change attribute name
        vector<header_cell_c> header = table->get_header();
        for(size_t index = 0; index < header.size(); index++){
            if(header.at(index).name == table_attribute_old){
                header.at(index).name = table_attribute_new;
                break;
            }
            if(index == header.size() - 1){
                cout << WARNING_TEXT << ": " << UNDERLINE_FONT << table_name << RESET_FONT << " table hasn't " << UNDERLINE_FONT << table_attribute_old << RESET_FONT << " attribute" << endl;
                return false;
            }
        }

        table->set_header(header);
        cout << SUCCESS_TEXT << ": renamed " << UNDERLINE_FONT << table_attribute_old << RESET_FONT << " as " << UNDERLINE_FONT << table_attribute_new << RESET_FONT << " in " << UNDERLINE_FONT << table_name << RESET_FONT << " table" << endl;
        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "RENAME" << RESET_FONT << " aborted" << endl;
    return false;
}
