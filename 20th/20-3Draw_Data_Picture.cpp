//
// Created by Zhou on 2023/4/21.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
struct Distribution{
    Distribution()=default;
    int year;
    int young;
    int middle;
    int old;
};
istream& operator>>(istream&is, Distribution&distribution) {
    char ch1=0, ch2=0, ch3=0;
    is>>ch1>>distribution.year>>ch2>>distribution.young>>distribution.middle>>distribution.old>>ch3;
    if (!is||ch1!='('||ch2!=':'||ch3!=')') {
        is.clear(ios_base::failbit);
        return is;
    }
    return is;
}
class Scale{
    int c_base;
    int base_year;
    double scale;
public:
    Scale(int Begin_draw_place, int Base_year, double Axis_scale): c_base{Begin_draw_place}, base_year{Base_year}, scale{Axis_scale}{}
    int operator()(int v)const {return c_base+ (v - base_year) * scale;}
};
int main(){
    using namespace Graph_lib;
    constexpr int base_year=1960;
    constexpr int end_year=2040;
    //窗口
    constexpr int win_x=1200;
    constexpr int win_y=700;
    constexpr int upper=50;
    constexpr int lower=100;
    constexpr int left=50;
    constexpr int right=50;
    constexpr int x_len=win_x-left-right;
    constexpr int y_len=win_y-upper-lower;
    Simple_window win{Point{100,100},win_x,win_y,"japanese-age-data"};
    //背景框架
    constexpr double x_scale=double(x_len)/(end_year-base_year);
    constexpr double y_scale=double (y_len)/100;
    Scale xs{left,base_year,x_scale};
    Scale ys{win_y-lower,0,-y_scale};
    Axis x{Axis::x,Point{left,win_y-lower},x_len,
           (end_year-base_year)/10,
           " year    1960                         1970                         1980                         1990"
           "                           2000                           2010                          2020                           2030                          2040"};
    x.label.set_font_size(15);
    x.label.move(-365,0);
    win.attach(x);
    Axis y{Graph_lib::Axis::y,Point{left,win_y-lower},y_len,10,"% of population"};
    y.label.set_font_size(15);
    win.attach(y);
    Line current_year{Point{xs(2008),ys(0)},Point{xs(2008),ys(100)}};
    current_year.set_style(Line_style{Graph_lib::Line_style::dash});
    win.attach(current_year);
    //绘图
    Open_polyline children;
    Open_polyline adult;
    Open_polyline aged;
    children.set_color(Color::red);
    win.attach(children);
    adult.set_color(Color::blue);
    win.attach(adult);
    aged.set_color(Color::dark_green);
    win.attach(aged);
    //数据读入
    ifstream ifs{R"(D:\MyProgramme\C++_Programming\C++_Principle_Practise_GUI\20th\japanese-age-data.txt)"};
    if (!ifs) error("Can not open file !");
    for (Distribution d{}; ifs>>d; ) {
        if (d.year<base_year||d.year>end_year) error("Year out of range !");
        if (d.young+d.middle+d.old!=100) error("Percentages don't add up !");
        const int x=xs(d.year);
        children.add(Point{x,ys(d.young)});
        adult.add(Point{x,ys(d.middle)});
        aged.add(Point{x,ys(d.old)});
    }
    //贴标签
    Text children_label{Point{90,children.point(0).y},"age 0-14"};
    win.attach(children_label);
    children_label.set_color(Color::red);
    children_label.set_font_size(20);
    Text adults_label{Point{90,adult.point(0).y-30},"age 15-64"};
    win.attach(adults_label);
    adults_label.set_color(Color::blue);
    adults_label.set_font_size(20);
    Text aged_label{Point{90,aged.point(0).y-10},"age 65+"};
    win.attach(aged_label);
    aged_label.set_color(Color::dark_green);
    aged_label.set_font_size(20);
    gui_main();

}