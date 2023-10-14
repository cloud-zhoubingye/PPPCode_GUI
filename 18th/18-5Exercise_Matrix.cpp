//
// Created by Zhou on 2023/4/13.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
int main(){
    Simple_window win(Point(100, 100), 700, 420, "rounded rectangles");
    Rounded_rectangle out(Point(10, 10), 630, 400, 20);
    out.set_style(Line_style(Line_style::solid, 5));
    out.set_fill_color(Color::red);
    win.attach(out);

    Rounded_rectangle left(Point(20, 20), 410, 380, 20);
    left.set_style(Line_style(Line_style::solid, 3));
    left.set_color(Color::dark_yellow);
    left.set_fill_color(Color::yellow);
    win.attach(left);

    Vector_ref<Rounded_rectangle> left_inner;
    for (int x = 30; x <= 330; x += 100) {
        Rounded_rectangle* r = new Rounded_rectangle(Point(x, 30), 90, 360);
        r->set_color(Color::dark_green);
        r->set_fill_color(Color::green);
        left_inner.push_back(r);
        win.attach(*r);
    }

    Rounded_rectangle right(Point(440, 20), 190, 380, 20);
    right.set_style(Line_style(Line_style::solid, 3));
    right.set_color(Color::dark_magenta);
    right.set_fill_color(Color::magenta);
    win.attach(right);

    Vector_ref<Rounded_rectangle> right_inner;
    for (int y = 30; y <= 215; y += 185) {
        Rounded_rectangle* r = new Rounded_rectangle(Point(450, y), 170, 175);
        r->set_color(Color::dark_cyan);
        r->set_fill_color(Color::cyan);
        right_inner.push_back(r);
        win.attach(*r);
    }
    Super_ellipse s{Point{300,300},100,100,10};
    s.set_fill_color(Color::green);
    win.attach(s);
    Arrow a{Point{100,100},Point{165,220}};
    win.attach(a);
    Rectangle r{Point{50,50},Point{80,80}};
    r.set_fill_color(Color::black);
    Line l{n(r),sw(r)};
    l.set_color(Color::white);
    l.set_style(Line_style{Graph_lib::Line_style::dashdot,10});
    win.attach(l);
    win.attach(r);
    Box b{Point{250,250},100,100,"WOW"};
    b.set_text_size(20);
    win.attach(b);
    win.wait_for_button();
    return 0;
}