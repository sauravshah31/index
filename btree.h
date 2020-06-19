#pragma once
#include <vector>

struct Column{
    std::vector<std::string> data;
    int key_ind;
    const bool operator> (const Column &b);
    const bool operator< (const Column &b);
    const bool operator>= (const Column &b);
    const bool operator<= (const Column &b);
    const bool operator== (const Column &b);
    const bool operator!= (const Column &b);
    Column();
};
typedef struct Column Cloumn;
struct ColumnPtr{
    Column* colPtr;
    ColumnPtr();
    ColumnPtr(Column* ptr);
    const bool operator> (const ColumnPtr &b);
    const bool operator< (const ColumnPtr &b);
    const bool operator>= (const ColumnPtr &b);
    const bool operator<= (const ColumnPtr &b);
    const bool operator== (const ColumnPtr &b);
    const bool operator!= (const ColumnPtr &b);
};
typedef struct ColumnPtr ColumnPtr;
struct bdnode{
    int cnt; 
    ColumnPtr* key;/*2d pointer to data*/
    struct bdnode **ptr;
    bdnode();
};
typedef struct bdnode bdnode;
typedef struct bdnode * BDPTR;



BDPTR create(ColumnPtr key);
BDPTR split(BDPTR &original,BDPTR &extra,int i);
void insertAndSort(BDPTR &T,ColumnPtr key,int i);
void insertAndSort(BDPTR &T,BDPTR &keyNode,int i);
BDPTR Add(BDPTR T,ColumnPtr key);
void shift(BDPTR &T,int i);
void combine(BDPTR &T,int i);
BDPTR remove(BDPTR &T,ColumnPtr key);
void printLevelOrder(BDPTR T);
void printInOrder(BDPTR T);
void printCol(ColumnPtr col);
void printColPtr(Column* col);  

extern int _d_btree;
