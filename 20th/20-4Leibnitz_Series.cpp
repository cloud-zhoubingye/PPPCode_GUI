//
// Created by Zhou on 2023/4/25.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
int times=0;
double leibnitz(double ){
    return 3.1414936/4;
};
double  my_function(double x){
    double sum=1;
    for (int i = 1; i <= times; ++i) {
        sum+=pow(-1,times)*(1.0/(2*times+1));
    }
    return sum;
}
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
    constexpr int number_of_points= 400;
    //比例因子
    constexpr int x_scale=30;
    constexpr int y_scale=30;
    //轴长
    constexpr int x_len=window_x_max;
    constexpr int y_len=window_y_max;

    Simple_window w{Point{100,100},window_x_max,window_y_max,"exp()"};
    Axis x{Graph_lib::Axis::x,Point{20,center_y_max},x_len,x_len/x_scale,"x"};
    Axis y{Graph_lib::Axis::y,Point{center_x_max,y_len+20},y_len,y_len/y_scale,"y"};
    x.set_style(Line_style{Graph_lib::Line_style::solid,3});
    y.set_style(Line_style{Graph_lib::Line_style::solid,3});
    w.attach(x);
    w.attach(y);
    Function f0 {leibnitz, r_min, r_max, center, number_of_points,2*x_scale,2*y_scale};
    f0.set_color(Color::red);
    f0.set_style(Line_style{Graph_lib::Line_style::solid,3});
    w.attach(f0);
    for (int i=0; i < 50; ++i,++times) {
        ostringstream ss;
        ss<<"leibnitz series approximation  |  n=="<<i;
        w.set_label(ss.str());
        Function f {my_function, r_min, r_max, center, number_of_points,2*x_scale,2*y_scale};
        f.set_color(Color::cyan);
        f.set_style(Line_style{Graph_lib::Line_style::solid,3});
        w.attach(f);
        w.wait_for_button();
        w.detach(f);
    }

}