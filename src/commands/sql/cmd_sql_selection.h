#ifndef CMD_SQL_SELECTION_H
#define CMD_SQL_SELECTION_H

#include "../../cmd.h"

class cmd_sql_selection_c : public cmd_c{
public:

    cmd_sql_selection_c();

    cmd_sql_selection_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

#endif /* CMD_SQL_SELECTION_H */