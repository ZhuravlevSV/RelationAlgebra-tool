#ifndef FILE_EXPORT_H
#define FILE_EXPORT_H

#include "config.h"
#include "tables.h"

using namespace std;

class file_export_c{
public:

    file_export_c();

    file_export_c(const string& new_file_name, shared_ptr<table_c> new_table_src);

    virtual void export() = 0;

protected:

    string file_name;
    shared_ptr<table_c> table_src;
};

class file_CSV_c : public file_export_c{
public:

    file_CSV_c();

    file_CSV_c(const string& new_file_name, shared_ptr<table_c> new_table_src);

    void export() override;
};

class file_XML_c : public file_export_c{
public:

    file_XML_c();

    file_XML_c(const string& new_file_name, shared_ptr<table_c> new_table_src);

    void export() override;
};

class file_JSON_c : public file_export_c{
public:

    file_JSON_c();

    file_JSON_c(const string& new_file_name, shared_ptr<table_c> new_table_src);

    void export() override;
};

#endif /* FILE_EXPORT_H */