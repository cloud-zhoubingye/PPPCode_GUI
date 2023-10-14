//
// Created by Zhou on 2023/4/29.
//
#include "FL/Fl_Clock.H"
#include "../GUI/Window.h"
#include "../GUI/Graph.h"
#include "../GUI/GUI.h"
using namespace Graph_lib;
class Clock:Window{
private:

};

    int main(int argc, char* argv[]) {
        Fl_Window* window = new Fl_Window(850, 850, "Clock");
        Fl_Clock* clock = new Fl_Clock(20, 20, 820, 820);
        window->end();
        window->show(argc, argv);
        return Fl::run();
    }
