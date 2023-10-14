
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#ifndef GRAPH_GUARD
#define GRAPH_GUARD 1

#include <FL/fl_draw.H>
#include <FL/Fl_Image.H>
#include "Point.h"
#include "std_lib_facilities.h"
#include "functional"

namespace Graph_lib {

// defense against ill-behaved Linux macros:
#undef major
#undef minor

//------------------------------------------------------------------------------

// Color is the type we use to represent line_size. We can use Color like this:
//    grid.set_color(Color::red);
struct Color {
    enum Color_type {
        red=FL_RED,
        blue=FL_BLUE,
        green=FL_GREEN,
        yellow=FL_YELLOW,
        white=FL_WHITE,
        black=FL_BLACK,
        magenta=FL_MAGENTA,
        cyan=FL_CYAN,
        dark_red=FL_DARK_RED,
        dark_green=FL_DARK_GREEN,
        dark_yellow=FL_DARK_YELLOW,
        dark_blue=FL_DARK_BLUE,
        dark_magenta=FL_DARK_MAGENTA,
        dark_cyan=FL_DARK_CYAN
    };

    enum Transparency : char { invisible = 0, visible=-1 };

    Color(Color_type cc) :c(Fl_Color(cc)), v(visible) { }
    Color(Color_type cc, Transparency vv) :c(Fl_Color(cc)), v(vv) { }
    Color(int cc) :c(Fl_Color(cc)), v(visible) { }
    Color(Transparency vv) :c(Fl_Color()), v(vv) { }    // default line_size
    Color(int red, int green, int blue):v(visible),c(fl_rgb_color(red,green,blue)){}

    int as_int() const { return c; }

    char visibility() const { return v; } 
    void set_visibility(Transparency vv) { v=vv; }
private:
    char v;    // invisible and visible for now
    Fl_Color c;
};
//------------------------------------------------------------------------------

struct Line_style {
    enum Line_style_type {
        solid=FL_SOLID,            // -------
        dash=FL_DASH,              // - - - -
        dot=FL_DOT,                // ....... 
        dashdot=FL_DASHDOT,        // - . - . 
        dashdotdot=FL_DASHDOTDOT,  // -..-..
    };

    Line_style(Line_style_type ss) :s(ss), w(0) { }
    Line_style(Line_style_type lst, int ww) :s(lst), w(ww) { }
    Line_style(int ss) :s(ss), w(0) { }

    int width() const { return w; }
    int style() const { return s; }
private:
    int s;
    int w;
};

//------------------------------------------------------------------------------

class Font {
public:
    enum Font_type {
        helvetica=FL_HELVETICA,
        helvetica_bold=FL_HELVETICA_BOLD,
        helvetica_italic=FL_HELVETICA_ITALIC,
        helvetica_bold_italic=FL_HELVETICA_BOLD_ITALIC,
        courier=FL_COURIER,
        courier_bold=FL_COURIER_BOLD,
        courier_italic=FL_COURIER_ITALIC,
        courier_bold_italic=FL_COURIER_BOLD_ITALIC,
        times=FL_TIMES,
        times_bold=FL_TIMES_BOLD,
        times_italic=FL_TIMES_ITALIC,
        times_bold_italic=FL_TIMES_BOLD_ITALIC,
        symbol=FL_SYMBOL,
        screen=FL_SCREEN,
        screen_bold=FL_SCREEN_BOLD,
        zapf_dingbats=FL_ZAPF_DINGBATS
    };

    Font(Font_type ff) :f(ff) { }
    Font(int ff) :f(ff) { }

    int as_int() const { return f; }
private:
    int f;
};

//------------------------------------------------------------------------------

template<class T> class Vector_ref {
    vector<T*> v;
    vector<T*> owned;
public:
    Vector_ref() {}
    Vector_ref(T& a) { push_back(a); }
    Vector_ref(T& a, T& b);
    Vector_ref(T& a, T& b, T& c);
    Vector_ref(T* a, T* b = 0, T* c = 0, T* d = 0)
    {
        if (a) push_back(a);
        if (b) push_back(b);
        if (c) push_back(c);
        if (d) push_back(d);
    }

    ~Vector_ref() { for (unsigned int i=0; i<owned.size(); ++i) delete owned[i]; }

    void push_back(T& s) { v.push_back(&s); }
    void push_back(T* p) { v.push_back(p); owned.push_back(p); }
/*
	void erase(int i)
	{
		if (i<0 || v.size()<=i) return;
		T* p = v[i];
		v.erase(v.begin()+i);
		for (int i=0; i<owned.size(); ++i)
				if (p==owned[i]) {
						delete p;
						owned.erase(owned.begin()+i);
				}
	}

	void erase(T* p)
	{
		for (int i=0; i<v.size(); ++i)
				if (p==v[i]) v.erase(&v[i]);
		for (int i=0; i<owned.size(); ++i)
				if (p==owned[i]) {
						delete p;
						owned.erase(&owned[i]);
				}
	}
*/
    T& operator[](int i) { return *v[i]; }
    const T& operator[](int i) const { return *v[i]; }

    int size() const { return v.size(); }

private:	// prevent copying
	Vector_ref(const Vector<T>&);
	Vector_ref& operator=(const Vector<T>&);
};

//------------------------------------------------------------------------------

typedef double Fct(double);

class Shape  {        // deals with line_size and style, and holds sequence of lines
public:
    void draw() const;                 // deal with line_size and draw lines
    virtual void move(int dx, int dy); // move the shape +=dx and +=dy

    Shape(const Shape&)=delete;               // prevent copying
    Shape& operator=(const Shape&)=delete;               // prevent copying

    void set_color(Color col) { lcolor = col; }
    Color color() const { return lcolor; }
    void set_style(Line_style sty) { ls = sty; }
    Line_style style() const { return ls; }
    void set_fill_color(Color col) { fcolor = col; }
    Color fill_color() const { return fcolor; }

    Point point(int i) const { return points[i]; } // read only access to points
    int number_of_points() const { return int(points.size()); }

    virtual ~Shape() { }
protected:
    Shape();
    virtual void draw_lines() const;   // draw the appropriate lines
    void add(Point p);                 // add p to points
    void set_point(int i,Point p);     // points[i]=p;
private:
    vector<Point> points;              // not used by all shapes
    Color lcolor;                      // line_size for lines and characters
    Line_style ls; 
    Color fcolor;                      // fill line_size
};

//------------------------------------------------------------------------------

struct Function : Shape {
    // the function parameters are not stored
    Function(Fct f, double r1, double r2, Point orig,
        int count = 100, double xscale = 25, double yscale = 25);    
};

//------------------------------------------------------------------------------

struct Line : Shape {            // a Line is a Shape defined by two Points
    Line(Point p1, Point p2);    // construct a line from two points
};

//------------------------------------------------------------------------------

struct Rectangle : Shape {

    Rectangle(Point xy, int ww, int hh) : w(ww), h(hh)
    {
        add(xy);
        if (h<=0 || w<=0) error("Bad rectangle: non-positive side");
    }

    Rectangle(Point x, Point y) : w(y.x-x.x), h(y.y-x.y)
    {
        add(x);
        if (h<=0 || w<=0) error("Bad rectangle: non-positive width or height");
    }
    void draw_lines() const;

    int height() const { return h; }
    int width() const { return w; }
private:
    int h;    // height
    int w;    // width
};

//------------------------------------------------------------------------------

struct Open_polyline : Shape {         // open sequence of lines
    void add(Point p) { Shape::add(p); }
    Open_polyline() {}
    Open_polyline(initializer_list<Point>lst) { for (auto p:lst) add(p); }
    void draw_lines() const;
};

//------------------------------------------------------------------------------

struct Closed_polyline : Open_polyline { // closed sequence of lines
    void draw_lines() const;
};

//------------------------------------------------------------------------------

struct Polygon : Closed_polyline {    // closed sequence of non-intersecting lines
    void add(Point p);
    void draw_lines() const;
};

//------------------------------------------------------------------------------

struct Lines : Shape {                 // related lines
    void draw_lines() const;
    void add(Point p1, Point p2);      // add a line defined by two points
    Lines(initializer_list<pair<Point,Point>>lst);
    Lines(){}
};

//------------------------------------------------------------------------------

struct Text : Shape {
    // the point is the bottom left of the first letter
    Text(Point x, const string& s) : lab(s), fnt(fl_font()), fnt_sz(fl_size()) { add(x); }

    void draw_lines() const;

    void set_label(const string& s) { lab = s; }
    string label() const { return lab; }

    void set_font(Font f) { fnt = f; }
    Font font() const { return Font(fnt); }

    void set_font_size(int s) { fnt_sz = s; }
    int font_size() const { return fnt_sz; }
private:
    string lab;    // label
    Font fnt;
    int fnt_sz=10;
};

//------------------------------------------------------------------------------

struct Axis : Shape {
    enum Orientation { x, y, z };
    Axis(Orientation d, Point xy, int length,
        int number_of_notches=0, string label = "");

    void draw_lines() const;
    void move(int dx, int dy);
    void set_color(Color c);

    Text label;
    Lines notches;
};

//------------------------------------------------------------------------------


struct Circle : Shape {
	Circle(Point p, int rr);	// center and radius

	void draw_lines() const;

	Point center() const;

	void set_radius(int rr) { set_point(0,Point(center().x-rr,center().y-rr)); r=rr;  }
	int radius() const { return r; }
private:
	int r;
};

//------------------------------------------------------------------------------

struct Ellipse : Shape {
	Ellipse(Point p, int ww, int hh)	// center, min, and max distance from center
		:w(ww), h(hh) { add(Point(p.x-ww,p.y-hh)); }

	void draw_lines() const;

	Point center() const { return Point(point(0).x+w,point(0).y+h); }
	Point focus1() const {
			if (h<=w)// foci are on the x-axis:
					return Point(center().x+int(sqrt(double(w*w-h*h))),center().y);
			else	// foci are on the y-axis:
					return Point(center().x,center().y+int(sqrt(double(h*h-w*w))));
	}

	Point focus2() const {
			if (h<=w)
					return Point(center().x-int(sqrt(double(w*w-h*h))),center().y);
			else
					return Point(center().x,center().y-int(sqrt(double(h*h-w*w))));
	}
	//Point focus2() const { return Point(center().x-int(sqrt(double(abs(w*w-h*h)))),center().y); }
	
	void set_major(int ww) { set_point(0,Point(center().x-ww,center().y-h)); w=ww; }
	int major() const { return w; }
	void set_minor(int hh) { set_point(0,Point(center().x-w,center().y-hh)); h=hh; }
	int minor() const { return h; }
private:
	int w;
	int h;
};

//------------------------------------------------------------------------------

struct Marked_polyline : Open_polyline {
    Marked_polyline(const string& m) :mark(m) { if (m=="") mark="*"; }
    Marked_polyline(const string& m, initializer_list<Point>lst) :mark(m), Open_polyline(lst) { if (m=="") mark="*"; }
    void draw_lines() const;
private:
    string mark;
};

//------------------------------------------------------------------------------

struct Marks : Marked_polyline {
    Marks(const string& m) :Marked_polyline(m)
    {
        set_color(Color(Color::invisible));
    }
};

//------------------------------------------------------------------------------

struct Mark : Marks {
    Mark(Point xy, char c) : Marks(string(1,c))
    {
        add(xy);
    }
};

//------------------------------------------------------------------------------

struct Suffix {
    enum Encoding { none, jpg, gif  };
};

Suffix::Encoding get_encoding(const string& s);

//------------------------------------------------------------------------------

struct Image : Shape {
    Image(Point xy, string file_name, Suffix::Encoding e = Suffix::none);
    ~Image() override { delete p; }
    void draw_lines() const override;
    void set_mask(Point xy, int ww, int hh) { w=ww; h=hh; cx=xy.x; cy=xy.y; }
private:
    int w,h;  // define "masking box" within image relative to position (cx,cy)
    int cx,cy; 
    Fl_Image* p;
    Text fn;
};

//------------------------------------------------------------------------------

struct Bad_image : Fl_Image {
    Bad_image(int h, int w) : Fl_Image(h,w,0) { }
    void draw(int x,int y, int, int, int, int) { draw_empty(x,y); }
};

//------------------------------------------------------------------------------
// 圆弧
    struct Arc : public Circle {
        Arc(Point center, int radius, int angle1, int angle2) : Circle(center, radius), a1(angle1), a2(angle2) {}
        void draw_lines() const override;

    private:
        int a1, a2;
    };

//------------------------------------------------------------------------------

// 正多边形
    struct Regular_polygon : public Polygon {
        // 正多边形
        Regular_polygon(Point center, int radius, int n, int rotate = 0);
        Point center() const { return Point(point(0).x - r, point(0).y); }
        int radius() const { return r; }

    private:
        int r;
    };

//------------------------------------------------------------------------------

// 超椭圆：|x/a|^n + |y/b|^n = 1
    struct Super_ellipse : public Shape {
        // 超椭圆：|x/a|^n + |y/b|^n = 1
        Super_ellipse(Point center, int a, int b, double n, int uniformly_spaced_interval = 1000);
    };
// 椭圆弧
    struct Elliptical_arc : Ellipse {
        Elliptical_arc(Point center, int major, int minor, int a1, int a2)
                :Ellipse(center, major, minor), a1(a1), a2(a2) {}
        void draw_lines() const override;

    private:
        int a1, a2;
    };

//------------------------------------------------------------------------------

// 圆角矩形
    struct Rounded_rectangle : Rectangle {
        Rounded_rectangle(Point p, int w, int h, int r);
        Rounded_rectangle(Point p, int w, int h) :Rounded_rectangle(p, w, h, min(w, h) / 5.0) {}

        // 由于set_color()被Marks的构造函数调用了，因此不能将其声明为虚函数，只能使用隐藏
        void set_color(Color color);
        void set_style(Line_style style);
        void set_fill_color(Color color);
        void draw_lines() const override;
        void move(int dx, int dy) override;

        int radius() const { return r; }

    private:
        Lines lines;
        Arc arc_top_right, arc_top_left, arc_bottom_left, arc_bottom_right;
        Rectangle fill_rect1, fill_rect2;
        int r;
    };

//------------------------------------------------------------------------------

// 带箭头的线
    struct Arrow : Line {
        Arrow(Point p1, Point p2);
        void set_color(Color color);
        void draw_lines() const override;
        void move(int dx, int dy) override;

    private:
        Polygon Arrow_Head;
    };

//------------------------------------------------------------------------------

// 矩形上的连接点
    Point n(const Rectangle& r);
    Point s(const Rectangle& r);
    Point e(const Rectangle& r);
    Point w(const Rectangle& r);
    Point center(const Rectangle& r);
    Point ne(const Rectangle& r);
    Point se(const Rectangle& r);
    Point sw(const Rectangle& r);
    Point nw(const Rectangle& r);

//------------------------------------------------------------------------------

// 圆上的连接点
    Point n(const Circle& c);
    Point s(const Circle& c);
    Point e(const Circle& c);
    Point w(const Circle& c);
    Point center(const Circle& c);
    Point ne(const Circle& c);
    Point se(const Circle& c);
    Point sw(const Circle& c);
    Point nw(const Circle& c);

//------------------------------------------------------------------------------

// 椭圆上的连接点
    Point n(const Ellipse& e);
    Point s(const Ellipse& e);
    Point e(const Ellipse& e);
    Point w(const Ellipse& e);
    Point center(const Ellipse& e);
    Point ne(const Ellipse& e);
    Point se(const Ellipse& e);
    Point sw(const Ellipse& e);
    Point nw(const Ellipse& e);

//------------------------------------------------------------------------------

// 带有文本标签的矩形
    struct Box : Rectangle {
    private:
        Text label;
    public:
        Box(Point p, int w, int  h, const string& text)
                :Rectangle(p, w, h), label(Point(p.x + 5, p.y + 20), text) {}
        void draw_lines() const override;
        void move(int dx, int dy) override;
        void set_text_size(int sz) {label.set_font_size(sz);}
        void set_text_style(Font f) {label.set_font(f);}
    };

//------------------------------------------------------------------------------

// 正六边形
    struct Regular_hexagon : Regular_polygon {
        Regular_hexagon(Point center, int radius) :Regular_polygon(center, radius, 6) {}
    };


//------------------------------------------------------------------------------

//柱状图
    class Column_Graph : public Shape {
    public:
        Column_Graph(
                Point top_left_point, int width, int height, const vector<double>& data,
                const vector<string>& column_data_labels, const string& title = "",
                double axis_data_min = 0, double axis_data_max = 0, int each_column_width = 20, int font_size = 14);
        void set_fill_color(Color color);
        void move(int dx, int dy) override;

    protected:
        void draw_lines() const override;

    private:
        int data_to_coordinate(double d) const;

        vector<double> data_;
        double d_min_, d_max_;

        Rectangle border_;
        Vector_ref<Rectangle> bars_;
        Vector_ref<Text> bar_labels_;
        Vector_ref<Text> data_labels_;
        Text title_;
    };

} // end of namespace Graph_lib

#endif
