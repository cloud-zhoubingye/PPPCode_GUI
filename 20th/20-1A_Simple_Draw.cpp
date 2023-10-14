//
// Created by Zhou on 2023/4/20.
//
#include "../GUI/Simple_window.h"
double one(double){return 1;}
double slope(double x){return x/2;}
double square(double x){return x*x;}
double sloping_cos(double x){return cos(x)+ slope(x);}
int main(){
    constexpr int window_x_max=600;
    constexpr int window_y_max=400;
    constexpr int center_x_max=window_x_max/2;
    constexpr int center_y_max=window_y_max/2+50;
    const Point center{center_x_max,center_y_max};
    //区间
    constexpr int r_min=-10;
    constexpr int r_max=11;
    //点数量
    constexpr int number_of_points= 1000;
    //比例因子
    constexpr int x_scale=30;
    constexpr int y_scale=30;

    Simple_window w{Point{100,100},window_x_max,window_y_max,"A Simple Draw"};

    constexpr int x_len=window_x_max-40;
    constexpr int y_len=window_y_max-40;
    Axis x{Graph_lib::Axis::x,Point{20,center_y_max},x_len,x_len/x_scale,"x"};
    Axis y{Graph_lib::Axis::y,Point{center_x_max,y_len+20},y_len,y_len/y_scale,"y"};
    x.set_style(Line_style{Graph_lib::Line_style::solid,3});
    x.set_color(Color::dark_red);
    y.set_style(Line_style{Graph_lib::Line_style::solid,3});
    y.set_color(Color::dark_red);
    y.notches.set_color(Color::cyan);
    w.attach(x);
    w.attach(y);

    Function f1{one,r_min,r_max,center,number_of_points,x_scale,y_scale};
    Function f2{slope,r_min,r_max,center,number_of_points,x_scale,y_scale};
    Function f3{square,r_min,r_max,center,number_of_points,x_scale,y_scale};
    Function f4{sloping_cos,r_min,r_max,center,number_of_points,x_scale,y_scale};
    f4.set_color(Color::red);
    Function f5{cos,r_min,r_max,center,number_of_points,x_scale,y_scale};
    Function f6{log,0.0000000001,r_max,center,number_of_points,x_scale,y_scale};
    f6.set_style(Line_style{Graph_lib::Line_style::solid,3});
    f6.set_color(Color::yellow);
    Function f7{sin,r_min,r_max,center,number_of_points,x_scale,y_scale};
    Function f8{exp,r_min,r_max,center,number_of_points,x_scale,y_scale};
    f8.set_style(Line_style{Graph_lib::Line_style::solid,3});
    f8.set_color(Color::cyan);
    Function f9{[](double x)->double {return sin(1.0/x);},r_min,r_max,center,number_of_points,2*x_scale,2*y_scale};
    f9.set_color(Color::red);
    Function f10([](double x){return (exp(x)+exp(-x))/2;},r_min,r_max,center,number_of_points,2*x_scale,2*y_scale);
    f10.set_color(Color::magenta);
    f10.set_style(Line_style{Graph_lib::Line_style::solid,3});
    Function f11([](double x){return 1/(exp(-x)+1);},r_min,r_max,center,number_of_points,2*x_scale,2*y_scale);
    f11.set_color(Color::magenta);
    f11.set_style(Line_style{Graph_lib::Line_style::solid,3});

    w.attach(f1);
    w.attach(f2);
    w.attach(f3);
    w.attach(f4);
    w.attach(f5);
    w.attach(f6);
    w.attach(f7);
    w.attach(f8);
    w.attach(f9);
    w.attach(f10);
    w.attach(f11);

    Text ts1{Point{20,center_y_max-40},"y=1"};
    Text ts2{Point{100,center_y_max+center_y_max/2-20},"y=x/2"};
    Text ts3{Point{center_x_max-60,12},"y=x*x"};
    w.attach(ts1);
    w.attach(ts2);
    w.attach(ts3);
    w.wait_for_button();

}