#ifndef CMD_RA_MULTITASK_H
#define CMD_RA_MULTITASK_H

#include "../../cmd.h"

/// @brief Command class in RA format of type Multitask
class cmd_ra_multitask_c : public cmd_c{
public:

    /// @brief Base constructor
    cmd_ra_multitask_c();

    /// @brief Initial constructor with command text
    /// @param new_cmd_text New command text
    cmd_ra_multitask_c(const string& new_cmd_text);

    /// @brief Command Run Method
    /// @param tables Existing tables
    /// @return TRUE - if the command was executed correctly, FALSE - otherwise
    bool run(tables_c& tables) override;

};

#endif /* CMD_RA_MULTITASK_H */