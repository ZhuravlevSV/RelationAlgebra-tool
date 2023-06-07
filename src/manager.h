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

/// @brief Selector to define command format and type from command text
class selector_c{
public:

    /// @brief Possible formats
    enum class format_e {RA, SQL, UNDEFINED};
    
    /// @brief Possible types
    enum class type_e {IMPORT, EXPORT, HELP, PRINT, RENAME, PROJECTION, SELECTION, NATURAL_JOIN, JOIN, MULTITASK, UNDEFINED};

    /// @brief Method for define command format from command text
    /// @param cmd_text Command text
    /// @return Current command format
    format_e define_format(string& cmd_text);

    /// @brief Method for define command type from command text
    /// @param cmd_text Command text
    /// @return Current command type
    type_e define_type(string& cmd_text);

private:

    /// @brief Map of possible command types
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

/// @brief Manager of application to execute input commands
class manager_c{
public:

    /// @brief Read command from input
    /// @param cmd_text Command text
    /// @return TRUE - if command is received from input, FALSE - otherwise
    bool read_cmd(string& cmd_text);

    /// @brief Getter of existing tables
    /// @return Tables
    tables_c& get_tables();

    /// @brief Setter of current command
    /// @param cmd_text Command text
    /// @return Smart pointer for an specific command
    unique_ptr<cmd_c> set_cmd(string& cmd_text);

    /// @brief Selector to define command format and type from command text
    selector_c selector;

private:

    /// @brief Existing tables
    tables_c tables;
};

/// @brief Class to define specific command
class cmd_factory{
public:

    /// @brief Method that determines command
    /// @param format Format of current command
    /// @param type Type of current command
    /// @param cmd_text Command text
    /// @return Smart pointer for a command
    static unique_ptr<cmd_c> create_cmd(selector_c::format_e format, selector_c::type_e type, const string& cmd_text);
    
};

#endif /* MANAGER_H */