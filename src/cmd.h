#ifndef CMD_H
#define CMD_H

#include "config.h"
#include "tables.h"
#include "file_export.h"
#include "multitask.h"

using namespace std;

class cmd_c{
public:

    cmd_c();

    cmd_c(const string& new_cmd_text);

    virtual ~cmd_c() = default;

    virtual bool run(tables_c& tables) = 0;

protected:

    string cmd_text;

};

#endif /* CMD_H */