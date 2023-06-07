#ifndef CMD_RA_PRINT_H
#define CMD_RA_PRINT_H

#include "../../cmd.h"

class cmd_ra_print_c : public cmd_c{
public:

    cmd_ra_print_c();

    cmd_ra_print_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

#endif /* CMD_RA_PRINT_H */