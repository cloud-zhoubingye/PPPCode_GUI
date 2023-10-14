//
// Created by Zhou on 03/05/2023.
//
#include "../GUI/Matrix.h"
#include "../GUI/MatrixIO.h"
#include "../GUI/window.h"
#include "../GUI/Graph.h"
#include "../GUI/GUI.h"
#include "sstream"
using namespace Graph_lib;
using namespace Numeric_lib;
typedef Numeric_lib::Matrix<double,2> my_matrix;
typedef Numeric_lib::Matrix<double,1> my_vector;
Index var_num;
ostringstream out;
istringstream in;
void classical_elimination(my_matrix &A, my_vector &b)
{
    //A为n*n阶方阵
    const Index n =A.dim1();

    //遍历第1~n-1列
    for (Index col = 0; col < n ; ++col) {
        const Index current_row = col;

        //该程序块内更换更合适的主元，以免主元为0
        {
            Index suitable_pivot_row = col;
            //查找合适的主元
            for (Index i = suitable_pivot_row + 1; i < n; ++i) {
                if (abs(A(i, col)) > abs(A(suitable_pivot_row, col)))
                    suitable_pivot_row = i;
            }
            //如果有更合适的主元，交换两行
            if (suitable_pivot_row != current_row){
                A.swap_rows(current_row, suitable_pivot_row);
                std::swap(b(current_row), b(suitable_pivot_row));
            }
        }

        //获取当前主对角线上的元素，如果为0，不存在唯一解
        const double pivot_element = A(current_row, col);
        if (pivot_element==0)
            error("There is no unique solution!");

        //初等行变换：将第col行的某倍数加至第col行 ~ 第n行
        for (Index row = current_row + 1; row < n; ++row) {
            //计算倍数
            const double multiplier = A(row, col) / pivot_element;
            //倍加
            A[row].slice(col) = scale_and_add(A[current_row].slice(col), -multiplier, A[row].slice(col));
            b(row) -= multiplier * b(current_row);
        }
    }
}
my_vector back_substitution(const my_matrix &A, const my_vector &b)
{
    //创建返回对象
    const Index n = A.dim1();
    my_vector result(n);

    //回代求解
    for (Index row = n - 1; row >= 0; --row) {
        const double col = row;

        //主元系数为0，有无穷多解，无法解出
        if (A(row, col)==0)
            error("There is no unique solution!");

        //回代
        double answer_of_x = (b(row) - dot_product(A[row].slice(col + 1), result.slice(col + 1)))/A(row, col);
        result(row) = answer_of_x;
    }
    return result;
}
my_vector classical_gaussian_elimination(my_matrix A, my_vector b)
{
    classical_elimination(A, b);
    return back_substitution(A, b);
}
void do_it()
{
    try{
        my_matrix A(var_num,var_num);
        my_vector b(var_num);
        in>>A>>b;
        cout<<"Get A:\n"<<A<<endl
            <<"Get b:\n"<<b<<endl;
        my_vector x = classical_gaussian_elimination(A, b);
        out<<x;
        cout<<"Done!\n"<<"Result is:\n"<<x;
    } catch (exception &e) {
        out<<e.what()<<endl;
    }
}

class My_Window : public Window{
public:
    My_Window(int width, int height, const string& title);
private:
    Button quit_button;
    Button calculate_button;
    In_box Matrix_A;
    In_box Matrix_b;
    In_box Var_cnt;
    Out_box Matrix_x;
    void quit();
    void show_answer();
};
My_Window::My_Window( int width, int height, const string &title)
: Window( width, height, title),
  quit_button(Point{x_max()-70, 0}, 70, 30, "Quit", [](Address, Address pw){ reference_to<My_Window>(pw).quit();}),
  calculate_button(Point{x_max()-70,35}, 70, 30, "Calculate", [](Address, Address pw){ reference_to<My_Window>(pw).show_answer();}),
  Matrix_A(Point{60,10},500,300,"Input A"),
  Matrix_b(Point{60,320},500,170,"Input b"),
  Matrix_x(Point{60,490},500,250,"Result x"),
  Var_cnt(Point{x_max()-70,66},70,40,"Var cnt")
{
    attach(quit_button);
    attach(calculate_button);
    attach(Matrix_A);
    attach(Matrix_b);
    attach(Matrix_x);
    attach(Var_cnt);
}
void My_Window::quit() {
    Fl_Window::hide();
}
void My_Window::show_answer() {
    var_num=Var_cnt.get_int();
    string input_a=Matrix_A.get_string();
    string input_b=Matrix_b.get_string();
    in.str(input_a+"\n"+input_b);
    do_it();
    Matrix_x.put(out.str());
    redraw();
    out.str(string{});
    in.str(string{});
}

int main(){
    cout<<"Hello!\nWelcome to use Liner Equations Calculator!\n";
    My_Window win{700,800,"线性方程组求解机"};
    return gui_main();
}