#ifndef APP_H
#define APP_H

#include "config.h"
#include "manager.h"

using namespace std;

class app_c{
public:

    void launch();

private:

    void print_start();

    void print_finish();

    void create_folders();

    manager_c manager;
};

#endif /* APP_H */