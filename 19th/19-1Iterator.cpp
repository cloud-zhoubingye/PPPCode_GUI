//
// Created by Zhou on 2023/4/19.
//
#include "iostream"
#include "vector"
#include "list"
#include "iterator"
using namespace std;
class Iterator{
public:
    virtual double next()=0;
protected:
    bool isFirst= true;

};
class Vector_iterator:public Iterator{
public:
    Vector_iterator(vector<double>&vector_init): v{vector_init}, i{0}{}
    double next(){
        if (isFirst){isFirst= false; return v[i];}
        if (i==v.size()-1)return -999;
        ++i;return v[i];}
private:
    vector<double>v;
    int i;
};
class List_iterator:public Iterator{
public:
    double next(){
        if (isFirst){ isFirst= false; return *i;}
        if (i!=l.end()){++i;
            return *i;}
        return -999;}
    List_iterator(list<double>&list_init): l{list_init},i{list_init.begin()}{}

private:
    list<double>l;
    list<double>::iterator i;
};
int main(){
    vector<double>vector1={11,22,33,44,55,66,77,88,99,100};
    Vector_iterator v{vector1};
    do {
        auto p=v.next();
        if (p==-999)break;
        cout<<p<<endl;
    } while (true);
    list<double>list1{1,2,3,4,5,6,7,8,9,10};
    List_iterator l{list1};
    do {
        auto p=l.next();
        if (p<1||p>100)break;
        cout<<p<<endl;
    } while (true);

}