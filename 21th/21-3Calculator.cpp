//
// Created by Zhou on 2023/4/28.
//
#include "../GUI/std_lib_facilities.h"
#include "../GUI/GUI.h"
#include "../GUI/Graph.h"
#include "../GUI/Window.h"
using namespace Graph_lib;
//------------------------------------------------------------------------------
///********************\n
///以下是非GUI部分\n
///********************\n
//与GUI输入输出对接
stringstream GUI_IN;
stringstream GUI_OUT;

//常量定义
const char NUMBER = 'N';
const char QUIT = 'q';
const char PRINT = ';';
const char EXIT = '~';
const char DEFINE = '=';
const char NAME = 'a';
const char LET = 'L';
const string DECLARATION_KEY = "let";

//输入后存储类型和输入流的类型实现
class Token
{
public:
    char kind;
    double value = 0;
    string name;
    Token(char ch) :kind{ ch } {};
    Token(char ch, double val) :kind{ ch }, value{ val } {};
    Token(char ch, string s) :kind{ ch }, name{ s } {};
};
class Token_Stream
{
public:
    //获取一个Token单词
    Token get();
    //放回一个单词
    void putback(Token t);
    //清除直到字符ch的所有字符
    void cleanup_mess(char ch);

private:
    //缓存区内是否有单词
    bool is_full{false };
    //存储放回单词的缓冲区
    Token buffer=Token(' ',0);
};
Token_Stream token_stream;

//变量定义的类型实现
class Variable
{
public:
    string name;
    double value;
};
vector<Variable>var_table;

//实现计算器基本文法规则的分析器函数声明
double expression();
double term();
double middle();
double primary(Token t);

//实现变量定义的函数声明
double get_value(string get_name);
void set_value(string set_name, double set_value);
double statement();
double declaration();
bool isAlpha(char ch);
bool isDigit(char ch);
double define_name(string variable, double value);
bool is_declared(string variable);

//其他函数声明
inline void keep_your_window_open(char ch);
void calculate();
void clean_up_mess();
void error_handling();

//输入后存储类型和输入流的类型的相关函数实现
void Token_Stream::putback(Token t)
{
    if (is_full)
    {
        //避免连续两次putback()而没有get()
        error("WRONG");
    }
    else
    {
        buffer = t;
        is_full = true;
    }
}
Token Token_Stream::get()
{
    if (is_full)
    {
        is_full = false;
        return buffer;
    }
    else
    {
        char ch;
        GUI_IN >> ch;
        switch (ch)
        {
            case PRINT:		//立即输出结果
            case QUIT:		//立即退出
            case DEFINE:
            case '(':
            case ')':
            case '{':
            case '}':
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '!':
            case '^':
            {
                Token t = { ch,0 };
                return t;
            }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '.':							//可能输入了小数
            {
                //将刚刚读入的内容放回标准输入流
                GUI_IN.putback(ch);
                double value;
                GUI_IN >> value;
                Token t = { NUMBER,value };
                return t;
            }
            default:
                if (isAlpha(ch))
                {
                    string s;
                    GUI_IN >> s;
                    s += ch;
                    while (GUI_IN.get(ch)&&(isAlpha(ch)||isDigit(ch)))
                    {
                        s + ch;
                    }
                    GUI_IN.putback(ch);
                    if (s==DECLARATION_KEY)
                    {
                        return Token(LET);
                    }
                    else
                    {
                        Token t = { NAME,s };
                        return t;
                    }
                }
                else
                {
                    error("WRONG");
                }
        }
    }
}
void Token_Stream::cleanup_mess(char ch)
{
    //首先查看缓冲区
    if (is_full && (ch == buffer.kind))
    {
        is_full = false;
        return;
    }
    else
    {
        is_full = false;
        //查找并吸收不需要的错误流
        char ch_rubbish;
        while (cin>>ch_rubbish)
        {
            if (ch_rubbish==ch)
            {
                return;
            }
        }
    }
}

//实现计算器基本文法规则的分析器函数实现
double expression()
/*
Expression表达式的文法规则：
	Term
	Expression '+' Term
	Expression '-' Term
*/
{
    double left_data = term();
    Token t = token_stream.get();
    for ( ; ; )
    {
        switch (t.kind)
        {
            case '+':
                left_data += term();
                t = token_stream.get();
                break;
            case '-':
                left_data -= term();
                t = token_stream.get();
                break;
            default:
                token_stream.putback(t);
                return left_data;
        }
    }
}
double term()
/*
Term项的文法规则：
	Primary
	Term '*' Middle
	Term '/' Middle
	Term '%' Middle
*/
{
    double left_data = middle();
    Token t = token_stream.get();
    for ( ; ; )
    {
        switch (t.kind)
        {
            case '*':
                left_data *= middle();
                t = token_stream.get();
                break;
            case '/':
            {
                double divisor = middle();
                if (divisor != 0)
                {
                    left_data /= divisor;
                    t = token_stream.get();
                }
                else
                {
                    error("WRONG");
                }
            }
                break;
            case '%':
            {
                double divided = middle();
                if (divided==0)
                {
                    error("WRONG");
                }
                else
                {
                    left_data = fmod(left_data, divided);
                    t = token_stream.get();
                }
            }
                break;
            default:
                token_stream.putback(t);
                return left_data;
        }
    }
}
double middle()
/*
Middle基本表达式的文法规则：
	primary
	'(' Expression ')'
	'(' Expression ')'!
	'(' Expression ')'^'(' Expression ')'
	‘+’ Expression
	‘-’ Expression
*/
{
    //number是get_token函数可以直接读取到的内容
    Token t = token_stream.get();
    for (; ; )
    {
        switch (t.kind)
        {
            case '{':
            {
                double expr = expression();
                t = token_stream.get();
                if (t.kind == '}')
                {
                    Token token = { NUMBER, expr };
                    expr = primary(token);
                    return expr;
                }
            }
            case '(':
            {
                double expr = expression();
                t = token_stream.get();
                if (t.kind == ')')
                {
                    Token token = { NUMBER,expr };
                    expr = primary(token);
                    return expr;
                }
            }
            case NUMBER:
                return primary(t);
            case '+':
                return middle();
            case '-':
                return -middle();
            default:
                error("WRONG");
        }
    }
}
double primary(Token t)
/*
Primary基本表达式的文法规则：
	Number
	expression!
	expression^expression
*/
{
    char next;
    GUI_IN >> next;
    if (next == '!')
    {
        double ret = t.value - (int)t.value;
        if (ret == 0)
        {
            //t.kind内存储的是一个整数
            int num = (int)t.value;
            int result = 1;
            //计算阶乘
            for (int i = num; i >= 1; i--)
            {
                result *= i;
            }
            return result;
        }
        else
        {
            error("WRONG");
        }
    }
    else if (next == '^')
    {
        double order = expression();
        return pow(t.value, order);
    }
    else
    {
        //退回给cin输入流
        GUI_IN.putback(next);
        return t.value;
    }
}

//实现变量定义的函数实现
double get_value(string get_name)
{
    for (const Variable v : var_table)
    {
        if (v.name==get_name)
        {
            return v.value;
        }
    }
    error("WRONG");
}
void set_value(string set_name, double set_value)
{
    /*设置名字set_name的变量值为set_value*/
    for (Variable v : var_table)
    {
        if (set_name==v.name)
        {
            v.value = set_value;
            return;
        }
    }
    error("WRONG");
}
double statement()
{
    Token t = token_stream.get();
    switch (t.kind)
    {
        case LET:
            return declaration();
        default:
            token_stream.putback(t);
            return expression();
    }
}
double declaration()
{
    Token t = token_stream.get();
    if (t.kind!=NAME)
    {
        error("WRONG");
    }
    else
    {
        string var_name = t.name;
        Token t1 = token_stream.get();
        if (t1.kind!='=')
        {
            error("WRONG");
        }
        else
        {
            double value = expression();
            define_name(var_name, value);
            return value;
        }
    }
}
bool is_declared(string variable)
{
    for (Variable v : var_table)
    {
        if (v.name==variable)
        {
            return true;
        }
    }
    return false;
}
bool isAlpha(char ch)
{
    if (ch>='a'&&ch<='z')
    {
        return true;
    }
    else if (ch >= 'A' && ch <= 'Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool isDigit(char ch)
{
    if (ch>='0'&&ch<='9')
    {
        return true;
    }
    else
    {
        return false;
    }
}
double define_name(string variable,double value)
{
    if (is_declared(variable))
    {
        error("WRONG");
    }
    else
    {
        Variable v{ variable,value };
        var_table.push_back(v);
        return value;
    }
}

//其他函数实现
void calculate()
{
    Token t = token_stream.get();
    while (t.kind == PRINT)
    {
        GUI_OUT<< statement() << '\n';
        GUI_OUT.str(std::string());
    }
    if (t.kind == QUIT)
    {
        //退出
        return;
    }
}
void do_calculate(){
    try {
        define_name("pi", 3.1415926535);
        define_name("e", 2.7182818284);
        while (GUI_IN) {
            try {
                GUI_OUT.str(std::string());
                calculate();
            }
            catch (const std::exception &err) {
                GUI_OUT << "ERROR: " << err.what() << '\n';
            }
            GUI_OUT.str(std::string());
        }
    } catch (exception &err) {
        GUI_OUT<<"ERROR: "<<err.what()<<'\n';
    }
    GUI_OUT.str(std::string());
}

//------------------------------------------------------------------------------
///******************\n
/// 以下是GUI部分\n
/// ******************\n
class Calculator_window:Window{
public:
    Calculator_window(int width, int height, string name);
private:
    //输出
    Out_box Input_Show;
    Out_box Result_Output;
    //输入缓冲
    stringstream Input_Buffer;
    //运算符
    Button Plus;
    Button Minus;
    Button Multiply;
    Button Divide;
    Button Modulus;
    Button Pow;
    Button Factorial;
    //括号和确认键
    Button Bracket_Small_Left;
    Button Bracket_Small_Right;
    Button Bracket_Big_Left;
    Button Bracket_Big_Right;
    Button Enter;
    //数字
    Button Zero;
    Button One;
    Button Two;
    Button Three;
    Button Four;
    Button Five;
    Button Six;
    Button Seven;
    Button Eight;
    Button Nine;
    Button Decimal_Point;
    //Operations
    void Operation_Plus()
    {
        Input_Buffer<<'+';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Minus()
    {
        Input_Buffer<<'-';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Multiply()
    {
        Input_Buffer<<'*';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Divide()
    {
        Input_Buffer<<'/';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Modulus()
    {
        Input_Buffer<<'%';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Pow()
    {
        Input_Buffer<<'^';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Factorial()
    {
        Input_Buffer<<'!';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Bracket_Small_Left()
    {
        Input_Buffer<<'(';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Bracket_Small_Right()
    {
        Input_Buffer<<')';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Bracket_Big_Left()
    {
        Input_Buffer<<'{';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Bracket_Big_Right()
    {
        Input_Buffer<<'}';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Zero()
    {
        Input_Buffer<<'0';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_One()
    {
        Input_Buffer<<'1';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Two()
    {
        Input_Buffer<<'2';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Three()
    {
        Input_Buffer<<'3';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Four()
    {
        Input_Buffer<<'4';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Five()
    {
        Input_Buffer<<'5';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Six()
    {
        Input_Buffer<<'6';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Seven()
    {
        Input_Buffer<<'7';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Eight()
    {
        Input_Buffer<<'8';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Nine()
    {
        Input_Buffer<<'9';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Decimal_Point()
    {
        Input_Buffer<<'.';
        Input_Show.put(GUI_IN.str());
        redraw();
    }
    void Operation_Enter();
};

void Calculator_window::Operation_Enter() {
    Input_Buffer<<' ';
    GUI_IN<<Input_Buffer.str();
    Input_Buffer.str(std::string());
    do_calculate();
    Result_Output.put(GUI_OUT.str());
    Result_Output.show();
    redraw();
}

Calculator_window::Calculator_window(int width, int height, string name)
: Window(width,height, name),
  Input_Show(Point{50,10},530,85,"Input"),
  Result_Output(Point{50,100},530,200,"Result"),
  One(Point{10,320},100,100,"1",
      [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_One();}),
  Two(Point{130,320},100,100,"2",
      [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Two();}),
  Three(Point{250,320},100,100,"3",
        [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Three();}),
  Plus(Point{370,320},100,100,"+",
       [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Plus();}),
  Modulus(Point{490,320},100,100,"%",
          [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Modulus();}),
  Four(Point{10,440},100,100,"4",
       [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Four();}),
  Five(Point{130,440},100,100,"5",
       [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Five();}),
  Six(Point{250,440},100,100,"6",
      [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Six();}),
  Minus(Point{370,440},100,100,"-",
        [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Minus();}),
  Pow(Point{490,440},100,100,"^",
      [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Pow();}),
  Seven(Point{10,560},100,100,"7",
        [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Seven();}),
  Eight(Point{130,560},100,100,"8",
        [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Eight();}),
  Nine(Point{250,560},100,100,"9",
       [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Nine();}),
  Multiply(Point{370,560},100,100,"*",
           [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Multiply();}),
  Divide(Point{490,560},100,100,"/",
         [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Divide();}),
  Bracket_Small_Left(Point{10,680},100,100,"(",
                     [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Bracket_Small_Left();}),
  Bracket_Small_Right(Point{130,680},100,100,")",
                      [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Bracket_Small_Right();}),
  Factorial(Point{250,680},100,100,"!",
            [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Factorial();}),
  Decimal_Point(Point{370,680},100,100,".",
                [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Decimal_Point();}),
  Zero(Point{490,680},100,100,"0",
       [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Zero();}),
  Bracket_Big_Left(Point{10,800},100,100,"{",
                   [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Bracket_Big_Left();}),
  Bracket_Big_Right(Point{130,800},100,100,"}",
                    [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Bracket_Big_Right();}),
  Enter(Point{280,800},300,100,"Enter",
        [](Address,Address pw){ reference_to<Calculator_window>(pw).Operation_Enter();})
{
    attach(Zero);
    attach(One);
    attach(Two);
    attach(Three);
    attach(Four);
    attach(Five);
    attach(Six);
    attach(Seven);
    attach(Eight);
    attach(Nine);
    attach(Plus);
    attach(Modulus);
    attach(Minus);
    attach(Pow);
    attach(Multiply);
    attach(Divide);
    attach(Factorial);
    attach(Decimal_Point);
    attach(Bracket_Small_Left);
    attach(Bracket_Small_Right);
    attach(Bracket_Big_Left);
    attach(Bracket_Big_Right);
    attach(Enter);
    attach(Input_Show);
    attach(Result_Output);
}

int main()
{
    Calculator_window win{600,910,"Calculator"};
    return gui_main();
}
