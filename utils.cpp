#include "utils.h"
#include "colors.h"


std::vector<std::string> all_sql_query = {"create","insert","select","index","view_index","sort"};

void print_tab_space(   /*prints tab space*/
    int n               /*no of tab space to be printed*/
){
    int i;
    for(i=0;i<n;i++){
        std::cout<<"\t";
    }
}

std::string input(){      /*returns sql query*/
    std::string res="";
    std::string inp;
    std::cout<<KYEL;
    while(true){
        getline(std::cin,inp);
        res+=inp;
        if(inp[0]==';'||inp[inp.size()-1]==';'){
            break;
        }
    }
    std::cout<<RST;
    return res;
}

int get_cmd_id(const std::string sql_cmd){
    std::string first_cmd = "";
    int i=0;
    while(sql_cmd[i]!='\0'&&sql_cmd[i]==' ')
        i++;
    while(sql_cmd[i]!='\0'&&sql_cmd[i]!=' '){
        first_cmd += sql_cmd[i];
        i++;
    }
    first_cmd = toLower(first_cmd);
    for(int i=0;i<all_sql_query.size();i++){
        if(first_cmd == all_sql_query[i])
            return i;
    }

    return -1;
}


std::vector<std::string> split_cmd(const char* sql_cmd){
    std::vector<std::string> cmd_lst;
    std::string curr_cmd="";
    int i=0;

    while(sql_cmd[i]!='\0'&&sql_cmd[i]!=';'){
        if(sql_cmd[i]==' '){
            if(curr_cmd!="" && curr_cmd[0]!=' '){
                cmd_lst.push_back(curr_cmd);
            }
            curr_cmd="";
        }else{
                curr_cmd+=sql_cmd[i];
        }
        i++;
    }
    
    if(curr_cmd!="" && curr_cmd[0]!=' '){
        cmd_lst.push_back(curr_cmd);
    }
    return cmd_lst;
}

std::string toLower(const std::string str){
    int i=0;
    std::string res="";
    while(str[i]!='\0'){
        if(str[i]>='A'&&str[i]<='Z'){
            res += (char)(str[i]-('A'-'a'));
        }else{
            res += str[i];
        }
        i++;
    }
    return res;
}
std::string toLower(const char* str){
    int i=0;
    std::string res="";
    while(str[i]!='\0'){
        if(str[i]>='A'&&str[i]<='Z'){
            res += (char)(str[i]-('A'-'a'));
        }else{
            res += str[i];
        }
        i++;
    }
    return res;
}