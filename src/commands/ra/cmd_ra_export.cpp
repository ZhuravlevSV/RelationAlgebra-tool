#include "cmd_ra_export.h"

cmd_ra_export_c::cmd_ra_export_c(){}

cmd_ra_export_c::cmd_ra_export_c(const string& new_cmd_text)
    : cmd_c(new_cmd_text) {}

bool cmd_ra_export_c::run(tables_c& tables) {
    regex export_regex("\\s*EXPORT\\s*(\\w+)\\s*\"(\\w+)\\.(\\w+)\"\\s*");
    match_results<string::const_iterator> match;

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
