#include "app.h"
#include "cmd.h"


void app_c::print_start(){
    cout << NOTE_TEXT << ": Program started!" << endl;
    cout << NOTE_TEXT << ": Write " << BOLD_FONT << "HELP" << RESET_FONT << " to see possible commands" << endl;
}

void app_c::print_finish(){
    cout << NOTE_TEXT << ": Program finished!" << endl;
}

void app_c::create_folders(){

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

void app_c::launch(){
    
    // initial program
    create_folders();
    print_start();

    // main cycle
    string cmd_text;
    while(manager.read_cmd(cmd_text)){

        // create command
        unique_ptr<cmd_c> cmd = manager.set_cmd(cmd_text);
        if(cmd == nullptr){ cout << WARNING_TEXT << ": unknown command" << endl; continue; }

        cmd->run(manager.get_tables());
    }


    // end program
    print_finish();
}

