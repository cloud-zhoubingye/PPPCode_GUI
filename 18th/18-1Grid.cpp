//
// Created by Zhou on 2023/4/10.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
using namespace Graph_lib;
struct MyPoint{
    int x,y;
    MyPoint()=default;
    friend bool operator==(const MyPoint&p1, const MyPoint&p2);
    friend bool operator!=(const MyPoint&p1, const MyPoint&p2);
};
bool operator==(const MyPoint &p1, const MyPoint &p2) {
    return p1.x==p2.x&&p1.y==p2.y;
}
bool operator!=(const MyPoint &p1, const MyPoint &p2) {
    return !(p1==p2);
}
struct MyLines:Shape{
    MyLines()= default;
    MyLines(initializer_list<pair<MyPoint,MyPoint>>lst);
    void draw_lines()const override;
    void add(MyPoint p1, MyPoint p2);
};
MyLines::MyLines(initializer_list<pair<MyPoint,MyPoint>> lst) {
    for (auto p : lst) add(p.first,p.second);
}
void MyLines::draw_lines() const {
    if (color().visibility())
        for (int i = 1; i < number_of_points(); i += 2) {
            fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);
        }
}
void MyLines::add(MyPoint p1, MyPoint p2) {
    Point P1{p1.x,p1.y};
    Point P2{p2.x,p2.y};
    Shape::add(P1);
    Shape::add(P2);
}
struct MyOpenPolyline:Shape{
    using Shape::Shape;     //使用shape的构造函数
    void add(MyPoint p) {Shape::add(Point{p.x,p.y});}
};
struct MyClosedPolyline:MyOpenPolyline{
    using MyOpenPolyline::MyOpenPolyline;       //使用MyOpenPolyline的构造函数
    void draw_lines();
};
void MyClosedPolyline::draw_lines() {
    //画非封闭部分
    MyOpenPolyline::draw_lines();
    //画最后一条封闭部分
    if (number_of_points()>2&&color().visibility())
        fl_line(point(0).x, point(0).y, point(number_of_points()-1).x, point(number_of_points()-1).y);
}
int main(){
    Simple_window win{Point{100,100},1000,600,"Grid"};
    const int x_size=win.x_max();
    const int y_size=win.y_max();
    const int x_grid=80;
    const int y_grid=40;
    MyLines grid;
    win.attach(grid);
    for(int x=x_grid; x<x_size; x+=x_grid)      //垂直线
        grid.add(MyPoint{x,0},MyPoint{x,y_size});
    for(int y=y_grid; y<y_size; y+=y_grid)     //水平线
        grid.add(MyPoint{0, y}, MyPoint{x_size, y});
    win.wait_for_button();
    grid.set_color(99);
    win.wait_for_button();
    grid.set_style(Line_style{Graph_lib::Line_style::solid,2});
    win.wait_for_button();
    MyOpenPolyline pl;
    pl.add(MyPoint{100,100});
    pl.add(MyPoint{200,400});
    pl.add(MyPoint{50,300});
    win.attach(pl);
    win.wait_for_button();
}