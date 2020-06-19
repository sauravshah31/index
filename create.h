#pragma once
#include <iostream>
#include <string>
#include <sqlite3.h>


int create(sqlite3* opened_DB,const char* sql_cmd);