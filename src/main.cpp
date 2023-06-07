#include "config.h"
#include "app.h"


int main(int argc, char* argv[]){
    
    unique_ptr<app_c> app = app_factory_c::create_app(argc, argv);
    app->launch();

    return 0;
}