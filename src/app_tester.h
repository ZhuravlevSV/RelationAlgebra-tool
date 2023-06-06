#ifndef APP_TESTER_H
#define APP_TESTER_H

#include "config.h"
#include "manager.h"

using namespace std;

class app_tester_c{
public:

    void launch();

private:

    void print_start();

    void cmd_ok_work_ok(string cmd_text);

    void cmd_er_work_er(string cmd_text);

    void cmd_ok_work_er(string cmd_text);

    void print_finish();

    void create_folders();

    manager_c manager;
};

#endif /* APP_TESTER_H */