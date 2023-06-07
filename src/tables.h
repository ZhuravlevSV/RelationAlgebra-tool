#ifndef TABLES_H
#define TABLES_H

#include "config.h"

using namespace std;

/// @brief Header cell class
class header_cell_c{
public:
    
    /// @brief Possible types of column under header cell
    enum class type_e {NONE, STRING, FLOAT, UNDEFINED};

    /// @brief Name of the current header cell
    string name;

    /// @brief Type of the current header cell
    type_e type;

    /// @brief Initial constructor with name and type of the current header cell
    /// @param new_name Header cell name
    /// @param new_type Header cell type
    header_cell_c(const string& new_name, const type_e& new_type);
};

/// @brief Table class
class table_c{
public:

    /// @brief Base constructor
    table_c();

    /// @brief Initial constructor with table name and his address
    /// @param new_name Table name
    /// @param file_address Table source address
    table_c(const string& new_name, const string& file_address);

    /// @brief Base destructor
    ~table_c();

    /// @brief Getter of the current table name
    /// @return Table name
    string get_name() const;

    /// @brief Setter of the current table name
    /// @param new_name New table name
    void set_name(const string& new_name);

    /// @brief Getter of the current table header
    /// @return Table header
    vector<header_cell_c> get_header() const;

    /// @brief Setter of the current table header
    /// @param new_header New table header
    void set_header(const vector<header_cell_c>& new_header);

    /// @brief Print current table
    void print() const;

private:

    /// @brief Check if current file is in valid format
    /// @param file_address File address to check a valid
    /// @return TRUE - if this file in CSV format, FALSE - otherwise
    bool is_valid_csv(const string& file_address);

    /// @brief Check if current text is float number
    /// @param text Text to check
    /// @return TRUE - if this text can be converted to float number, FALSE - otherwise
    bool is_number(const string& text);

    /// @brief Table name
    string name;
    
    /// @brief Table header
    vector<header_cell_c> header;

};

/// @brief Database of existsing tables class
class tables_c{
public:

    /// @brief Base constructor
    tables_c();

    /// @brief Add table from workspace folder to existing tables
    /// @param new_name New table name
    /// @param file_address Source file address of current table
    /// @return TRUE - if this table has been added to tables, FALSE - otherwise
    bool add_table_from_workspace(const string& new_name, const string& file_address);

    /// @brief Add table to existing tables
    /// @param table_name New table name
    /// @param table_header New table header
    /// @return TRUE - if this table has been added to tables, FALSE - otherwise
    bool add_table(const string& table_name, const vector<header_cell_c>& table_header);

    /// @brief Getter of the requested table name
    /// @param name Requested table name
    /// @return SMART POINTER - of requested table, NULLPTR - if requested table doesn't exist 
    const shared_ptr<table_c> get_table(const string& name);

    /// @brief Print requested table
    /// @param name Requested table name
    /// @return TRUE - if requested table exist, FALSE - otherwise
    bool print_table(const string& name) const;

private:

    /// @brief Database of existing tables
    map<string, shared_ptr<table_c>> tables_map;
};

#endif /* TABLES_H */