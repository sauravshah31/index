#include "insert.h"
#include "colors.h"
#include "index.h"
#include "utils.h"
#include <vector>

std::string getTableName(const char* sql_cmd){
    std::vector<std::string> all_cmd = split_cmd(sql_cmd);
    /*insert into tablename...*/
    std::string table = all_cmd[2];
    int i=0;
    while(table[i]!='\0'&&table[i]!=' '&&table[i]!='('){
        i++;
    }
    return table.substr(0,i);
}

int insert(sqlite3* opened_DB,const char* sql_cmd){
    char* msgErr = NULL;

    int exit_stat = 0;
    int *cnt = new int;
    *cnt = 1;
    exit_stat = sqlite3_exec(opened_DB,sql_cmd,NULL,cnt,&msgErr);
    std::cout<<KBOLD;
    if(exit_stat != SQLITE_OK){
        std::cerr<<KRED;
        std::cerr<<"Error: "<<msgErr<<std::endl;
        sqlite3_free(msgErr);
        std::cerr<<RST;
        std::cerr<<RST;
        return -1;
    }else{
        /*now insert into indexed BTree*/
        std::string table = getTableName(sql_cmd);
        insert_into_all_indexed(opened_DB,table);
        std::cout<<KGRN;
        std::cout<<"...Success..."<<std::endl;
        std::cout<<RST;
        std::cerr<<RST;
    }
    return 0;
}