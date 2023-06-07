#include "config.h"
#include "app.h"
#include "app_tester.h"


int main(int argc, char* argv[]){
    
    // auto test
    if(argc == 2){
        string argument = argv[1];
        if(argument == "test"){
            app_tester_c app_tester;
            app_tester.launch();
        }
    }
    
    // application
    app_c app;
    app.launch();

    return 0;
}