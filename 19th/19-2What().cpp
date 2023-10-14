//
// Created by Zhou on 2023/4/20.
//
#include "../GUI/std_lib_facilities.h"
int main(){
    try{
        throw runtime_error("TEST");
    } catch (exception&e) {
        cerr<<e.what()<<endl;
    }
    try{
        vector<int>v;
        cout<<v[100];
    } catch (exception&e) {
        cerr<<e.what()<<endl;
    }
    try{
        unsigned long long i=10;
        for (;;--i) {
            cout<<99/i<<endl;
        }
    } catch (exception&e) {
        cerr<<e.what()<<endl;
    }
}