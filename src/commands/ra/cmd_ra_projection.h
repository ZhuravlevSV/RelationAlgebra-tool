#ifndef CMD_RA_PROJECTION_H
#define CMD_RA_PROJECTION_H

#include "../../cmd.h"

class cmd_ra_projection_c : public cmd_c{
public:

    cmd_ra_projection_c();

    cmd_ra_projection_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

#endif /* CMD_RA_PROJECTION_H */