//
// Created by Zhou on 2023/4/11.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
template<typename T>class Vector_Ref{
    vector<T*>v;
    vector<T*>owned;
public:
    Vector_Ref() {}
    ~Vector_Ref() { for (int i = 0; i < owned.size(); ++i) delete owned[i]; }
    //如果以引用方式传值，对象的生命周期由调用者自己处理（已命名对象）
    void push_back(T&s) { v.push_back(&s); }
    //如果以指针方式传值，假定它是用new分配的，由Vector_Ref负责释放空间（未命名对象）
    void push_back(T*p) { v.push_back(p); owned.push_back(p); }
    T& operator[](int i) { return *v[i]; }
    const T& operator[](int i)const { return *v[i]; }
    int size()const { return v.size(); }
};
int main(){
    Simple_window win{Point{100,100},550,480,"16*16 Color Matrix"};
    Vector_Ref<Rectangle>vr;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            vr.push_back(new Rectangle{Point{i*30,j*30},30,30});
            vr[vr.size()-1].set_fill_color(Color{i*16+j});
            win.attach(vr[vr.size()-1]);
        }
    }
    win.wait_for_button();

}