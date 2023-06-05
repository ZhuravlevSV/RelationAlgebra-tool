#ifndef CMD_H
#define CMD_H

#include "config.h"
#include "tables.h"
#include "file_export.h"

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

// ==========================================
// Relation Algebra

class cmd_ra_import_c : public cmd_c{
public:

    cmd_ra_import_c();

    cmd_ra_import_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_ra_export_c : public cmd_c{
public:

    cmd_ra_export_c();

    cmd_ra_export_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_ra_help_c : public cmd_c{
public:

    cmd_ra_help_c();

    cmd_ra_help_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_ra_print_c : public cmd_c{
public:

    cmd_ra_print_c();

    cmd_ra_print_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_ra_rename_c : public cmd_c{
public:

    cmd_ra_rename_c();

    cmd_ra_rename_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_ra_projection_c : public cmd_c{
public:

    cmd_ra_projection_c();

    cmd_ra_projection_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_ra_selection_c : public cmd_c{
public:

    cmd_ra_selection_c();

    cmd_ra_selection_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_ra_natural_join_c : public cmd_c{
public:

    cmd_ra_natural_join_c();

    cmd_ra_natural_join_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_ra_join_c : public cmd_c{
public:

    cmd_ra_join_c();

    cmd_ra_join_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_ra_multitask_c : public cmd_c{
public:

    cmd_ra_multitask_c();

    cmd_ra_multitask_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

// ==========================================
// SQL

class cmd_sql_print_c : public cmd_c{
public:

    cmd_sql_print_c();

    cmd_sql_print_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_sql_rename_c : public cmd_c{
public:

    cmd_sql_rename_c();

    cmd_sql_rename_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_sql_projection_c : public cmd_c{
public:

    cmd_sql_projection_c();

    cmd_sql_projection_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_sql_selection_c : public cmd_c{
public:

    cmd_sql_selection_c();

    cmd_sql_selection_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_sql_natural_join_c : public cmd_c{
public:

    cmd_sql_natural_join_c();

    cmd_sql_natural_join_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_sql_join_c : public cmd_c{
public:

    cmd_sql_join_c();

    cmd_sql_join_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

class cmd_sql_multitask_c : public cmd_c{
public:

    cmd_sql_multitask_c();

    cmd_sql_multitask_c(const string& new_cmd_text);

    bool run(tables_c& tables) override;

};

#endif /* CMD_H */