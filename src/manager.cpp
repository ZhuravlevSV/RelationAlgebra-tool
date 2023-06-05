#include "manager.h"

selector_c::format_e selector_c::define_format(string& cmd_text){
    regex ra_regex("\\s*(\\w+)");
    regex sql_regex("\\s*TO\\s*SQL\\s*(\\w+)");
    match_results<string::const_iterator> match;

    // compare formats
    if(regex_search(cmd_text, match, sql_regex) && match.size() == 2){
        cmd_text = match[1];
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
    selector_c::format_e format = selector.define_format(cmd_text);
    selector_c::type_e type = selector.define_type(cmd_text);

    /*
    cout << "\t" << SUCCESS_TEXT << ": " << cmd_text << endl;
    switch (format){
    case selector_c::format_e::RA: cout << "\t" << NOTE_TEXT << ": FORMAT: RA" << endl; break;
    case selector_c::format_e::SQL: cout << "\t" << NOTE_TEXT << ": FORMAT: SQL" << endl; break;
    case selector_c::format_e::UNDEFINED: cout << "\t" << ERROR_TEXT << ": FORMAT: UNDEFINED" << endl; break;
    default: break;
    }
    switch (type){
    case selector_c::type_e::IMPORT: cout << "\t" << NOTE_TEXT << ": TYPE: IMPORT" << endl; break;
    case selector_c::type_e::EXPORT: cout << "\t" << NOTE_TEXT << ": TYPE: EXPORT" << endl; break;
    case selector_c::type_e::HELP: cout << "\t" << NOTE_TEXT << ": TYPE: HELP" << endl; break;
    case selector_c::type_e::PRINT: cout << "\t" << NOTE_TEXT << ": TYPE: PRINT" << endl; break;
    case selector_c::type_e::RENAME: cout << "\t" << NOTE_TEXT << ": TYPE: RENAME" << endl; break;
    case selector_c::type_e::PROJECTION: cout << "\t" << NOTE_TEXT << ": TYPE: PROJECTION" << endl; break;
    case selector_c::type_e::SELECTION: cout << "\t" << NOTE_TEXT << ": TYPE: SELECTION" << endl; break;
    case selector_c::type_e::NATURAL_JOIN: cout << "\t" << NOTE_TEXT << ": TYPE: NATURAL JOIN" << endl; break;
    case selector_c::type_e::JOIN: cout << "\t" << NOTE_TEXT << ": TYPE: JOIN" << endl; break;
    case selector_c::type_e::MULTITASK: cout << "\t" << NOTE_TEXT << ": TYPE: MULTITASK" << endl; break;
    case selector_c::type_e::UNDEFINED: cout << "\t" << ERROR_TEXT << ": TYPE: UNDEFINED" << endl; break;
    default: break;
    }
    */

    return cmd_factory::create_cmd(format, type, cmd_text);
}

tables_c& manager_c::get_tables(){
    return tables;
}