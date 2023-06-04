#include "file_export.h"

file_export_c::file_export_c(){}

file_export_c::file_export_c(const string& new_file_name, shared_ptr<table_c> new_table_src)
    : file_name(new_file_name), table_src(new_table_src) {}

// ===================================================================

file_CSV_c::file_CSV_c(){}

file_CSV_c::file_CSV_c(const string& new_file_name, shared_ptr<table_c> new_table_src)
    : file_export_c(new_file_name, new_table_src) {}

void file_CSV_c::export(){
    
}
