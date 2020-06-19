#include "callback.h"
#include <sqlite3.h>
#include "utils.h"
#include "colors.h"

int callback(           /*callback function to be passed in*/
    void* cnt,            /*This argument is not used*/
    int argc,           /*No of results*/
    char **argv,        /*Values in array of char**/
    char **azColName    /*column name of each value*/
){
    if(*((int*)cnt)==1){
        std::cout<<KBOLD;
        std::cout<<KWHT;
        print_tab_space(1);
        for(int i=0;i<argc;i++){
            std::cout<<azColName[i];
            print_tab_space(1+(i==2));
        }
        std::cout<<std::endl;
        print_tab_space(1);
        for(int i=0;i<std::min(100,argc*10);i++){
            std::cout<<"=";
        }
        std::cout<<std::endl;
        std::cout<<RST;
        std::cout<<RST;
    }
    *((int*)cnt)+=1;
    int i;

    std::cout<<KBOLD;
    std::cout<<KWHT;
    print_tab_space(1);
    for(i=0;i<argc;i++){
        std::cout<<argv[i];
        print_tab_space(1);
    }
    std::cout<<std::endl;
    std::cout<<RST;
    std::cout<<RST;
    return 0;
}
