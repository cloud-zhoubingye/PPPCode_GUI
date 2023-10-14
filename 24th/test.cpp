#include "iostream"
#include "list"
using namespace std;

template <class T >
void Merge(list<T>&la, list<T> &lb, list<T> &lc){
    /***
     * 参数la,lb为递增链表，lc为空链表
     * Merge()将lc变成la+lb的单减链表
     ***/

    for (;!la.empty()&&!lb.empty();) {
        if (la.back()>lb.back()){
            lc.push_back(la.back());
            la.pop_back();
        } else{
            lc.push_back(lb.back());
            lb.pop_back();
        }
    }
    if (!la.empty()){
        la.reverse();
        for (T elem:la) {
            lc.push_back(elem);
        }
    } else if(!lb.empty()){
        lb.reverse();
        for (T elem:lb) {
            lc.push_back(elem);
        }
    }
}

template <class T >
void print(list<T>l){
    for (T elem:l) {
        cout<<elem<<"  ";
    }
    cout<<endl;
}

int main(){
    list<int>la={1,3,5,7};
    list<int>lb={2,4,6,8};
    list<int>lc;
    Merge(la,lb,lc);
    print(lc);
}