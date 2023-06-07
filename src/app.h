#ifndef APP_H
#define APP_H

#include "config.h"
#include "manager.h"

using namespace std;

class app_c{
public:

    virtual ~app_c() = default;

    virtual void launch() = 0;

private:

    virtual void print_start() = 0;

    virtual void print_finish() = 0;

    virtual void create_folders() = 0;

protected:

    manager_c manager;

};

class app_launcher_c : public app_c {
public:

    void launch() override;

private:

    void print_start() override;

    void print_finish() override;
    
    void create_folders() override;

};

class app_tester_c : public app_c {
public:

    void launch() override;

private:

    void print_start() override;

    void print_finish() override;
    
    void create_folders() override;

    void cmd_ok_work_ok(string cmd_text);

    void cmd_er_work_er(string cmd_text);

    void cmd_ok_work_er(string cmd_text);

};

class app_factory_c{
public:
    static unique_ptr<app_c> create_app(int argc, char* argv[]);
};

#endif /* APP_H */