//
// Created by Zhou on 2023/4/11.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
int main(){
    Simple_window win{Point{350,220},600,400,"Rectangle 1"};
    Rectangle r00{Point{150,100},200,100};
    Rectangle r11{Point{50,50},Point{250,150}};
    Rectangle r12{Point{50,150},Point{250,250}};
    Rectangle r21{Point{250,50},200,100};
    Rectangle r22{Point{250,150},200,100};
    win.attach(r00);
    win.attach(r11);
    win.attach(r12);
    win.attach(r21);
    win.attach(r22);
    r00.set_fill_color(Color::yellow);
    r11.set_fill_color(Color::blue);
    r12.set_fill_color(Color::red);
    r21.set_fill_color(Color::green);
    win.wait_for_button();
    r11.move(400,0);
    r11.set_fill_color(Color::white);
    win.set_label("Rectangle 2");
    win.wait_for_button();
    win.put_on_top(r00);
    win.set_label("Rectangle 3");
    win.wait_for_button();
    r00.set_color(Color::invisible);
    r11.set_color(Color::invisible);
    r21.set_color(Color::invisible);
    r12.set_color(Color::invisible);
    r22.set_color(Color::invisible);
    win.set_label("Rectangle 4");
    win.wait_for_button();
}