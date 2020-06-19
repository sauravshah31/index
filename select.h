#pragma once
#include<sqlite3.h>
#include "callback.h"

int select(sqlite3* opened_DB,const char* sql_cmd);