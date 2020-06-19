#pragma once
#include <iostream>
#include <sqlite3.h>

int insert(sqlite3* opened_DB,const char* sql_cmd);
std::string getTableName(const char* sql_cmd);
