#include "app_tester.h"
#include "config.h"


void app_tester_c::print_start(){
    cout << NOTE_TEXT << ": Application tester started!" << endl;
}

void app_tester_c::print_finish(){
    cout << SUCCESS_TEXT << ": Application tester finished!" << endl;
    cout << GREEN_COLOR << "========================================================================================================" << RESET_FONT << endl;
    cout << endl;
}

void app_tester_c::create_folders(){

    if(!filesystem::is_directory(EXAMPLES_FOLDER)){
        filesystem::create_directory(EXAMPLES_FOLDER);
        cout << NOTE_TEXT << ": Creating a " << UNDERLINE_FONT << EXAMPLES_FOLDER << RESET_FONT << " folder..." << endl;
    }
    if(!filesystem::is_directory(BUFFER_FOLDER)){
        filesystem::create_directory(BUFFER_FOLDER);
        cout << NOTE_TEXT << ": Creating a " << UNDERLINE_FONT << BUFFER_FOLDER << RESET_FONT << " folder..." << endl;
    }
    if(!filesystem::is_directory(WORKSPACE_FOLDER)){
        filesystem::create_directory(WORKSPACE_FOLDER);
        cout << NOTE_TEXT << ": Creating a " << UNDERLINE_FONT << WORKSPACE_FOLDER << RESET_FONT << " folder..." << endl;
        cout << NOTE_TEXT << ": Please add to the folder " << UNDERLINE_FONT << WORKSPACE_FOLDER << RESET_FONT << " tables in CSV format" << endl;
    }
    if(!filesystem::is_directory(SAVED_FILES_FOLDER)){
        filesystem::create_directory(SAVED_FILES_FOLDER);
        cout << NOTE_TEXT << ": Creating a " << UNDERLINE_FONT << SAVED_FILES_FOLDER << RESET_FONT << " folder..." << endl;
    }
}

void app_tester_c::cmd_ok_work_ok(string cmd_text){
    unique_ptr<cmd_c> cmd = manager.set_cmd(cmd_text);    
    assert(cmd != nullptr);
    assert(cmd->run(manager.get_tables()));
}

void app_tester_c::cmd_er_work_er(string cmd_text){
    unique_ptr<cmd_c> cmd = manager.set_cmd(cmd_text);    
    assert(cmd == nullptr);
}

void app_tester_c::cmd_ok_work_er(string cmd_text){
    unique_ptr<cmd_c> cmd = manager.set_cmd(cmd_text);    
    assert(cmd != nullptr);
    assert(!cmd->run(manager.get_tables()));
}

void app_tester_c::launch(){
    
    // initial program
    create_folders();
    print_start();

    // main cycle

    // Help tester
    cmd_ok_work_ok("HELP");
    cmd_ok_work_ok("        HELP        ");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "HELP" << RESET_FONT << " working correctly!" << endl;

    // Import tester
    cmd_ok_work_er("IMPORT err \"something.csv\"");
    cmd_ok_work_er("IMPORT err \"error1.csv\"");
    cmd_ok_work_er("IMPORT err \"error2.csv\"");
    cmd_ok_work_er("IMPORT err \"error3.csv\"");
    cmd_ok_work_er("IMPORT err \"error4.csv\"");
    cmd_ok_work_er("IMPORT err \"error5.csv\"");
    cmd_ok_work_er("IMPORT err \"error6.csv\"");
    cmd_ok_work_er("IMPORT err \"error7.csv\"");
    cmd_ok_work_ok("IMPORT fruits \"fruits.csv\"");
    cmd_ok_work_ok("     IMPORT       exporters      \"exporters.csv\"      ");
    


    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "IMPORT" << RESET_FONT << " working correctly!" << endl;
    
    /*
    IMPORT err "something.csv"
IMPORT err "error1.csv"
IMPORT err "error2.csv"
IMPORT err "error3.csv"
IMPORT err "error4.csv"
IMPORT err "error5.csv"
IMPORT err "error6.csv"
IMPORT err "error7.csv"

IMPORT fruits "fruits.csv"
	  IMPORT 	exporters 	"exporters.csv"	 	
    */

    // end program
    print_finish();
}