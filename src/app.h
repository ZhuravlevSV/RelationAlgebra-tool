#ifndef APP_H
#define APP_H

#include "config.h"
#include "manager.h"

using namespace std;

/// @brief Application class, which can be both a tester and a launcher
class app_c{
public:

    /// @brief Base destructor
    virtual ~app_c() = default;

    /// @brief Application launch method
    virtual void launch() = 0;

private:

    /// @brief Method that writes out the introductory message
    virtual void print_start() = 0;

    /// @brief Method that writes out the final message
    virtual void print_finish() = 0;

    /// @brief Method that creates folders if they don't exist
    virtual void create_folders() = 0;

protected:

    /// @brief Current application manager
    manager_c manager;

};

/// @brief Application launcher class
class app_launcher_c : public app_c {
public:

    /// @brief Application launch method
    void launch() override;

private:

    /// @brief Method that writes out the introductory message
    void print_start() override;

    /// @brief Method that writes out the final message
    void print_finish() override;
    
    /// @brief Method that creates folders if they don't exist
    void create_folders() override;

};

/// @brief Application tester class
class app_tester_c : public app_c {
public:

    /// @brief Application launch method
    void launch() override;

private:

    /// @brief Method that writes out the introductory message
    void print_start() override;

    /// @brief Method that writes out the final message
    void print_finish() override;
    
    /// @brief Method that creates folders if they don't exist
    void create_folders() override;

    /// @brief A method that correctly determines the command and the command is executed correctly
    /// @param cmd_text Command text
    void cmd_ok_work_ok(string cmd_text);

    /// @brief Method that does not define a command
    /// @param cmd_text Command text
    void cmd_er_work_er(string cmd_text);

    /// @brief A method that correctly defines a command, but the command is not executed
    /// @param cmd_text Command text
    void cmd_ok_work_er(string cmd_text);

};

/// @brief Class to define application type
class app_factory_c{
public:
    /// @brief Method that determines the application type
    /// @param argc Number of arguments
    /// @param argv Arguments
    /// @return Smart pointer for an application
    static unique_ptr<app_c> create_app(int argc, char* argv[]);
};

#endif /* APP_H */