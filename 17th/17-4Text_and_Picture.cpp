//
// Created by Zhou on 2023/4/8.
//
#include <FL/Fl_JPEG_Image.H>
#include "../GUI/Graph.h"
#include "../GUI/Simple_window.h"
int main(){
    using namespace Graph_lib;
    //创建一个窗口
    Point tl{100,100};
    Simple_window win{tl,600,400,"Canvas_1"};
    //添加文字
    Text t{Point{150,150},"Hello Graphical world !"};
    t.set_color(Color::dark_yellow);
    t.set_font(Font::helvetica);
    t.set_font_size(30);
    win.attach(t);
    win.wait_for_button();
    //添加图片
    Image i{Point{100,50},R"(D:\MyProgramme\C++_Programming\C++_Principle_Practise_GUI\Picture\17-4Hello_Picture.jpg)"};
    win.attach(i);
    win.wait_for_button();
    i.move(200,200);
    win.wait_for_button();
}