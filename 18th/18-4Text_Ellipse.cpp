//
// Created by Zhou on 2023/4/11.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
struct MyText:Shape{
    MyText(Point x, const string&s) : lab{s} { add(x); }
    void draw_lines()const { fl_draw(lab.c_str(), point(0).x, point(0).y); }
    void set_label(const string s) { lab=s; }
    string lable()const { return lab; }
    void set_font(const Font f) { fnt=f; }
    Font font() { return fnt; }
    void set_font_size(const int sz) { fnt_sz=sz; }
    int font_size() { return fnt_sz; }
private:
    string lab;
    Font fnt{fl_font()};
    int fnt_sz{ (fl_size()<14)? 14:fl_size() };
};
int main(){
    using namespace Graph_lib;
    Simple_window win{Point{100,100},800,600,"Text"};
    MyText t{Point{200,100},"HELLO"};
    cout<<t.lable()<<endl;
    t.set_font(Font::helvetica_bold_italic);
    t.set_font_size(95);
    win.attach(t);
    win.wait_for_button();
    Marked_polyline markedPolyline{"*/*/-+/",{{100,100},{200,300},{400,250}}};
    win.attach(markedPolyline);
    win.wait_for_button();
    Ellipse e{Point{100,100},80,40};
    e.set_fill_color(Color::green);
    win.attach(e);
    Circle c{Point{400,400},55};
    c.set_fill_color(Color::dark_cyan);
    win.attach(c);
    win.wait_for_button();
}