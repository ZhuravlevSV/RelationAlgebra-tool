#include "app_tester.h"
#include "config.h"


void app_tester_c::print_start(){
    cout << NOTE_TEXT << ": Application tester started!" << endl;
}

void app_tester_c::print_finish(){
    cout << SUCCESS_TEXT << ": Application tester finished!" << endl;
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
    cmd_ok_work_ok("IMPORT fruits \"fruits.csv\"");
    cmd_ok_work_ok("     IMPORT       exporters      \"exporters.csv\"      ");
    cmd_ok_work_er("IMPORT err \"something.csv\"");
    cmd_ok_work_er("IMPORT err \"error1.csv\"");
    cmd_ok_work_er("IMPORT err \"error2.csv\"");
    cmd_ok_work_er("IMPORT err \"error3.csv\"");
    cmd_ok_work_er("IMPORT err \"error4.csv\"");
    cmd_ok_work_er("IMPORT err \"error5.csv\"");
    cmd_ok_work_er("IMPORT err \"error6.csv\"");
    cmd_ok_work_er("IMPORT err \"error7.csv\"");
    cmd_ok_work_er("IMPORT");
    cmd_ok_work_er("IMPORT err");
    cmd_ok_work_er("IMPORT err \"fruits.gif\"");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "IMPORT" << RESET_FONT << " working correctly!" << endl;
    
    // Print tester
    cmd_ok_work_ok("PRINT fruits");
    cmd_ok_work_ok("     PRINT    exporters        ");
    cmd_ok_work_er("PRINT");
    cmd_ok_work_er("PRINT   ");
    cmd_ok_work_er("PRINT   fruits exporters");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "PRINT" << RESET_FONT << " working correctly!" << endl;

    // Export tester
    cmd_ok_work_ok("EXPORT fruits \"test.csv\"");
    cmd_ok_work_ok("EXPORT fruits \"test.xml\"");
    cmd_ok_work_ok("EXPORT fruits \"test.json\"");
    cmd_ok_work_ok("EXPORT fruits \"test.csv\"");
    cmd_ok_work_ok("EXPORT fruits \"test.xml\"");
    cmd_ok_work_er("EXPORT");
    cmd_ok_work_er("EXPORT fruits");
    cmd_ok_work_er("EXPORT fruits \"\"");
    cmd_ok_work_er("EXPORT fruits \"test_err.jpg\"");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "EXPORT" << RESET_FONT << " working correctly!" << endl;

    // Rename tester
    cmd_ok_work_ok("RENAME fruits color AS barva");
    cmd_ok_work_ok("    RENAME      fruits      price       AS          cena        ");
    cmd_ok_work_ok("PRINT fruits");
    cmd_ok_work_er("RENAME");
    cmd_ok_work_er("RENAME fruits barva AS");
    cmd_ok_work_er("RENAME fruits AS");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "RENAME" << RESET_FONT << " working correctly!" << endl;

    // Projection tester
    cmd_ok_work_ok("PROJECTION fruits_2 OF fruits [barva,fruit,id,cena]");
    cmd_ok_work_ok("    PROJECTION      fruits_3    OF      fruits      [fruit,cena,barva]  ");
    cmd_ok_work_er("PROJECTION");
    cmd_ok_work_er("PROJECTION fruits_err OF ");
    cmd_ok_work_er("PROJECTION OF ");
    cmd_ok_work_ok("PRINT fruits_2");
    cmd_ok_work_ok("PRINT fruits_3");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "PROJECTION" << RESET_FONT << " working correctly!" << endl;

    // Select tester
    cmd_ok_work_ok("SELECT fruits_yellow OF fruits_2 (barva = Yellow)");
    cmd_ok_work_ok("    SELECT      fruits_yellow_more_than_1_dollar    OF      fruits_yellow   (cena > 1)   ");
    cmd_ok_work_er("SELECT OF ( > )");
    cmd_ok_work_ok("PRINT fruits_yellow_more_than_1_dollar");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "PROJECTION" << RESET_FONT << " working correctly!" << endl;

    // Natural join tester
    cmd_ok_work_ok("NATURAL JOIN fruits_yellow_exporters_na_join FROM fruits_yellow_more_than_1_dollar, exporters");
    cmd_ok_work_ok("    NATURAL     JOIN    fruits_yellow_exporters_na_join_2   FROM    fruits_yellow_more_than_1_dollar    ,   exporters   ");
    cmd_ok_work_er("NATURAL JOIN FROM , ");
    cmd_ok_work_ok("PRINT fruits_yellow_exporters_na_join");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "NATURAL JOIN" << RESET_FONT << " working correctly!" << endl;

    // Join tester
    cmd_ok_work_ok("JOIN fruits_yellow_exporters_join FROM fruits_yellow_more_than_1_dollar, exporters ON id = id_fruit");
    cmd_ok_work_ok("    JOIN    fruits_yellow_exporters_join_2    FROM    fruits_yellow_more_than_1_dollar  ,   exporters   ON      id      =   id_fruit    ");
    cmd_ok_work_er("JOIN FROM  ,  ON  =  ");
    cmd_ok_work_ok("PRINT fruits_yellow_exporters_join");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "JOIN" << RESET_FONT << " working correctly!" << endl;

    // Multitask tester
    cmd_ok_work_ok("IMPORT people1 \"people1.csv\"");
    cmd_ok_work_ok("IMPORT people2 \"people2.csv\"");
    cmd_ok_work_ok("PRINT people1");
    cmd_ok_work_ok("PRINT people2");
    cmd_ok_work_ok("MULTITASK people_cr FROM (people1 CROSS people2)");
    cmd_ok_work_ok("    MULTITASK   people_in   FROM    people1     INTERSECT   people2     ");
    cmd_ok_work_ok("    MULTITASK   people_ex   FROM (  people1     EXCEPT      people2    )");
    cmd_ok_work_ok("MULTITASK people_un FROM people1 UNION people2");
    cmd_ok_work_er("MULTITASK people_cr_er FROM");
    cmd_ok_work_er("MULTITASK people_cr_er FROM     ");
    cmd_ok_work_er("MULTITASK people_cross_er FROM ( people1 CROSS people2");
    cmd_ok_work_er("MULTITASK people_cross_er FROM people1 CROSS people2 )");
    cmd_ok_work_er("MULTITASK people_cross_er FROM people1 CROSS");
    cmd_ok_work_er("MULTITASK people_cross_er FROM CROSS ");
    cmd_ok_work_er("MULTITASK people_cross_er FROM CROSS people2");
    cmd_ok_work_er("MULTITASK people_cross_er FROM ( ( people1 CROSS people2 )");
    cmd_ok_work_er("MULTITASK people_cross_er FROM ( people1 CROSS people2 ) )");
    cmd_ok_work_er("MULTITASK people_cross_er FROM ( ( people1 CROSS people2 ) )");
    cmd_ok_work_ok("PRINT people_cr");
    cmd_ok_work_ok("PRINT people_in");
    cmd_ok_work_ok("PRINT people_ex");
    cmd_ok_work_ok("PRINT people_un");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "MULTITASK" << RESET_FONT << " working correctly!" << endl;

    // SQL tester
    cmd_ok_work_ok("TO SQL PRINT table");
    cmd_ok_work_ok("    TO      SQL     PRINT   table   ");
    cmd_ok_work_ok("TO SQL RENAME table old_att AS new_att");
    cmd_ok_work_ok("    TO      SQL     RENAME      table   old_att     AS      new_att     ");
    cmd_ok_work_ok("TO SQL PROJECTION new_table OF table_source [att1, att2, att3, att4]");
    cmd_ok_work_ok("    TO      SQL     PROJECTION      new_table   OF      table_source    [att1, att2, att3, att4]    ");
    cmd_ok_work_ok("TO SQL SELECT new_table OF table_source (att = Yellow)");
    cmd_ok_work_ok("    TO      SQL     SELECT      new_table   OF    table_source    (att = 100)");
    cmd_ok_work_ok("TO SQL SELECT new_table OF table_source (att < 250)");
    cmd_ok_work_ok("TO SQL SELECT new_table OF table_source (att > 500)");
    cmd_ok_work_ok("TO SQL NATURAL JOIN new_table FROM table_source_1, table_source_2");
    cmd_ok_work_ok("    TO      SQL     NATURAL     JOIN    new_table   FROM    table_source_1  ,   table_source_2  ");
    cmd_ok_work_ok("TO SQL JOIN new_table FROM table_source_1, table_source_2 ON att_1 = att_2");
    cmd_ok_work_ok("    TO      SQL     JOIN    new_table   FROM    table_source_1  ,   table_source_2      ON      att_1   =   att_2   ");
    cmd_ok_work_ok("TO SQL MULTITASK new_table FROM (source_table_1 CROSS source_table_2)");
    cmd_ok_work_ok("    TO      SQL     MULTITASK   new_table   FROM       source_table_1      INTERSECT   source_table_2     ");
    cmd_ok_work_ok("TO SQL MULTITASK new_table FROM (source_table_1 EXCEPT source_table_2)");
    cmd_ok_work_ok("TO SQL MULTITASK new_table FROM source_table_1 UNION source_table_2");
    cmd_ok_work_ok("TO SQL MULTITASK new_table FROM ((source_table_1 UNION source_table_2 EXCEPT source_table_3) INTERSECT (source_table_4 CROSS source_table_5)) EXCEPT source_table_6 EXCEPT source_table_7");    
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM    ");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM ( source_table_1 CROSS source_table_2");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM source_table_1 CROSS source_table_2 )");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM source_table_1 CROSS");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM CROSS source_table_2");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM CROSS");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM ( ( source_table_1 CROSS source_table_2 )");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM ( source_table_1 CROSS source_table_2 ) )");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM ( ( source_table_1 CROSS source_table_2 		) )");
    cmd_ok_work_er("TO SQL MULTITASK new_table FROM ( ( ) )");
    cout << SUCCESS_TEXT << ": " << UNDERLINE_FONT << "SQL" << RESET_FONT << " working correctly!" << endl;
    cout << endl;
    cout << SUCCESS_TEXT << ": all tests passed!" << endl;


    // end program
    print_finish();
}