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
    ///添加多边形
    Polygon poly;
    poly.add(Point{300,200});
    poly.add(Point{350,100});
    poly.add(Point{400,200});
    poly.set_color(Color::dark_red);
    poly.set_style(Line_style::dashdotdot);
    win.attach(poly);
    win.set_label("Canvas_5");
    win.wait_for_button();
    ///添加矩形
    Rectangle r{Point{190,170},100,50};
    win.attach(r);
    win.set_label("Canvas_6");
    win.wait_for_button();
    ///添加开闭图形
   //闭
    Closed_polyline closed_polyline;
    closed_polyline.add(Point{10,10});
    closed_polyline.add(Point{25,30});
    closed_polyline.add(Point{180,100});
    closed_polyline.add(Point{150,120});
    closed_polyline.add(Point{100,90});
    closed_polyline.add(Point{60,40});
    closed_polyline.add(Point{30,15});
    win.attach(closed_polyline);
    win.set_label("Canvas_7");
    win.wait_for_button();
    //开
    Open_polyline open_polyline;
    open_polyline.add(Point{110,110});
    open_polyline.add(Point{125,130});
    open_polyline.add(Point{280,200});
    open_polyline.add(Point{250,220});
    open_polyline.add(Point{120,190});
    open_polyline.add(Point{100,140});
    win.attach(open_polyline);
    win.set_label("Canvas_8");
    win.wait_for_button();
    //改变线型和填充颜色
    r.set_fill_color(Color::yellow);
    poly.set_style(Line_style{Graph_lib::Line_style::dashdot,4});
    poly.set_fill_color(Color::blue);
    closed_polyline.set_fill_color(Color::green);
    win.set_label("Canvas_9");
    win.wait_for_button();
}