//
// Created by Zhou on 2023/4/27.
//
#include "../GUI/window.h"
#include "../GUI/Graph.h"
#include "../GUI/GUI.h"

using namespace Graph_lib;
struct Lines_window : Window {
    Lines_window(Point location, int width, int height, const string& title);   //初始化所有成员
private:
    Open_polyline lines;
    Menu color_menu;
    Button next_button;  //将点添加到线
    Button quit_button;  //退出
    Button menu_button; //显示菜单

    In_box next_x;  //输入
    In_box next_y;  //输入
    In_box line_size;   //输入线条
    Out_box xy_out; //输入
    //回调
    static void cb_red(Address,Address);
    static void cb_blue(Address,Address);
    static void cb_black(Address,Address);

    //动作
    void red_pressed() {lines.set_color(Color::red); hide_menu_show_button();}
    void blue_pressed() {lines.set_color(Color::blue); hide_menu_show_button();}
    void black_pressed() {lines.set_color(Color::black); hide_menu_show_button();}
    void menu_pressed() {menu_button.hide(); color_menu.show();}
    void hide_menu_show_button() {menu_button.show(); color_menu.hide();}
    void next();
    void quit();
};

//初始化所有成员
Lines_window::Lines_window(Point location, int width, int height, const string &title)
        : Window(location, width, height, title),
          next_button(Point{x_max()-150,0}, 70, 20, "Next Point",[](Address, Address pw){ reference_to<Lines_window>(pw).next();}),
          quit_button(Point{x_max()-70,0},70,20,"Quit",[](Address, Address pw){ reference_to<Lines_window>(pw).quit();}),
          next_x(Point{x_max()-310,0},50,20,"next x:"),
          next_y(Point{x_max()-210,0},50,20,"next y:"),
          line_size(Point{x_max() - 310, 25}, 50, 20, "line_width:"),
          xy_out(Point{100,0},100,20,"current (x,y):"),
          color_menu(Point{x_max()-70,40},70,20,Menu::vertical,"color"),
          menu_button(Point{x_max() - 80, 30}, 80, 20, "color menu", [](Address, Address pw){ reference_to<Lines_window>(pw).menu_pressed();})
{
    attach(next_button);
    attach(quit_button);
    attach(next_x);
    attach(next_y);
    attach(xy_out);
    attach(lines);
    attach(line_size);
    color_menu.attach(new Button{Point{0,0},0,0,"red",cb_red});
    color_menu.attach(new Button{Point{0,0},0,0,"blue",cb_blue});
    color_menu.attach(new Button{Point{0,0},0,0,"black",cb_black});
    attach(color_menu);
    attach(menu_button);
    hide_menu_show_button();
}

void Lines_window::quit() {
    Fl_Window::hide();
}

void Lines_window::next() {
    //读取
    int x=next_x.get_int();
    int y=next_y.get_int();
    lines.add(Point{x,y});
    int fontsize=line_size.get_int();
    lines.set_style(Line_style{Graph_lib::Line_style::solid,fontsize});
    //更新当前位置
    ostringstream ss;
    ss<<'('<<x<<','<<y<<')';
    xy_out.put(ss.str());

    //重绘窗口
    redraw();
}

void Lines_window::cb_red(Address, Address pw) {
    reference_to<Lines_window>(pw).red_pressed();
}

void Lines_window::cb_blue(Address, Address pw) {
    reference_to<Lines_window>(pw).blue_pressed();
}

void Lines_window::cb_black(Address, Address pw) {
    reference_to<Lines_window>(pw).black_pressed();
}

int main(){
    using namespace Graph_lib;
    try{
        string Name;
        cout<<"Window Name: \n> ";
        cin>>Name;
        Lines_window win{Point{100,100},600,400,"周炳烨为你独家定制的\""+Name+'\"'};
        return gui_main();
    } catch (exception&error){
        cerr<<"exception: "<<error.what()<<endl;
        return 1;
    }
}