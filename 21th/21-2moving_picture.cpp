//
// Created by Zhou on 2023/4/28.
//
#include "../GUI/Window.h"
#include "../GUI/Graph.h"
#include "../GUI/GUI.h"
int times=0;
int rand_int(int min, int max){
    static default_random_engine ran;
    return uniform_int_distribution<>{min,max}(ran);
}
using namespace Graph_lib;
struct Moving_picture_window:Window{
    Moving_picture_window(int width, int height, string label);
private:
    Image picture;
    Button move_button;
    void move_picture();
};

Moving_picture_window::Moving_picture_window(int width, int height, string label)
: Window(width,height,label),
  picture(Point{0,0},R"(D:\MyProgramme\C++_Programming\C++_Principle_Practise_GUI\21th\picture.jpg)"),
  move_button(Point{x_max()-70,10},68,28,"move",[](Address, Address pw){ reference_to<Moving_picture_window>(pw).move_picture();})
{
    attach(picture);
    attach(move_button);
}

void Moving_picture_window::move_picture() {
    int dx=rand_int(0,x_max()/2);
    int dy= rand_int(0,y_max()/2);
    times++;
    int cnt=pow(-1,times);
    picture.move(cnt*dx,cnt*dy);
    redraw();
}

int main(){
    Moving_picture_window win{1000,800,"moving picture"};
    return gui_main();
}