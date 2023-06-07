#ifndef CMD_H
#define CMD_H

#include "config.h"
#include "tables.h"
#include "file_export.h"
#include "multitask.h"

using namespace std;

/// @brief The parent class of the command to be executed
class cmd_c{
public:

    /// @brief Base constructor
    cmd_c();

    /// @brief Constructor with current command text
    /// @param new_cmd_text Command text
    cmd_c(const string& new_cmd_text);

    /// @brief Base destructor
    virtual ~cmd_c() = default;

    /// @brief Command Run Method
    /// @param tables Existing tables
    /// @return TRUE - if the command is successful, FALSE - otherwise
    virtual bool run(tables_c& tables) = 0;

protected:

    /// @brief Command text
    string cmd_text;

};

#endif /* CMD_H */