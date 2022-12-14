#include <iostream>
#include <string>
#include <string.h>
using namespace std;
enum Error_code{underflow,success,overflow,illegal,range_Error};

template<class type>
class Record
{
public:
    Record();
    Record(const type &x,const type &y);
    type the_key()const;
    type the_other()const;
    void operator=(const Record &copy);

protected:
    type key;
    type other;
};
template<class type>
Record<type>::Record()
{

}
template<class type>
Record<type>::Record(const type &x,const type &y)
{
    key = x;
    other = y;
}
template<class type>
type Record<type>::the_key()const
{
    return key;
}
template<class type>
type Record<type>::the_other()const
{
    return other;
}
template<class type>
void Record<type>::operator=(const Record &copy)
{
    key = copy.key;
    other = copy.other;
}


template<class type>
class Key
{
    type key;
public:
    Key();
    Key(type x);
    Key(const Record<type> &r);
    type the_key()const;
};
template<class type>
Key<type>::Key()
{

}
template<class type>
Key<type>::Key(type x)
{
    key = x;
}

template<class type>
Key<type>::Key(const Record<type> &r)
{
    key = r.the_key();
}

template<class type>
type Key<type>::the_key()const
{
    return key;
}
template<class type>
bool operator ==(const Key<type> &x,const Key<type> &y)
{
    //Key::comparisons++;
    return x.the_key()==y.the_key();
}
template<class type>
bool operator > (const Key<type> &x,const Key<type> &y)
{
    return x.the_key()>y.the_key();
}
template<class type>
bool operator < (const Key<type> &x,const Key<type> &y)
{
    return x.the_key()<y.the_key();
}
template<class type>
bool operator >=(const Key<type> &x,const Key<type> &y)
{
    return x.the_key()>=y.the_key();
}
template<class type>
bool operator <=(const Key<type> &x,const Key<type> &y)
{
    return x.the_key()<=y.the_key();
}
template<class type>
bool operator !=(const Key<type> &x,const Key<type> &y)
{
    return x.the_key()!=y.the_key();
}

template<class Node_entry>
struct node
{
    Node_entry entry;
    node<Node_entry> *back,*next;

    node();
    node(Node_entry item, node<Node_entry>* link_back=NULL,node<Node_entry> *link_next=NULL);
};
template<class Node_entry>
node<Node_entry>::node()
{
    back=next=NULL;
}
template<class Node_entry>
node<Node_entry>::node(Node_entry item, node<Node_entry> *link_back,node<Node_entry> *link_next)
{
    entry=item;
    back=link_back;
    next=link_next;
}

//doubly list
template<class List_entry>
class double_list
{
public:
    double_list();
    ~double_list();
    double_list(const double_list &copy);

    void operator=(const double_list &copy);

    int size()const;
    bool full()const;
    bool empty()const;
    void clear();
    Error_code insert(int position,const List_entry&x);
    Error_code remove(int position,List_entry&x);
    Error_code retrieve(int position,List_entry&x)const;
    Error_code replace(int position,List_entry x);

    void traverse(void(*visit)(List_entry &x));
protected:
    int count;
//???????????????head????????????????????????????????????????????????..??????!
    node<List_entry> *head;
    mutable int current_position;
    mutable node<List_entry>* current;
    void set_position(int position)const;
};
/**/
/*set_position*/
template<class List_entry>
void double_list<List_entry>::set_position(int position)const
{//current?????????
    if(current_position<=position)
        for(; current_position!=position; ++current_position)
            current=current->next;

//current?????????
    else
        for(; current_position!=position; --current_position)
            current=current->back;
}
/*constructors*/
template<class List_entry>
double_list<List_entry>::double_list()
{
    count=0;current_position=-1;
    head=current=NULL;
}

template<class List_entry>
double_list<List_entry>::~double_list()
{
    List_entry x;
    while(!empty())
        remove(0,x);
}
template<class List_entry>
double_list<List_entry>::double_list(const double_list &copy)
{
    count=0;
    head=NULL;
    node<List_entry> *new_node=copy.head;
    while(new_node){
        insert(size(),new_node->entry);
        new_node=new_node->next;
    }
}

template<class List_entry>
void double_list<List_entry>::operator=(const double_list &copy)
{
    List_entry x;
//if-->x=x  ???&?????????????????? x=y this ??????????????????main?????????????????????x
    if(this == &copy)
        return;
//?????????
    while(!empty())
        remove(0,x);
//?????????
    node<List_entry> *new_node=copy.head;
    while(new_node){
        insert(size(),new_node->entry);
        new_node=new_node->next;
    }
}

template<class List_entry>
int double_list<List_entry>::size()const
{
    return count;
}
template<class List_entry>
bool double_list<List_entry>::full()const
{
    return false;
/*//?????????????????????????????????????????????????????????????????????
    node *new_node = new node();
    if(new_node==NULL)
        return true;
//!!delete it!!! don't forget
    delete new_node;
    return false;
*/
}

template<class List_entry>
bool double_list<List_entry>::empty()const
{
    return count==0;
}

template<class List_entry>
void double_list<List_entry>::clear()
{
    node<List_entry> *p,*q;

    if(current==NULL)
        return;
    for(p=current; p; p=q){
        q=p->next;
        delete p;
    }
    for(p=current; p; p=q){
        q=p->back;
        delete p;
    }
    count=0;
    current=NULL;
    current_position=-1;

    return;
}

/* insert */
template<class List_entry>
Error_code double_list<List_entry>::insert(int position,const List_entry&x)
{
    if(position<0 || position>count) return range_Error;

    node<List_entry> *new_node,*previous,*following;
//??????????????????
    if(position==0){
        if(count==0)
            following=NULL;
        else{
            set_position(0);
            following=current;
        }
        previous=NULL;
    }
//????????????(??????????????????????????????)
    else{
        set_position(position-1);
        previous=current;
        following=current->next;
    }

    new_node = new node<List_entry>(x,previous,following);
    if(new_node==NULL) return overflow;
//???????????????
//??????????????????newnode???back???next?????? previous???following???next???back???????????????
    if(previous!=NULL)
        previous->next=new_node;
    if(following!=NULL)
        following->back=new_node;
//?????? current,count,current_position
    current=new_node;
    current_position=position;
    count++;
//!!don't forget my HEAD!orz
    if(position==0)
        head=new_node;
    return success;
}

template<class List_entry>
Error_code double_list<List_entry>::remove(int position,List_entry&x)
{
    if(count==0 || position<0 || position>=count) return illegal;

    node<List_entry> *old_node,*neighbor;

    set_position(position);
    old_node=current;
/*!!??????!!
??????????????????????????????????????????neighbor?????????NULL
node 1    old node(AIM)     node 2*/
/*???????????????????????????node1->next??????old_node->next
 ??????cur????????????????????????*/
    if(neighbor=current->back)
        neighbor->next=current->next;
/*?????????node2-<back??????node1
??????current????????????????????????*/
    if(neighbor=current->next){
        neighbor->back=current->back;
        current=neighbor;
    }
//old_node=current;
    else{
        current=current->back;
        current_position--;
    }

    x=old_node->entry;
    delete old_node;
    count--;

    return success;
}

template<class List_entry>
Error_code double_list<List_entry>::retrieve(int position,List_entry&x)const
{
    if (position < 0 || position >= count) return range_Error;
    set_position(position);
    x = current->entry;
    return success;
}

template<class List_entry>
Error_code double_list<List_entry>::replace(int position,List_entry x)
{
    if (position < 0 || position >= count) return range_Error;

    set_position(position);
    current->entry = x;
    return success;
}

template<class List_entry>
void double_list<List_entry>::traverse(void(*visit)(List_entry &x))
{
    node<List_entry>* newptr = head;
    while (newptr) {
        (*visit)(newptr->entry);
        newptr = newptr->next;
    }
}

template<class type>
class myhash
{
public:
    myhash();
    void getsize(unsigned int size);
    
    Error_code insert(const Record<type> &data);
    Error_code remove(const Key<type> &target);
    Error_code retrieve(const Key<type> &target);
    void clear();

    unsigned int BKDRHash(char *str);
private:
    unsigned int Hash_Size;
    double_list<Record<type>> table[100];
};

template<class type>
myhash<type>::myhash()
{
    memset(table, NULL, sizeof(table));
}

template<class type>
void myhash<type>::getsize(unsigned int size)
{
    Hash_Size = size;
}

template<class type>
Error_code myhash<type>::insert(const Record<type> &data)
{
    string tt = data.the_key();
    char *str = (char *)tt.c_str();
    unsigned pos = BKDRHash(str);

    for(int i=0; i<table[pos].size(); ++i){
        Record<type> tmp;
        table[pos].retrieve(i,tmp);

        //??????????????????????????????
        if(tmp.the_key() == data.the_key()){
            table[pos].replace(i,data);
            return success;
        }
    }
//???????????????
    table[pos].insert(0,data);
    return success;
}

template<class type>
Error_code myhash<type>::remove(const Key<type> &target)
{
    char *str=(char*)target.the_key().c_str();
    unsigned pos=BKDRHash(str);
    cout<<pos<<' ';

    for(int i=0; i<table[pos].size(); ++i){
        string tmp;
        Record<type> print1;
        table[pos].retrieve(i,print1);

        cout<<print1.the_key()<<' ';

        if(print1.the_key() == target.the_key()){
            table[pos].remove(i,print1);
            cout<<print1.the_other()<<endl;
            return success;
        }
    }
    return underflow;
}

template<class type>
Error_code myhash<type>::retrieve(const Key<type> &target)
{
    string s=target.the_key();
    char *str=(char*)s.c_str();
    unsigned pos = BKDRHash(str);
    cout<<pos<<' ';

    for(int i=0; i<table[pos].size(); ++i){
        Record<type> print1;
        table[pos].retrieve(i,print1);
        cout<<print1.the_key()<<' ';

        if(print1.the_key() == target.the_key()){
            cout<<print1.the_other()<<endl;
            return success;
        }
    }
    return underflow;
}

template<class type>
void myhash<type>::clear()
{
    for(int i=0; i<Hash_Size; ++i)
        table[i].clear();
}

template<class type>
unsigned int myhash<type>::BKDRHash(char *str)
{
    unsigned int seed = 31;
    unsigned int hash = 0;
    while(*str){
        hash = (hash *seed + (*str++)) % Hash_Size;
    }
    return (hash % Hash_Size);
//???hash??????????????????hash????????????????????????
}

int main()
{
    myhash<string> HASH;
    unsigned int n,m;
    cin>>n>>m;
    HASH.getsize(n);


    string str1,str2;

    while(m--){
        cin>>str1>>str2;
        Record<string> tmp(str1,str2);
        HASH.insert(tmp);
    }
//insert
    cin>>str1>>str2;
    char *str = (char*)str1.c_str();
    cout<<HASH.BKDRHash(str)<<endl;
    Record<string> data(str1,str2);
    HASH.insert(data);
//remove
    cin>>str1;
    Key<string> target(str1);
    if(HASH.remove(target) != success)
        cout<<str1<<" NULL"<<endl;
//retrieve
    cin>>str1;
    Key<string> goal(str1);
    if(HASH.retrieve(goal) != success)
        cout<<str1<<" NULL"<<endl;

    return 0;
}
