#ifndef CMD_RA_JOIN_H
#define CMD_RA_JOIN_H

#include "../../cmd.h"

class cmd_ra_join_c : public cmd_c{
public:

    cmd_ra_join_c();

    cmd_ra_join_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

#endif /* CMD_RA_JOIN_H */