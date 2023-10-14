//
// Created by Zhou on 2023/4/8.
//
#include "../GUI/Graph.h"
#include "../GUI/Simple_window.h"
int main(){
    using namespace Graph_lib;
    ///创建一个窗口
    Point tl{100,100};
    Simple_window win{tl,600,400,"Canvas_1"};
    win.wait_for_button();
    ///绘制坐标轴
    //x轴
    Axis axis_x{Axis::x,Point{35,360},320,10,"axis_x"};
    win.attach(axis_x);
    axis_x.set_color(Color::dark_cyan);
    axis_x.label.set_color(Color::dark_magenta);
    win.set_label("Canvas_2");
    win.wait_for_button();
    //y轴
    Axis axis_y{Axis::y,Point{35,360},320,10,"axis_y"};
    axis_y.set_color(Color::dark_cyan);
    axis_y.label.set_color(Color::dark_magenta);
    win.attach(axis_y);
    win.set_label("Canvas_3");
    win.wait_for_button();
    ///绘制函数图
    Function sine {sin,0,100,Point{35,200},10000,40,40};
    sine.set_color(Color::dark_green);
    win.attach(sine);
    win.set_label("Canvas_4");
    win.wait_for_button();
}