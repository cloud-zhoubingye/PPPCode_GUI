//
// Created by Zhou on 2023/4/8.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
int main(){
    using namespace Graph_lib;
    Simple_window win{Point{0,0},8000,8000,"CheckerBoard"};
    Rectangle r{Point{25,25},750,750};
    r.set_style(Line_style{Line_style::solid,13});
    win.attach(r);
    Rectangle r1={Point{25,25},250,250};
    Rectangle r2={Point{275,25},250,250};
    Rectangle r3={Point{525,25},250,250};
    Rectangle r4={Point{25,275},250,250};
    Rectangle r5={Point{275,275},250,250};
    Rectangle r6={Point{525,275},250,250};
    Rectangle r7={Point{25,525},250,250};
    Rectangle r8={Point{275,525},250,250};
    Rectangle r9={Point{525,525},250,250};
    r1.set_style(Line_style{Graph_lib::Line_style::solid,4});
    r2.set_style(Line_style{Graph_lib::Line_style::solid,4});
    r3.set_style(Line_style{Graph_lib::Line_style::solid,4});
    r4.set_style(Line_style{Graph_lib::Line_style::solid,4});
    r5.set_style(Line_style{Graph_lib::Line_style::solid,4});
    r6.set_style(Line_style{Graph_lib::Line_style::solid,4});
    r7.set_style(Line_style{Graph_lib::Line_style::solid,4});
    r8.set_style(Line_style{Graph_lib::Line_style::solid,4});
    r9.set_style(Line_style{Graph_lib::Line_style::solid,4});
    r1.set_fill_color(Color::red);
    r3.set_fill_color(Color::red);
    r5.set_fill_color(Color::red);
    r7.set_fill_color(Color::red);
    r9.set_fill_color(Color::red);
    r2.set_fill_color(Color::white);
    r4.set_fill_color(Color::white);
    r6.set_fill_color(Color::white);
    r8.set_fill_color(Color::white);
    win.attach(r1);
    win.attach(r2);
    win.attach(r3);
    win.attach(r4);
    win.attach(r5);
    win.attach(r6);
    win.attach(r7);
    win.attach(r8);
    win.attach(r9);
    win.wait_for_button();
}