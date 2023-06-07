#ifndef CMD_SQL_SELECTION_H
#define CMD_SQL_SELECTION_H

#include "../../cmd.h"

/// @brief Command class in SQL format of type Selection
class cmd_sql_selection_c : public cmd_c{
public:

    /// @brief Base constructor
    cmd_sql_selection_c();

    /// @brief Initial constructor with command text
    /// @param new_cmd_text New command text
    cmd_sql_selection_c(const string& new_cmd_text);

    /// @brief Command Run Method
    /// @param tables Existing tables
    /// @return TRUE - if the command was executed correctly, FALSE - otherwise
    bool run(tables_c& tables) override;

};

#endif /* CMD_SQL_SELECTION_H */