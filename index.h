#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sqlite3.h>
#include "btree.h"
#include <unordered_map>


extern int COL_TO_BE_INDEX_ON;
extern std::string COL_NAME_TO_BE_INDEXED;
extern std::unordered_map<std::string,BDPTR> ALL_INDEX_ROOT;



int getColIndex(int argc,char** azColName);
int index_callback(void *IR,int argc,char** argv,char** azColName);
std::pair<std::string,BDPTR> index(sqlite3* opened_DB,const char* sql_cmd);
int view_index_structure(std::string sql_cmd);
int view_sorted(std::string sql_cmd);
void insert_into_all_indexed(sqlite3* opened_DB,std::string table);