#include <iostream>
#include <sqlite3.h>

#include "insert.h"
#include "create.h"
#include "select.h"

#include "utils.h"
#include "colors.h"
#include "index.h"

static char* db_dir = new char[100] ;

int main(){
    /*
        open database
    */
    std::cout<<CLEAR;
    std::cout<<BOLD(FMAG("------SQL INDEXING USING BTREE------\n"));
    sqlite3* DB;
    char *msgErr = NULL;
    int exit_stat=0;
    int stat=0;
    std::string sql_cmd;
    int cmd_id = -1;

    std::cout<<FCYN(BOLD("Enter database directory"))<<std::endl;
    std::cout<<" : ";
    std::cout<<KYEL;
    std::cin>>db_dir;
    std::cout<<RST;

    std::cout<<BOLD(FBLU("...Connecting to Database..."))<<std::endl;
    exit_stat = sqlite3_open(db_dir,&DB);

    if(exit_stat){
        std::cout<<KRED;
        std::cerr<<"Error:"<<std::endl;
        std::cerr<<sqlite3_errmsg(DB);
        std::cout<<RST;
        return 0;
    }
    std::cout<<BOLD(FGRN("...Success..."));
    std::cout<<std::endl;
    std::cout<<std::endl;

    std::cout<<FCYN(BOLD("...INPUT QUERIES..."))<<std::endl;
    while(true){
        sql_cmd = input();
        if(sql_cmd[0]==';')
            break;
        
        cmd_id = get_cmd_id(sql_cmd);
        if(cmd_id == 0){
            std::cout<<BOLD(FBLU("...creating..."))<<std::endl;
            stat = create(DB,sql_cmd.c_str());
        }else if(cmd_id == 1){
            std::cout<<BOLD(FBLU("...inserting..."))<<std::endl;
            stat = insert(DB,sql_cmd.c_str());
        }else if(cmd_id==2){
            std::cout<<BOLD(FBLU("...selecting..."))<<std::endl;
            stat = select(DB,sql_cmd.c_str());
        }else if(cmd_id==3){
            std::cout<<BOLD(FBLU("...creating index..."))<<std::endl;
            std::pair<std::string,BDPTR> ROOT= index(DB,sql_cmd.c_str());
            if(ROOT.second==NULL)
                stat=-1;
            else
                stat=0;
            
        }else if(cmd_id==4){
            stat=view_index_structure(sql_cmd);
        }else if(cmd_id==5){
            stat=view_sorted(sql_cmd);
        }else{
            std::cout<<BOLD(FRED("Error:\nsuch command doesn't exits \n"));
        }
        if(stat==-1){
            std::cout<<BOLD(FRED("TRY AGAIN"))<<std::endl;
        }
        
    }

    std::cout<<BOLD(FBLU("...Closing Database ..."))<<std::endl;
    sqlite3_close(DB);
    std::cout<<BOLD(FGRN("...Done..."))<<std::endl;
    
    std::cout<<BOLD(FMAG("------------------------------\n"));
    std::cout<<BOLD(FMAG("BY:\n"));
    std::cout<<BOLD(FMAG("Saurav Shah\n"));
    std::cout<<BOLD(FMAG("Roll no: 187157\n"));
    std::cout<<BOLD(FMAG("Reg no: 941874\n"));
    std::cout<<BOLD(FMAG("CSE II A\n"));
    /* 
        display options  
    */
    /*
        listen to commands
        execute command
        check condition for exit
    */  

}