#include "config.h"
#include "app.h"
#include "app_tester.h"


int main(void){
    
    {
    app_tester_c app_tester;
    app_tester.launch();
    }

    app_c app;
    app.launch();

    return 0;
}