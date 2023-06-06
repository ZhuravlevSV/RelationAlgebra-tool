#include "manager.h"

selector_c::format_e selector_c::define_format(string& cmd_text){
    regex ra_regex("\\s*(\\w+)");
    regex sql_regex("\\s*TO\\s*SQL\\s*(\\w+)");
    match_results<string::const_iterator> match;

    // compare formats
    if(regex_search(cmd_text, match, sql_regex) && match.size() == 2){
        string new_cmd_text = match[1];
        new_cmd_text += " ";
        new_cmd_text += cmd_text.substr(match[0].length());
        cmd_text = new_cmd_text;
        return selector_c::format_e::SQL;
    }
    if(regex_search(cmd_text, match, ra_regex) && match.size() == 2){
        return selector_c::format_e::RA;
    }
    
    return selector_c::format_e::UNDEFINED;
}

selector_c::type_e selector_c::define_type(string& cmd_text){
    regex type_regex("\\s*(\\w+)");
    match_results<string::const_iterator> match;

    // compare types
    if(regex_search(cmd_text, match, type_regex) && match.size() == 2){
        string keyword = match[1];

        // find keyword in types
        auto it_type = types_map.find(keyword);
        if(it_type != types_map.end())
            return it_type->second;
    }

    return selector_c::type_e::UNDEFINED;
}

// ===============================================

bool manager_c::read_cmd(string& cmd_text){

    bool result = false;
    cout << BOLD_FONT << ">> ";
    while(getline(cin, cmd_text)){
        if(cmd_text.length() == 0){
            cout << ">> ";
            continue;
        }
        result = true;
        break;
    }
    cout << RESET_FONT;

    if(!result)
        cout << endl;
    return result;
}

unique_ptr<cmd_c> manager_c::set_cmd(string& cmd_text){

    // define format and type of the command
    selector_c::format_e format = selector.define_format(cmd_text);
    selector_c::type_e type = selector.define_type(cmd_text);

    // set command
    return cmd_factory::create_cmd(format, type, cmd_text);
}

tables_c& manager_c::get_tables(){
    return tables;
}

// ===============================================

unique_ptr<cmd_c> cmd_factory::create_cmd(selector_c::format_e format, selector_c::type_e type, const string& cmd_text){
    switch(type){
    case selector_c::type_e::HELP:
        if(format == selector_c::format_e::RA)
            return make_unique<cmd_ra_help_c>(cmd_text);
        else{
            cout << WARNING_TEXT << ": Unknown command" << endl;
            return nullptr;
        }
    case selector_c::type_e::IMPORT:
        if(format == selector_c::format_e::RA)
            return make_unique<cmd_ra_import_c>(cmd_text);
        else{
            cout << WARNING_TEXT << ": Unknown command" << endl;
            return nullptr;
        }
    case selector_c::type_e::EXPORT:
        if(format == selector_c::format_e::RA)
            return make_unique<cmd_ra_export_c>(cmd_text);
        else{
            cout << WARNING_TEXT << ": Unknown command" << endl;
            return nullptr;
        }
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
        cout << WARNING_TEXT << ": Unknown command" << endl;
        return nullptr;
    }
}
