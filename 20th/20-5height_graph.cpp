//
// Created by Zhou on 2023/4/25.
//
#include "../GUI/Simple_window.h"
#include "../GUI/Graph.h"
int main(){
    using namespace Graph_lib;
    Simple_window win{Point{100,100},800,600,"Height"};
    vector<double> data;
    vector<string>labels;
    ifstream ifs{R"(D:\MyProgramme\C++_Programming\C++_Principle_Practise_GUI\20th\people_height)"};
    while (true){
        double data0;
        string s0;
        ifs>>s0>>data0;
        if (!ifs)break;
        labels.push_back(s0);
        data.push_back(data0);
    }
    for (int i = 0; i < data.size(); ++i) {
        cout<<data[i]<<endl;
    }
    Column_Graph c{Point{50,50}, 700, 400, data, labels, "Height"};
    win.attach(c);
    win.wait_for_button();
}