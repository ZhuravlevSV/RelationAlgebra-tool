#include "config.h"
#include "app.h"


int main(int argc, char* argv[]){
    
    // Create and define an application
    unique_ptr<app_c> app = app_factory_c::create_app(argc, argv);

    // Launch current application
    app->launch();

    return 0;
}