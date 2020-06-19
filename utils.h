#pragma once
#include <iostream>
#include <string>
#include <vector>




void print_tab_space(   /*prints tab space*/
    int n               /*no of tab space to be printed*/
);

std::string input();

int get_cmd_id(const std::string sql_cmd);

std::vector<std::string> split_cmd(const char* sql_cmd);

extern std::vector<std::string> all_sql_query;
std::string toLower(const std::string str);
std::string toLower(const char* str);