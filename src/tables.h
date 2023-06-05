#ifndef TABLES_H
#define TABLES_H

#include "config.h"

using namespace std;

// ===========================================================================

class header_cell_c{
public:
    
    enum class type_e {NONE, STRING, FLOAT, UNDEFINED};

    string name;
    type_e type;

    header_cell_c(const string& new_name, const type_e& new_type);
};

// ===========================================================================

class table_c{
public:

    table_c();

    table_c(const string& new_name, const string& file_address);

    ~table_c();

    string get_name() const;

    vector<header_cell_c> get_header() const;

    void set_header(const vector<header_cell_c>& new_header);

    void print() const;

private:

    string name;
    vector<header_cell_c> header;

};

// ===========================================================================

class tables_c{
public:

    tables_c();

    bool add_table_from_workspace(const string& new_name, const string& file_address);

    const shared_ptr<table_c> get_table(const string& name);

    bool print_table(const string& name) const;

private:

    map<string, shared_ptr<table_c>> tables_map;
};

// ===========================================================================
// files functions

bool is_valid_csv(const string& file_address);

bool is_number(const string& text);

#endif /* TABLES_H */