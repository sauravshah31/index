#include<iostream>
#include<assert.h>
#include<queue>
#include "btree.h"
#include "colors.h"
#include "utils.h"
#include "index.h"

int _d_btree=0;
Column::Column(){
}
const bool Column::operator>(const Column &b){
    return this->data[this->key_ind]>b.data[b.key_ind];
}
const bool Column::operator>=(const Column &b){
    return this->data[this->key_ind]>=b.data[b.key_ind];
}
const bool Column::operator<(const Column &b){
    return this->data[this->key_ind]<b.data[b.key_ind];
}
const bool Column::operator<=(const Column &b){
    return this->data[this->key_ind]<=b.data[b.key_ind];
}
const bool Column::operator==(const Column &b){
    return this->data[this->key_ind]==b.data[b.key_ind];
}
const bool Column::operator!=(const Column &b){
    return this->data[this->key_ind]!=b.data[b.key_ind];
}

ColumnPtr::ColumnPtr(){}
ColumnPtr::ColumnPtr(Column* ptr){
    this->colPtr = ptr;
}
const bool ColumnPtr::operator>(const ColumnPtr &b){
    return *(this->colPtr)>*(b.colPtr);
}
const bool ColumnPtr::operator>=(const ColumnPtr &b){
    return *(this->colPtr)>=*(b.colPtr);
}
const bool ColumnPtr::operator<(const ColumnPtr &b){
    return *(this->colPtr)<*(b.colPtr);
}
const bool ColumnPtr::operator<=(const ColumnPtr &b){
    return *(this->colPtr)<=*(b.colPtr);
}
const bool ColumnPtr::operator==(const ColumnPtr &b){
    return *(this->colPtr)==*(b.colPtr);
}
const bool ColumnPtr::operator!=(const ColumnPtr &b){
    return *(this->colPtr)!=*(b.colPtr);
}
bdnode::bdnode():cnt(0){
    if(_d_btree==0){
        std::cout<<KRED;
        std::cout<<KRED;
        std::cout<<"Invalid size"<<std::endl;
        std::cout<<RST;
        std::cout<<RST;
        assert(0);
    }
    key=new ColumnPtr[2*_d_btree];
    ptr=new struct bdnode*[2*_d_btree+1]; /*store pointer to data*/
    for(int i=0;i<=2*_d_btree;i++){
        ptr[i]=NULL;
    }
}



BDPTR create(ColumnPtr key){
    BDPTR temp=new struct bdnode;
    temp->key[0]=key;
    temp->cnt=1;
    return temp;
}

BDPTR split(BDPTR &original,BDPTR &extra,int i){
    int mid=_d_btree;
    BDPTR left=new struct bdnode;
    if(mid==i){
        int j,k;
        for(j=mid,k=0;j<original->cnt;k++,j++){
            left->key[k]=original->key[j];
            left->ptr[k]=original->ptr[j];
        }
        left->ptr[k]=original->ptr[j];
        original->cnt=mid;
        original->ptr[mid]=extra->ptr[0];
        left->cnt=_d_btree;
        left->ptr[0]=extra->ptr[1];
        extra->ptr[0]=original;
        extra->ptr[1]=left;
    }else if(i<mid){
        BDPTR lc,rc;
        lc=original->ptr[mid-1];
        rc=original->ptr[mid];
        ColumnPtr kk=original->key[mid-1];
        int j,k;
        for(j=mid-1;j>i;j--){
            original->key[j]=original->key[j-1];
            original->ptr[j]=original->ptr[j-1];
        }
        original->key[i]=extra->key[0];
        original->ptr[i]=extra->ptr[0];
        original->ptr[i+1]=extra->ptr[1];
        if(i==mid-1){
            lc=extra->ptr[1];
        }
        for(j=mid,k=0;j<original->cnt;k++,j++){
            left->key[k]=original->key[j];
            left->ptr[k]=original->ptr[j];
        }
        left->ptr[k]=original->ptr[j];
        original->cnt=mid;

        original->ptr[mid]=lc;
        left->cnt=_d_btree;
        left->ptr[0]=rc;
        extra->key[0]=kk;
        extra->ptr[0]=original;
        extra->ptr[1]=left;
    }else{
        BDPTR lc,rc;
        lc=original->ptr[mid];
        rc=original->ptr[mid+1];
        ColumnPtr kk=original->key[mid];
        int j,k;
        for(j=mid;j<i;j++){
            original->key[j]=original->key[j+1];
            original->ptr[j]=original->ptr[j+1];
        }
        original->key[i-1]=extra->key[0];
        original->ptr[i-1]=extra->ptr[0];
        original->ptr[i]=extra->ptr[1];
        if(i==mid+1){
            rc=extra->ptr[0];
        }
        for(j=mid,k=0;j<original->cnt;k++,j++){
            left->key[k]=original->key[j];
            left->ptr[k]=original->ptr[j];
        }
        left->ptr[k]=original->ptr[j];
        left->cnt=_d_btree;
        original->cnt=mid;
        original->ptr[mid]=lc;
        left->ptr[0]=rc;
        extra->key[0]=kk;
        extra->ptr[0]=original;
        extra->ptr[1]=left;
    }
    return extra;
}
void insertAndSort(BDPTR &T,ColumnPtr key,int i){
    for(int j=T->cnt;j>i;j--)
        T->key[j]=T->key[j-1];
    T->key[i]=key;
    T->cnt++;
}
void insertAndSort(BDPTR &T,BDPTR &keyNode,int i){
    T->ptr[T->cnt+1]=T->ptr[T->cnt];
    for(int j=T->cnt;j>i;j--){
        T->key[j]=T->key[j-1];
        T->ptr[j]=T->ptr[j-1];
    }
    T->key[i]=keyNode->key[0];
    T->ptr[i]=keyNode->ptr[0];
    T->ptr[i+1]=keyNode->ptr[1];
    T->cnt++;
    //Deleting the extra node created
    delete keyNode;
}

//Function to add key to the B Tree
BDPTR Add(BDPTR T,ColumnPtr key){
    if(T==NULL) return create(key);
        int i=0;
        BDPTR test,extra;
        while(i<T->cnt&&key>T->key[i]) i++;
            if(T->ptr[i]==NULL){
                //leaf node
                if(T->cnt==2*_d_btree){
                    extra=create(key);
                    return split(T,extra,i);
                }else{
                    insertAndSort(T,key,i);
                    return T;
                }

            }else{
                test=Add(T->ptr[i],key);
                if(test!=T->ptr[i]){
                    //splitting had occured in the previous step
                    if(T->cnt==2*_d_btree){
                        return split(T,test,i);
                    }else{
                        insertAndSort(T,test,i);
                    }
                }
            }
            return T;
}
void shift(BDPTR &T,int i){
    int j;
    for(j=i;j<T->cnt-1;j++){
        T->key[j]=T->key[j+1];
        T->ptr[j]=T->ptr[j+1];
    }
        T->ptr[j]=T->ptr[j+1];
    T->cnt--;
}

void combine(BDPTR &T,int i){
    BDPTR left=T->ptr[i];
    BDPTR right=T->ptr[i+1];
    int j,k;
    left->key[left->cnt]=T->key[i];
    left->cnt++;
    for(j=left->cnt,k=0;k<right->cnt;k++,j++){
        left->key[j]=right->key[k];
        left->ptr[j]=right->ptr[k];
        left->cnt++;
    }
    left->ptr[j]=right->ptr[k];
    for(j=i;j<T->cnt-1;j++){
        T->key[j]=T->key[j+1];
        T->ptr[j]=T->ptr[j+1];
    }
        T->ptr[j]=T->ptr[j+1];
        T->cnt--;
    delete right;
    T->ptr[i]=left;
}

BDPTR remove(BDPTR &T,ColumnPtr key){
    if(T==NULL) return NULL;
    int i=0;
    while(i<T->cnt&&key>T->key[i])
        i++;

    if(T->ptr[i]==NULL){
        if(i==T->cnt||T->key[i]!=key)
            return T;
        //leaf node;
        shift(T,i);
    }else{
    if(i<T->cnt&&T->key[i]==key){
        ColumnPtr r;
        if(T->ptr[i]){
            r=T->ptr[i]->key[T->ptr[i]->cnt-1];
            T->key[i]=r;
        }else{
            T->key[i]=r;
            i++;
            r=T->ptr[i]->key[0];
        }

        T->ptr[i]=remove(T->ptr[i],r);

        }else{
            T->ptr[i]=remove(T->ptr[i],key);
        }
        if(T->ptr[i]&&T->ptr[i]->cnt<_d_btree){
            if(i<T->cnt&&T->ptr[i+1]&&T->ptr[i+1]->cnt>_d_btree){
                //redistribution possible
                insertAndSort(T->ptr[i],T->key[i],T->ptr[i]->cnt);
                T->key[i]=T->ptr[i+1]->key[0];
                shift(T->ptr[i+1],0);

            }else if(i>0&&T->ptr[i-1]&&T->ptr[i-1]->cnt>_d_btree){
                //redistribution possible
                insertAndSort(T->ptr[i],T->key[i-1],0);
                T->key[i-1]=T->ptr[i-1]->key[T->ptr[i-1]->cnt-1];
                T->ptr[i-1]->cnt--;
            }else{
                //redistribution not possible
                int rd;
                if(i==T->cnt){
                    rd=i-1;
                }else{
                    rd=i;
                }
                combine(T,rd);
            }
        }
    }
    if(T->cnt==0)
        T=T->ptr[0];
    return T;
}

void printColPtr(Column* col){
    for(int i=0;i<col->data.size();i++){
        std::cout<<col->data[i]<<" ";
    }
    std::cout<<"\b";
}
void printCol(ColumnPtr col){
    printColPtr(col.colPtr);
}
void printLevelOrder(BDPTR T){
    if(T==NULL){
        return;
    }

    std::queue<BDPTR> Q;
    Q.push(T);
    while(!Q.empty()){
        std::queue<BDPTR> Qtemp;
        while(!Q.empty()){
            BDPTR temp=Q.front();
            Q.pop();
            int i;
            std::cout<<"(";
            for(i=0;i<temp->cnt;i++){
                printCol(temp->key[i]);
                std::cout<<",";
                if(temp->ptr[i])
                Qtemp.push(temp->ptr[i]);
            }
            std::cout<<"\b";
                if(temp->ptr[i])
                Qtemp.push(temp->ptr[i]);
		    std::cout<<")";
                std::cout<<" ";
        }
        std::cout<<std::endl;
        Q=Qtemp;
    }
}

void printInOrder(BDPTR T){
    if(T==NULL)
        return;
    int i;
    for(i=0;i<T->cnt;i++){
        printInOrder(T->ptr[i]);
        printCol(T->key[i]);
        std::cout<<std::endl;
    }
    printInOrder(T->ptr[i]);
}


