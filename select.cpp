#include "select.h"
#include "colors.h"

int select(sqlite3* opened_DB,const char* sql_cmd){
    char* msgErr = NULL;

    int exit_stat = 0;
    int* cnt=new int;
    *cnt = 1;

    exit_stat = sqlite3_exec(opened_DB,sql_cmd,callback,(void*)cnt,&msgErr);

    std::cout<<KBOLD;
    if(exit_stat != SQLITE_OK){
        std::cerr<<KRED;
        std::cerr<<"Error: "<<msgErr<<std::endl;
        sqlite3_free(msgErr);
        std::cerr<<RST;
        std::cerr<<RST;

        return -1;
    }else{
        std::cout<<KGRN;
        std::cout<<"...Success..."<<std::endl;
        std::cout<<RST;
        std::cerr<<RST;
    }
    return 0;
}