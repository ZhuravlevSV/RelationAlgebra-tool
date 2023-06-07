#ifndef FILE_EXPORT_H
#define FILE_EXPORT_H

#include "config.h"
#include "tables.h"

using namespace std;

/// @brief File to export class
class file_export_c{
public:

    /// @brief Base constructor
    file_export_c();
    
    /// @brief Constructor with file name and source table
    /// @param new_file_name Name for current file
    /// @param new_table_src Table source
    file_export_c(const string& new_file_name, shared_ptr<table_c> new_table_src);
    
    /// @brief Method for export
    virtual void export_file() = 0;

protected:

    /// @brief Name of current file
    string file_name;

    /// @brief Table source for current file
    shared_ptr<table_c> table_src;
};

/// @brief Class for exporting a file in CSV format
class file_CSV_c : public file_export_c{
public:

    /// @brief Base constructor
    file_CSV_c();

    /// @brief Constructor with file name and source table
    /// @param new_file_name Name for current file
    /// @param new_table_src Table source
    file_CSV_c(const string& new_file_name, shared_ptr<table_c> new_table_src);

    /// @brief Method for export
    void export_file() override;
};

/// @brief Class for exporting a file in XML format
class file_XML_c : public file_export_c{
public:

    /// @brief Base constructor
    file_XML_c();

    /// @brief Constructor with file name and source table
    /// @param new_file_name Name for current file
    /// @param new_table_src Table source
    file_XML_c(const string& new_file_name, shared_ptr<table_c> new_table_src);

    /// @brief Method for export
    void export_file() override;
};

/// @brief Class for exporting a file in JSON format
class file_JSON_c : public file_export_c{
public:

    /// @brief Base constructor
    file_JSON_c();

    /// @brief Constructor with file name and source table
    /// @param new_file_name Name for current file
    /// @param new_table_src Table source
    file_JSON_c(const string& new_file_name, shared_ptr<table_c> new_table_src);

    /// @brief Method for export
    void export_file() override;
};

/// @brief Class to define file export format
class file_factory{
public:

    /// @brief Possible export formats
    enum class formats { CSV, XML, JSON, UNDEFINED };

    /// @brief Method that determines the file export format
    /// @param file_format File format
    /// @param file_name File name
    /// @param table_src Table source
    /// @return Smart pointer for an file export format
    static unique_ptr<file_export_c> create_file_export(const string& file_format, const string& file_name, shared_ptr<table_c> table_src);

};

#endif /* FILE_EXPORT_H */