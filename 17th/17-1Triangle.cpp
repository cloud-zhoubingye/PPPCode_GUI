//
// Created by Zhou on 2023/4/7.
//
#include "../GUI//Graph.h"
#include "../GUI/Simple_window.h"
int main (int argc, char *argv[]) {
    using namespace Graph_lib;
    Point tl{100,100};
    Simple_window win{tl,800,600,"Canvas"};
    Polygon poly;
    poly.add(Point{10,10});
    poly.add(Point{790,10});
    poly.add(Point{400,590});
    poly.set_color(Color::red);
    win.attach(poly);
    win.wait_for_button();
}

