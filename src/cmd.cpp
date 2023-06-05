#include "cmd.h"

cmd_c::cmd_c(){}

cmd_c::cmd_c(const string& new_cmd_text)
    : cmd_text(new_cmd_text) {}

// ======================================================================
// Relation Algebra

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
            cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "IMPORT" << RESET_FONT << " aborted" << endl;
            return false;
        }

        return true;
    }

    return false;
}

// ==========================================

cmd_ra_export_c::cmd_ra_export_c(){}

cmd_ra_export_c::cmd_ra_export_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_export_c::run(tables_c& tables) {
    regex export_regex("\\s*EXPORT\\s*(\\w+)\\s*\"(\\w+)\\.(\\w+)\"\\s*");
    smatch match;

    // compare request
    if (regex_search(cmd_text, match, export_regex) && match.size() == 4) {
        string table_src_name = match[1];
        string file_dst_name = match[2];
        string file_dst_format = match[3];

        // find table
        shared_ptr<table_c> table_src = tables.get_table(table_src_name);
        if (table_src == nullptr) {
            cout << WARNING_TEXT << ": requested " << UNDERLINE_FONT << table_src_name << RESET_FONT << " table doesn't exist" << endl;
            return false;
        }

        // check correct format
        unique_ptr<file_export_c> file_exporter = file_factory::create_file_export(file_dst_format, file_dst_name, table_src);
        if (file_exporter == nullptr) {
            cout << WARNING_TEXT << ": unknown file format" << endl;
            return false;
        }

        file_exporter->export_file();
        return true;
    }

    cout << WARNING_TEXT << ": " << UNDERLINE_FONT << "EXPORT" << RESET_FONT << " aborted" << endl;
    return false;
}

// ==========================================

cmd_ra_help_c::cmd_ra_help_c(){}

cmd_ra_help_c::cmd_ra_help_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_help_c::run(tables_c& tables){
    cout << "RA HELP" << endl;

    return true;
}

// ==========================================

cmd_ra_print_c::cmd_ra_print_c(){}

cmd_ra_print_c::cmd_ra_print_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_print_c::run(tables_c& tables){
    regex import_regex("\\s*PRINT\\s*(\\w+)\\s*");
    match_results<string::const_iterator> match;

    // compare request
    if(regex_search(cmd_text, match, import_regex) && match.size() == 2){
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

    return false;
}

// ==========================================

cmd_ra_rename_c::cmd_ra_rename_c(){}

cmd_ra_rename_c::cmd_ra_rename_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_rename_c::run(tables_c& tables){
    cout << "RA RENAME" << endl;

    return true;
}

// ==========================================

cmd_ra_projection_c::cmd_ra_projection_c(){}

cmd_ra_projection_c::cmd_ra_projection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_projection_c::run(tables_c& tables){
    cout << "RA PROJECTION" << endl;

    return true;
}

// ==========================================

cmd_ra_selection_c::cmd_ra_selection_c(){}

cmd_ra_selection_c::cmd_ra_selection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_selection_c::run(tables_c& tables){
    cout << "RA SELECTION" << endl;

    return true;
}

// ==========================================

cmd_ra_natural_join_c::cmd_ra_natural_join_c(){}

cmd_ra_natural_join_c::cmd_ra_natural_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_natural_join_c::run(tables_c& tables){
    cout << "RA NATURAL JOIN" << endl;

    return true;
}

// ==========================================

cmd_ra_join_c::cmd_ra_join_c(){}

cmd_ra_join_c::cmd_ra_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_join_c::run(tables_c& tables){
    cout << "RA JOIN" << endl;

    return true;
}

// ==========================================

cmd_ra_multitask_c::cmd_ra_multitask_c(){}

cmd_ra_multitask_c::cmd_ra_multitask_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_multitask_c::run(tables_c& tables){
    cout << "RA MULTITASK" << endl;

    return true;
}

// ======================================================================
// SQL

cmd_sql_print_c::cmd_sql_print_c(){}

cmd_sql_print_c::cmd_sql_print_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_print_c::run(tables_c& tables){
    cout << "SQL PRINT" << endl;

    return true;
}

// ==========================================

cmd_sql_rename_c::cmd_sql_rename_c(){}

cmd_sql_rename_c::cmd_sql_rename_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_rename_c::run(tables_c& tables){
    cout << "SQL RENAME" << endl;

    return true;
}

// ==========================================

cmd_sql_projection_c::cmd_sql_projection_c(){}

cmd_sql_projection_c::cmd_sql_projection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_projection_c::run(tables_c& tables){
    cout << "SQL PROJECTION" << endl;

    return true;
}

// ==========================================

cmd_sql_selection_c::cmd_sql_selection_c(){}

cmd_sql_selection_c::cmd_sql_selection_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_selection_c::run(tables_c& tables){
    cout << "SQL SELECT" << endl;

    return true;
}

// ==========================================

cmd_sql_natural_join_c::cmd_sql_natural_join_c(){}

cmd_sql_natural_join_c::cmd_sql_natural_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_natural_join_c::run(tables_c& tables){
    cout << "SQL NATURAL JOIN" << endl;

    return true;
}

// ==========================================

cmd_sql_join_c::cmd_sql_join_c(){}

cmd_sql_join_c::cmd_sql_join_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_join_c::run(tables_c& tables){
    cout << "SQL JOIN" << endl;

    return true;
}

// ==========================================

cmd_sql_multitask_c::cmd_sql_multitask_c(){}

cmd_sql_multitask_c::cmd_sql_multitask_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_sql_multitask_c::run(tables_c& tables){
    cout << "SQL MULTITASK" << endl;

    return true;
}

// ==========================================