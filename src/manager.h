#ifndef MANAGER_H
#define MANAGER_H

#include "config.h"
#include "tables.h"
#include "cmd.h"

using namespace std;

class selector_c{
public:

    enum class format_e {RA, SQL, UNDEFINED};
    
    enum class type_e {IMPORT, EXPORT, HELP, PRINT, RENAME, PROJECTION, SELECTION, NATURAL_JOIN, JOIN, MULTITASK, UNDEFINED};

    format_e define_format(string& cmd_text);

    type_e define_type(string& cmd_text);

private:

    map<string, selector_c::type_e> types_map = {
        {"IMPORT", type_e::IMPORT},
        {"EXPORT", type_e::EXPORT},
        {"HELP", type_e::HELP},
        {"PRINT", type_e::PRINT},
        {"RENAME", type_e::RENAME},
        {"PROJECTION", type_e::PROJECTION},
        {"SELECT", type_e::SELECTION},
        {"NATURAL", type_e::NATURAL_JOIN},
        {"JOIN", type_e::JOIN},
        {"MULTITASK", type_e::MULTITASK}
        };
};

class manager_c{
public:

    bool read_cmd(string& cmd_text);

    tables_c& get_tables();

    unique_ptr<cmd_c> set_cmd(string& cmd_text);

    selector_c selector;

private:

    tables_c tables;
};

class cmd_factory{
public:
    static unique_ptr<cmd_c> create_cmd(selector_c::format_e format, selector_c::type_e type, const string& cmd_text){
        switch(type){
        case selector_c::type_e::HELP:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_help_c>(cmd_text);
            else
                return nullptr;
        case selector_c::type_e::IMPORT:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_import_c>(cmd_text);
            else
                return nullptr;
        case selector_c::type_e::EXPORT:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_export_c>(cmd_text);
            else
                return nullptr;
        case selector_c::type_e::PRINT:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_print_c>(cmd_text);
            else
                return make_unique<cmd_sql_print_c>(cmd_text);
        case selector_c::type_e::RENAME:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_rename_c>(cmd_text);
            else
                return make_unique<cmd_sql_rename_c>(cmd_text);
        case selector_c::type_e::PROJECTION:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_projection_c>(cmd_text);
            else
                return make_unique<cmd_sql_projection_c>(cmd_text);
        case selector_c::type_e::SELECTION:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_selection_c>(cmd_text);
            else
                return make_unique<cmd_sql_selection_c>(cmd_text);
        case selector_c::type_e::NATURAL_JOIN:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_natural_join_c>(cmd_text);
            else
                return make_unique<cmd_sql_natural_join_c>(cmd_text);
        case selector_c::type_e::JOIN:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_join_c>(cmd_text);
            else
                return make_unique<cmd_sql_join_c>(cmd_text);
        case selector_c::type_e::MULTITASK:
            if(format == selector_c::format_e::RA)
                return make_unique<cmd_ra_multitask_c>(cmd_text);
            else
                return make_unique<cmd_sql_multitask_c>(cmd_text);
        
        default:
            return nullptr;
        }
    }
};


#endif /* MANAGER_H */