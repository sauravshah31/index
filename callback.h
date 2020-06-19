#pragma once
#include <sqlite3.h>
#include <iostream>

int callback(
    void* cnt,      /*This argument is not used*/
    int argc,           /*No of results*/
    char **argv,        /*Values in array of char**/
    char **azColName    /*column name of each value*/
);
