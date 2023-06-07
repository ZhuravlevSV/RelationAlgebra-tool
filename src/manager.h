#ifndef MANAGER_H
#define MANAGER_H

#include "config.h"
#include "tables.h"
#include "cmd.h"
#include "commands/ra/cmd_ra_import.h"
#include "commands/ra/cmd_ra_export.h"
#include "commands/ra/cmd_ra_help.h"
#include "commands/ra/cmd_ra_print.h"
#include "commands/ra/cmd_ra_rename.h"
#include "commands/ra/cmd_ra_projection.h"
#include "commands/ra/cmd_ra_selection.h"
#include "commands/ra/cmd_ra_natural_join.h"
#include "commands/ra/cmd_ra_join.h"
#include "commands/ra/cmd_ra_multitask.h"
#include "commands/sql/cmd_sql_print.h"
#include "commands/sql/cmd_sql_rename.h"
#include "commands/sql/cmd_sql_projection.h"
#include "commands/sql/cmd_sql_selection.h"
#include "commands/sql/cmd_sql_natural_join.h"
#include "commands/sql/cmd_sql_join.h"
#include "commands/sql/cmd_sql_multitask.h"

using namespace std;

class selector_c{
public:

    enum class format_e {RA, SQL, UNDEFINED};
    
    enum class type_e {IMPORT, EXPORT, HELP, PRINT, RENAME, PROJECTION, SELECTION, NATURAL_JOIN, JOIN, MULTITASK, UNDEFINED};

    format_e define_format(string& cmd_text);

    type_e define_type(string& cmd_text);

private:

    map<string, selector_c::type_e> types_map = {
        {"IMPORT", type_e::IMPORT},
        {"EXPORT", type_e::EXPORT},
        {"HELP", type_e::HELP},
        {"PRINT", type_e::PRINT},
        {"RENAME", type_e::RENAME},
        {"PROJECTION", type_e::PROJECTION},
        {"SELECT", type_e::SELECTION},
        {"NATURAL", type_e::NATURAL_JOIN},
        {"JOIN", type_e::JOIN},
        {"MULTITASK", type_e::MULTITASK}
        };
};

class manager_c{
public:

    bool read_cmd(string& cmd_text);

    tables_c& get_tables();

    unique_ptr<cmd_c> set_cmd(string& cmd_text);

    selector_c selector;

private:

    tables_c tables;
};

class cmd_factory{
public:
    static unique_ptr<cmd_c> create_cmd(selector_c::format_e format, selector_c::type_e type, const string& cmd_text);
    
};


#endif /* MANAGER_H */