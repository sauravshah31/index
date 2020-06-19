
#include <cassert>
#include <iostream>
#include "index.h"
#include "colors.h"
#include "utils.h"


/*
    create a structure that stores the data
    2d array like structure
*/ 

int COL_TO_BE_INDEX_ON=-1;
std::string COL_NAME_TO_BE_INDEXED="";

std::unordered_map<std::string,BDPTR> ALL_INDEX_ROOT;

int getColIndex(int argc,char** azColName){
    for(int i=0;i<argc;i++){
        if(toLower(azColName[i])==toLower(COL_NAME_TO_BE_INDEXED)){
            return i;
        }
        //std::cout<<(std::string)azColName[i]<<":"<<((std::string)azColName[i]==COL_NAME_TO_BE_INDEXED)<<" ";
    }
    return -1;
}

int index_callback(void *IR,int argc,char** argv,char** azColName){
    BDPTR & INDEX_ROOT = *((BDPTR *) IR);
    Column* col = new Column;
    for(int i=0;i<argc;i++){
        col->data.push_back((std::string)argv[i]);
    }
    if(INDEX_ROOT == NULL){
        /*figure out COL_TO_BE_INDEX_ON*/
        COL_TO_BE_INDEX_ON = getColIndex(argc,azColName);
    }

    assert(COL_TO_BE_INDEX_ON!=-1);
    col->key_ind = COL_TO_BE_INDEX_ON;
    
    INDEX_ROOT = Add(INDEX_ROOT,col);
    return 0;   
}

/* return column_name and its's indexed root */
std::pair<std::string,BDPTR> index(sqlite3* opened_DB,const char* sql_cmd){
    std::vector<std::string> cmd_lst;
    char* msgErr = NULL;

    int exit_stat = 0;
    _d_btree = 2;
    BDPTR INDEX_ROOT=NULL;


    /*index on col_name from table_name*/
    cmd_lst=split_cmd(sql_cmd);

    /*
        col_name -> cmd_lst[2]
        table_name -> cmd_lst[4]
    */

    /*check if already indexed*/
    if(ALL_INDEX_ROOT.find(toLower((std::string )cmd_lst[2]))!=ALL_INDEX_ROOT.end()){
        std::cout<<BOLD(FGRN("...INDEX ALREADY PRESENT..."))<<std::endl;
        return {(std::string) cmd_lst[2],ALL_INDEX_ROOT[toLower((std::string) cmd_lst[2])]};
    }

    if(cmd_lst.size()!=5){
        std::cout<<KBOLD;
        std::cout<<KRED;
        std::cout<<"Error:\n";
        std::cout<<"Invalid syntax";
        std::cout<<std::endl;
        std::cout<<RST;
        std::cout<<RST;
        return {"",NULL};
    }else if(cmd_lst[2]=="*"){
        std::cout<<KBOLD;
        std::cout<<KRED;
        std::cout<<"Error:\n";
        std::cout<<"Index can only be created on one column";
        std::cout<<std::endl;
        std::cout<<RST;
        std::cout<<RST;
        return {"",NULL};
    }
    std::string sql="select * from "+cmd_lst[4]+";";
    COL_NAME_TO_BE_INDEXED = toLower((std::string)cmd_lst[2]);
    exit_stat = sqlite3_exec(opened_DB,sql.c_str(),index_callback,&INDEX_ROOT,&msgErr);


    std::cout<<KBOLD;
    if(exit_stat != SQLITE_OK || INDEX_ROOT==NULL){
        std::cerr<<KRED;
        std::cerr<<"Error: "<<msgErr<<std::endl;
        sqlite3_free(msgErr);
        std::cerr<<RST;
        std::cerr<<RST;

        return {"",NULL};
    }else{
        std::cout<<KGRN;
        std::cout<<"...Success..."<<std::endl;
        std::cout<<RST;
        std::cerr<<RST;
    }
    ALL_INDEX_ROOT[toLower(cmd_lst[2])] = INDEX_ROOT;
    return {cmd_lst[2],INDEX_ROOT};
}


int view_index_structure(std::string sql_cmd){
    /*view_index on col_name*/
    std::vector<std::string> cmd_lst = split_cmd(sql_cmd.c_str());
    BDPTR ROOT = NULL;
    if(cmd_lst.size()!=3)
    {
        std::cout<<BOLD(FRED("INVALID SYNTAX\n"));
        return -1;
    }
    if(ALL_INDEX_ROOT.find(toLower(cmd_lst[2]))==ALL_INDEX_ROOT.end()){
        std::cout<<BOLD(FRED("INDEX HAS NOT BEEN CREATED\n"));
        return -1;
    }

    ROOT = ALL_INDEX_ROOT[toLower(cmd_lst[2])];

    //std::cout<<CLEAR;
    std::cout<<BOLD(FBLU(".....INDEXED B-TREE.....\n"));
    for(int i=0;i<100;i++)
        std::cout<<BOLD(FBLU("="));
    std::cout<<std::endl;
    std::cout<<KBOLD;
    printLevelOrder(ROOT);
    for(int i=0;i<100;i++)
        std::cout<<BOLD(FBLU("="));
    std::cout<<RST;
    std::cout<<std::endl;
    return 0;
} 

int view_sorted(std::string sql_cmd){
    /*sort on col_name*/
    std::vector<std::string> cmd_lst = split_cmd(sql_cmd.c_str());
    BDPTR ROOT = NULL;
    if(cmd_lst.size()!=3)
    {
        std::cout<<BOLD(FRED("INVALID SYNTAX\n"));
        return -1;
    }
    if(ALL_INDEX_ROOT.find(toLower(cmd_lst[2]))==ALL_INDEX_ROOT.end()){
        std::cout<<BOLD(FRED("INDEX HAS NOT BEEN CREATED\n"));
        return -1;
    }

    ROOT = ALL_INDEX_ROOT[toLower(cmd_lst[2])];

    //std::cout<<CLEAR;
    std::cout<<BOLD(FBLU(".....SORTED VALUE.....\n"));
    for(int i=0;i<100;i++)
        std::cout<<BOLD(FBLU("="));
    std::cout<<std::endl;
    std::cout<<KBOLD;
    printInOrder(ROOT);
    for(int i=0;i<100;i++)
        std::cout<<BOLD(FBLU("="));
    std::cout<<RST;
    std::cout<<std::endl;
    return 0;
} 

int insert_into_all_indexed_callback(void *NotUsed,int argc,char** argv,char** azColName){
    for(auto &key: ALL_INDEX_ROOT){
        Column* col = new Column;
        for(int i=0;i<argc;i++){
            col->data.push_back((std::string)argv[i]);
        }
        assert(COL_TO_BE_INDEX_ON!=-1);
        col->key_ind = COL_TO_BE_INDEX_ON;
        key.second = Add(key.second,col);
    }
    return 0;
}

void insert_into_all_indexed(sqlite3* opened_DB,std::string table){
    int rowid = 0;
    char* msgErr;
    rowid=sqlite3_last_insert_rowid(opened_DB);
    std::string sql="";
    sql = "select * from "+ table+" where rowid = "+std::to_string(rowid)+";";
    sqlite3_exec(opened_DB,sql.c_str(),insert_into_all_indexed_callback,NULL,&msgErr);

}