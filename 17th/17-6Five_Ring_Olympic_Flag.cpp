//
// Created by Zhou on 2023/4/8.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"

static const int i = 57;

static const int j = 40;

int main(){
    using namespace Graph_lib;
    Simple_window win{Point{0,0},800,600,"Olympics"};
    Rectangle r{Point{100,50},Point{700,500}};
    r.set_fill_color(Color::white);
    r.set_style(Line_style{Graph_lib::Line_style::solid,5});
    win.attach(r);
    Circle c1_O{Point{300,250}, i};
    c1_O.set_fill_color(Color::blue);
    c1_O.set_color(Color::invisible);
    win.attach(c1_O);
    Circle c1_I{Point{300,250}, j};
    c1_I.set_color(Color::invisible);
    c1_I.set_fill_color(Color::white);
    win.attach(c1_I);
    Circle c2_O{Point{400,250}, i};
    c2_O.set_fill_color(Color::black);
    c2_O.set_color(Color::invisible);
    win.attach(c2_O);
    Circle c2_I{Point{400,250}, j};
    c2_I.set_color(Color::invisible);
    c2_I.set_fill_color(Color::white);
    win.attach(c2_I);
    Circle c3_O{Point{500,250}, i};
    c3_O.set_fill_color(Color::red);
    c3_O.set_color(Color::invisible);
    win.attach(c3_O);
    Circle c3_I{Point{500,250}, j};
    c3_I.set_color(Color::invisible);
    c3_I.set_fill_color(Color::white);
    win.attach(c3_I);
    Circle c4_O{Point{350,330}, i};
    c4_O.set_fill_color(Color::green);
    c4_O.set_color(Color::invisible);
    win.attach(c4_O);
    Circle c4_I{Point{350,330}, j};
    c4_I.set_color(Color::invisible);
    c4_I.set_fill_color(Color::white);
    win.attach(c4_I);
    Circle c5_O{Point{450,330}, i};
    c5_O.set_fill_color(Color::dark_green);
    c5_O.set_color(Color::invisible);
    win.attach(c5_O);
    Circle c5_I{Point{450,330}, j};
    c5_I.set_color(Color::invisible);
    c5_I.set_fill_color(Color::white);
    win.attach(c5_I);
    Text t{Point{160,140},"Five-Ring Olympic Flag"};
    t.set_color(Color::black);
    t.set_font_size(45);
    win.attach(t);
    win.wait_for_button();
}