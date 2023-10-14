//
// Created by Zhou on 2023/4/21.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
int fac(int n) { return n>1? n* fac(n-1) : 1; }
double term(double x, int n) { return pow(x,n)/fac(n); }
int exp_times=0;
double exp_approximate(double x){
    double sum=0;
    for (int i = 0; i < exp_times; ++i) sum+= term(x,i);
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
    Function real_exp{exp,r_min,r_max,center,number_of_points,2*x_scale,2*y_scale};
    real_exp.set_style(Line_style{Graph_lib::Line_style::solid,3});
    real_exp.set_color(Color::red);
    w.attach(real_exp);
    Axis x{Graph_lib::Axis::x,Point{20,center_y_max},x_len,x_len/x_scale,"x"};
    Axis y{Graph_lib::Axis::y,Point{center_x_max,y_len+20},y_len,y_len/y_scale,"y"};
    x.set_style(Line_style{Graph_lib::Line_style::solid,3});
    y.set_style(Line_style{Graph_lib::Line_style::solid,3});
    w.attach(x);
    w.attach(y);

    for (; exp_times < 50; ++exp_times) {
        ostringstream ss;
        ss<<"exp approximation  |  n=="<<exp_times;
        w.set_label(ss.str());
        Function f {exp_approximate, r_min, r_max, center, number_of_points,2*x_scale,2*y_scale};
        f.set_color(Color::cyan);
        f.set_style(Line_style{Graph_lib::Line_style::solid,3});
        w.attach(f);
        w.wait_for_button();
        w.detach(f);
    }

}
