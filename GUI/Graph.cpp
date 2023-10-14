
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include <FL/Fl_GIF_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include "Graph.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

//------------------------------------------------------------------------------

Shape::Shape() : 
    lcolor(fl_color()),      // default line_size for lines and characters
    ls(0),                   // default style
    fcolor(Color::invisible) // no fill
{}

//------------------------------------------------------------------------------

void Shape::add(Point p)     // protected
{
    points.push_back(p);
}

//------------------------------------------------------------------------------

void Shape::set_point(int i,Point p)        // not used; not necessary so far
{
    points[i] = p;
}

//------------------------------------------------------------------------------

void Shape::draw_lines() const
{
    if (color().visibility() && 1<points.size())    // draw sole pixel?
        for (unsigned int i=1; i<points.size(); ++i)
            fl_line(points[i-1].x,points[i-1].y,points[i].x,points[i].y);
}

//------------------------------------------------------------------------------

void Shape::draw() const
{
    Fl_Color oldc = fl_color();
    // there is no good portable way of retrieving the current style
    fl_color(lcolor.as_int());            // set line_size
    fl_line_style(ls.style(),ls.width()); // set style
    draw_lines();
    fl_color(oldc);      // reset line_size (to previous)
    fl_line_style(0);    // reset line style to default
}

//------------------------------------------------------------------------------


void Shape::move(int dx, int dy)    // move the shape +=dx and +=dy
{
    for (unsigned int i = 0; i<points.size(); ++i) {
        points[i].x+=dx;
        points[i].y+=dy;
    }
}

//------------------------------------------------------------------------------

Line::Line(Point p1, Point p2)    // construct a line from two points
{
    add(p1);    // add p1 to this shape
    add(p2);    // add p2 to this shape
}

//------------------------------------------------------------------------------

void Lines::add(Point p1, Point p2)
{
    Shape::add(p1);
    Shape::add(p2);
}

//------------------------------------------------------------------------------

// draw lines connecting pairs of points
void Lines::draw_lines() const
{
    if (color().visibility())
        for (int i=1; i<number_of_points(); i+=2)
            fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);
}

Lines::Lines(initializer_list<pair<Point, Point>> lst) {
    for (auto p : lst) add(p.first,p.second);
}

//------------------------------------------------------------------------------

// does two lines (p1,p2) and (p3,p4) intersect?
// if se return the distance of the intersect point as distances from p1
inline pair<double,double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool& parallel) 
{
    double x1 = p1.x;
    double x2 = p2.x;
    double x3 = p3.x;
    double x4 = p4.x;
    double y1 = p1.y;
    double y2 = p2.y;
    double y3 = p3.y;
    double y4 = p4.y;

    double denom = ((y4 - y3)*(x2-x1) - (x4-x3)*(y2-y1));
    if (denom == 0){
        parallel= true;
        return pair<double,double>(0,0);
    }
    parallel = false;
    return pair<double,double>( ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3))/denom,
                                ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3))/denom);
}

//------------------------------------------------------------------------------

//intersection between two line segments
//Returns true if the two segments intersect,
//in which case intersection is set to the point of intersection
bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection){
   bool parallel;
   pair<double,double> u = line_intersect(p1,p2,p3,p4,parallel);
   if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1) return false;
   intersection.x = p1.x + lround(u.first*(p2.x - p1.x));
   intersection.y = p1.y + lround(u.first*(p2.y - p1.y));
   return true;
}

//------------------------------------------------------------------------------

void Polygon::add(Point p)
{
    int np = number_of_points();

    if (1<np) {    // check that thenew line isn't parallel to the previous one
        if (p==point(np-1)) error("polygon point equal to previous point");
        bool parallel;
        line_intersect(point(np-1),p,point(np-2),point(np-1),parallel);
        if (parallel)
            error("two polygon points lie in a straight line");
    }

    for (int i = 1; i<np-1; ++i) {    // check that new segment doesn't interset and old point
        Point ignore(0,0);
        if (line_segment_intersect(point(np-1),p,point(i-1),point(i),ignore))
            error("intersect in polygon");
    }
    

    Closed_polyline::add(p);
}

//------------------------------------------------------------------------------

void Polygon::draw_lines() const
{
    if (number_of_points() < 3) error("less than 3 points in a Polygon");
    Closed_polyline::draw_lines();
}

//------------------------------------------------------------------------------

void Open_polyline::draw_lines() const
{
    if (fill_color().visibility()) {
        fl_color(fill_color().as_int());
        fl_begin_complex_polygon();
        for(int i=0; i<number_of_points(); ++i){
            fl_vertex(point(i).x, point(i).y);
        }
        fl_end_complex_polygon();
        fl_color(color().as_int());    // reset line_size
    }
    
    if (color().visibility())
        Shape::draw_lines();
}

//------------------------------------------------------------------------------

void Closed_polyline::draw_lines() const
{
    Open_polyline::draw_lines();    // first draw the "open poly line part"
    // then draw closing line:
    if (color().visibility())
        fl_line(point(number_of_points()-1).x, 
        point(number_of_points()-1).y,
        point(0).x,
        point(0).y);
}

//------------------------------------------------------------------------------

void draw_mark(Point xy, char c)
{
    static const int dx = 4;
    static const int dy = 4;

    string m(1,c);
    fl_draw(m.c_str(),xy.x-dx,xy.y+dy);
}

//------------------------------------------------------------------------------

void Marked_polyline::draw_lines() const
{
    Open_polyline::draw_lines();
    for (int i=0; i<number_of_points(); ++i) 
        draw_mark(point(i),mark[i%mark.size()]);
}

//------------------------------------------------------------------------------

void Rectangle::draw_lines() const
{
    if (fill_color().visibility()) {    // fill
        fl_color(fill_color().as_int());
        fl_rectf(point(0).x,point(0).y,w,h);
        fl_color(color().as_int());    // reset line_size
    }

    if (color().visibility()) {    // lines on top of fill
        fl_color(color().as_int());
        fl_rect(point(0).x,point(0).y,w,h);
    }
}

//------------------------------------------------------------------------------

Circle::Circle(Point p, int rr)    // center and radius
:r(rr)
{
    add(Point(p.x-r,p.y-r));       // store top-left corner
}

//------------------------------------------------------------------------------

Point Circle::center() const
{
    return Point(point(0).x+r, point(0).y+r);
}

//------------------------------------------------------------------------------

void Circle::draw_lines() const
{
  	if (fill_color().visibility()) {	// fill
		fl_color(fill_color().as_int());
		fl_pie(point(0).x,point(0).y,r+r-1,r+r-1,0,360);
		fl_color(color().as_int());	// reset line_size
	}

	if (color().visibility()) {
		fl_color(color().as_int());
		fl_arc(point(0).x,point(0).y,r+r,r+r,0,360);
	}
}

//------------------------------------------------------------------------------

void Ellipse::draw_lines() const
{
   if (fill_color().visibility()) {	// fill
		fl_color(fill_color().as_int());
		fl_pie(point(0).x,point(0).y,w+w-1,h+h-1,0,360);
		fl_color(color().as_int());	// reset line_size
	}

	if (color().visibility()) {
		fl_color(color().as_int());
		fl_arc(point(0).x,point(0).y,w+w,h+h,0,360);
	}
}

//------------------------------------------------------------------------------

void Text::draw_lines() const
{
    int ofnt = fl_font();
    int osz = fl_size();
    fl_font(fnt.as_int(),fnt_sz);
    fl_draw(lab.c_str(),point(0).x,point(0).y);
    fl_font(ofnt,osz);
}

//------------------------------------------------------------------------------

Axis::Axis(Orientation d, Point xy, int length, int n, string lab) :
    label(Point(0,0),lab)
{
    if (length<0) error("bad axis length");
    switch (d){
    case Axis::x:
    {
        Shape::add(xy); // axis line
        Shape::add(Point(xy.x+length,xy.y));

        if (0<n) {      // add notches
            int dist = length/n;
            int x = xy.x+dist;
            for (int i = 0; i<n; ++i) {
                notches.add(Point(x,xy.y),Point(x,xy.y-5));
                x += dist;
            }
        }
        // label under the line
        label.move(length/3,xy.y+20);
        break;
    }
    case Axis::y:
    {
        Shape::add(xy); // a y-axis goes up
        Shape::add(Point(xy.x,xy.y-length));

        if (0<n) {      // add notches
            int dist = length/n;
            int y = xy.y-dist;
            for (int i = 0; i<n; ++i) {
                notches.add(Point(xy.x,y),Point(xy.x+5,y));
                y -= dist;
            }
        }
        // label at top
        label.move(xy.x-10,xy.y-length-10);
        break;
    }
    case Axis::z:
        error("z axis not implemented");
    }
}

//------------------------------------------------------------------------------

void Axis::draw_lines() const
{
    Shape::draw_lines();
    notches.draw();  // the notches may have a different line_size from the line
    label.draw();    // the label may have a different line_size from the line
}

//------------------------------------------------------------------------------

void Axis::set_color(Color c)
{
    Shape::set_color(c);
    notches.set_color(c);
    label.set_color(c);
}

//------------------------------------------------------------------------------

void Axis::move(int dx, int dy)
{
    Shape::move(dx,dy);
    notches.move(dx,dy);
    label.move(dx,dy);
}

//------------------------------------------------------------------------------

Function::Function(Fct f, double r1, double r2, Point xy,
                   int count, double xscale, double yscale)
// graph f(x) for x in [r1:r2) using count line segments with (0,0) displayed at xy
// x coordinates are scaled by xscale and y coordinates scaled by yscale
{
    if (r2-r1<=0) error("bad graphing range");
    if (count <=0) error("non-positive graphing count");
    double dist = (r2-r1)/count;
    double r = r1;
    for (int i = 0; i<count; ++i) {
        add(Point(xy.x+int(r*xscale),xy.y-int(f(r)*yscale)));
        r += dist;
    }
}

//------------------------------------------------------------------------------

bool can_open(const string& s)
// check if a file named s exists and can be opened for reading
{
    ifstream ff(s.c_str());
    return bool(ff);
}

//------------------------------------------------------------------------------

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

Suffix::Encoding get_encoding(const string& s)
{
    struct SuffixMap 
    { 
        const char*      extension;
        Suffix::Encoding suffix;
    };

    static SuffixMap smap[] = {
        {".jpg",  Suffix::jpg},
        {".jpeg", Suffix::jpg},
        {".gif",  Suffix::gif},
    };

    for (int i = 0, n = ARRAY_SIZE(smap); i < n; i++)
    {
        unsigned int len = strlen(smap[i].extension);

        if (s.length() >= len && s.substr(s.length()-len, len) == smap[i].extension)
            return smap[i].suffix;
    }

    return Suffix::none;
}

//------------------------------------------------------------------------------

// somewhat over-elaborate constructor
// because errors related to image files can be such a pain to debug
Image::Image(Point xy, string s, Suffix::Encoding e)
    :w(0), h(0), fn(xy,"")
{
    add(xy);

    if (!can_open(s)) {    // can we open s?
        fn.set_label("cannot open \""+s+'\"');
        p = new Bad_image(30,20);    // the "error image"
        return;
    }

    if (e == Suffix::none) e = get_encoding(s);

    switch(e) {        // check if it is a known encoding
    case Suffix::jpg:
        p = new Fl_JPEG_Image(s.c_str());
        break;
    case Suffix::gif:
        p = new Fl_JPEG_Image(s.c_str());
        break;
    default:    // Unsupported image encoding
        fn.set_label("unsupported file type \""+s+'\"');
        p = new Bad_image(30,20);    // the "error image"
    }
}

//------------------------------------------------------------------------------

void Image::draw_lines() const
{
    if (fn.label()!="") fn.draw_lines();

    if (w&&h)
        p->draw(point(0).x,point(0).y,w,h,cx,cy);
    else
        p->draw(point(0).x,point(0).y);
}

//------------------------------------------------------------------------------
    void Elliptical_arc::draw_lines() const {
        if (fill_color().visibility()) {    // fill
            fl_color(fill_color().as_int());
            fl_pie(point(0).x, point(0).y, 2 * major() - 1, 2 * minor() - 1, a1, a2);
            fl_color(color().as_int());    // reset line_size
        }
        if (color().visibility()) {
            fl_color(color().as_int());
            fl_arc(point(0).x, point(0).y, 2 * major(), 2 * minor(), a1, a2);
        }
    }

    Rounded_rectangle::Rounded_rectangle(Point p, int w, int h, int r)
            :Rectangle(p, w, h),
             lines{
                     {{p.x + r, p.y}, {p.x + w - r, p.y}},
                     {{p.x + r, p.y + h}, {p.x + w - r, p.y + h}},
                     {{p.x, p.y + r}, {p.x, p.y + h - r}},
                     {{p.x + w, p.y + r}, {p.x + w, p.y + h - r}}
             },
             arc_top_right(Point(p.x + w - r, p.y + r), r, 0, 90),
             arc_top_left(Point(p.x + r, p.y + r), r, 90, 180),
             arc_bottom_left(Point(p.x + r, p.y + h - r), r, 180, 270),
             arc_bottom_right(Point(p.x + w - r, p.y + h - r), r, 270, 360),
             fill_rect1(Point(p.x, p.y + r), w, h - 2 * r),
             fill_rect2(Point(p.x + r, p.y), w - 2 * r, h) {
        if (r <= 0 || 2 * r > min(w, h))
            error("Bad rounded rectangle: invalid radius");
        fill_rect1.set_color(Color::invisible);
        fill_rect2.set_color(Color::invisible);
    }

//------------------------------------------------------------------------------

    void Rounded_rectangle::set_color(Color color) {
        Shape::set_color(color);
        lines.set_color(color);
        arc_top_right.set_color(color);
        arc_top_left.set_color(color);
        arc_bottom_left.set_color(color);
        arc_bottom_right.set_color(color);
    }

//------------------------------------------------------------------------------

    void Rounded_rectangle::set_style(Line_style style) {
        Shape::set_style(style);
        lines.set_style(style);
        arc_top_right.set_style(style);
        arc_top_left.set_style(style);
        arc_bottom_left.set_style(style);
        arc_bottom_right.set_style(style);
    }

//------------------------------------------------------------------------------

    void Rounded_rectangle::set_fill_color(Color color) {
        Shape::set_fill_color(color);
        arc_top_right.set_fill_color(color);
        arc_top_left.set_fill_color(color);
        arc_bottom_left.set_fill_color(color);
        arc_bottom_right.set_fill_color(color);
        fill_rect1.set_fill_color(color);
        fill_rect2.set_fill_color(color);
    }

//------------------------------------------------------------------------------

    void Rounded_rectangle::draw_lines() const {
        fill_rect1.draw();
        fill_rect2.draw();
        arc_top_right.draw();
        arc_top_left.draw();
        arc_bottom_left.draw();
        arc_bottom_right.draw();
        lines.draw();
    }

//------------------------------------------------------------------------------

    void Rounded_rectangle::move(int dx, int dy) {
        Shape::move(dx, dy);
        lines.move(dx, dy);
        arc_top_right.move(dx, dy);
        arc_top_left.move(dx, dy);
        arc_bottom_left.move(dx, dy);
        arc_bottom_right.move(dx, dy);
        fill_rect1.move(dx, dy);
        fill_rect2.move(dx, dy);
    }

//------------------------------------------------------------------------------

    Arrow::Arrow(Point p1, Point p2) :Line(p1, p2) {
        if (p1 == p2)
            error("Bad arrow: two end points can't be the same");
        Arrow_Head.add(p2);

        const int w = 2, h = 5;     // 箭头小三角形底边的一半和高
        if (p1.x == p2.x) {
            int y = p2.y > p1.y ? p2.y - h : p2.y + h;
            Arrow_Head.add(Point(p1.x + w, y));
            Arrow_Head.add(Point(p1.x - w, y));
        }
        else {
            double k = double(p2.y - p1.y) / (p2.x - p1.x), d = sqrt(1 + k * k);
            double x0 = p2.x > p1.x ? p2.x - h / d : p2.x + h / d;  // 箭头小三角形底边中点坐标
            double y0 = p2.x > p1.x ? p2.y - h * k / d : p2.y + h * k / d;
            Arrow_Head.add(Point(x0 + w * k / d, y0 - w / d));   // 箭头小三角形底边端点坐标
            Arrow_Head.add(Point(x0 - w * k / d, y0 + w / d));
        }
        Arrow_Head.set_fill_color(color());
    }

//------------------------------------------------------------------------------

    void Arrow::set_color(Color color) {
        Shape::set_color(color);
        Arrow_Head.set_color(color);
        Arrow_Head.set_fill_color(color);
    }

//------------------------------------------------------------------------------

    void Arrow::draw_lines() const {
        Shape::draw_lines();
        Arrow_Head.draw();
    }

//------------------------------------------------------------------------------

    void Arrow::move(int dx, int dy) {
        Shape::move(dx, dy);
        Arrow_Head.move(dx, dy);
    }

//------------------------------------------------------------------------------

    Point n(const Rectangle& r) {
        return Point(r.point(0).x + r.width() / 2, r.point(0).y);
    }

    Point s(const Rectangle& r) {
        return Point(r.point(0).x + r.width() / 2, r.point(0).y + r.height());
    }

    Point e(const Rectangle& r) {
        return Point(r.point(0).x + r.width(), r.point(0).y + r.height() / 2);
    }

    Point w(const Rectangle& r) {
        return Point(r.point(0).x, r.point(0).y + r.height() / 2);
    }

    Point center(const Rectangle& r) {
        return Point(r.point(0).x + r.width() / 2, r.point(0).y + r.height() / 2);
    }

    Point ne(const Rectangle& r) {
        return Point(r.point(0).x + r.width(), r.point(0).y);
    }

    Point se(const Rectangle& r) {
        return Point(r.point(0).x + r.width(), r.point(0).y + r.height());
    }

    Point sw(const Rectangle& r) {
        return Point(r.point(0).x, r.point(0).y + r.height());
    }

    Point nw(const Rectangle& r) {
        return r.point(0);
    }

    Point n(const Circle& c) {
        return Point(c.center().x, c.center().y - c.radius());
    }

    Point s(const Circle& c) {
        return Point(c.center().x, c.center().y + c.radius());
    }

    Point e(const Circle& c) {
        return Point(c.center().x + c.radius(), c.center().y);
    }

    Point w(const Circle& c) {
        return Point(c.center().x - c.radius(), c.center().y);
    }

    Point center(const Circle& c) {
        return c.center();
    }

    const double SQRT2 = sqrt(2);

    Point ne(const Circle& c) {
        return Point(c.center().x + c.radius() / SQRT2, c.center().y - c.radius() / SQRT2);
    }

    Point se(const Circle& c) {
        return Point(c.center().x + c.radius() / SQRT2, c.center().y + c.radius() / SQRT2);
    }

    Point sw(const Circle& c) {
        return Point(c.center().x - c.radius() / SQRT2, c.center().y + c.radius() / SQRT2);
    }

    Point nw(const Circle& c) {
        return Point(c.center().x - c.radius() / SQRT2, c.center().y - c.radius() / SQRT2);
    }

    Point n(const Ellipse& e) {
        return Point(e.center().x, e.center().y - e.minor());
    }

    Point s(const Ellipse& e) {
        return Point(e.center().x, e.center().y + e.minor());
    }

    Point e(const Ellipse& e) {
        return Point(e.center().x + e.major(), e.center().y);
    }

    Point w(const Ellipse& e) {
        return Point(e.center().x - e.major(), e.center().y);
    }

    Point center(const Ellipse& e) {
        return e.center();
    }

    Point ne(const Ellipse& e) {
        return Point(e.center().x + e.major() / SQRT2, e.center().y - e.minor() / SQRT2);
    }

    Point se(const Ellipse& e) {
        return Point(e.center().x + e.major() / SQRT2, e.center().y + e.minor() / SQRT2);
    }

    Point sw(const Ellipse& e) {
        return Point(e.center().x - e.major() / SQRT2, e.center().y + e.minor() / SQRT2);
    }

    Point nw(const Ellipse& e) {
        return Point(e.center().x - e.major() / SQRT2, e.center().y - e.minor() / SQRT2);
    }

//------------------------------------------------------------------------------

    void Box::draw_lines() const {
        Rectangle::draw_lines();
        label.draw();
    }

//------------------------------------------------------------------------------

    void Box::move(int dx, int dy) {
        Shape::move(dx, dy);
        label.move(dx, dy);
    }

    void Arc::draw_lines() const {
        if (fill_color().visibility()) {    // fill
            fl_color(fill_color().as_int());
            fl_pie(point(0).x, point(0).y, 2 * radius() - 1, 2 * radius() - 1, a1, a2);
            fl_color(color().as_int());    // reset line_size
        }
        if (color().visibility()) {
            fl_color(color().as_int());
            fl_arc(point(0).x, point(0).y, 2 * radius(), 2 * radius(), a1, a2);
        }
    }

//------------------------------------------------------------------------------

    Regular_polygon::Regular_polygon(Point center, int radius, int n, int rotate) :r(radius) {
        if (n < 3)
            error("Bad regular polygon: less than 3 points in a Polygon");
        if (radius <= 0)
            error("Bad regular polygon: non-positive radius");

        const double pi = 4 * atan(1);
        for (int k = 0; k < n; ++k) {
            double theta = 2 * k * pi / n + rotate * pi / 180;
            Closed_polyline::add(Point(center.x + radius * cos(theta), center.y - radius * sin(theta)));
        }
    }

//------------------------------------------------------------------------------

    Super_ellipse::Super_ellipse(Point center, int a, int b, double n, int uniformly_spaced_interval) {
        if (a <= 0 || b <= 0 || n <= 0 || uniformly_spaced_interval <= 0)
            error("Bad super ellipse: invalid argument");

        // x = a(cos t)^(2/n), y = b(sin t)^(2/n)
        const double pi = 3.14159265358, delta = pi / (2 * uniformly_spaced_interval);
        double theta = 0;
        vector<Point> vp1, vp2, vp3, vp4;
        for (int i = 0; i < uniformly_spaced_interval; ++i, theta += delta) {
            double x = a * pow(cos(theta), 2.0 / n), y = b * pow(sin(theta), 2.0 / n);
            vp1.emplace_back(center.x + x, center.y + y);
            vp2.emplace_back(center.x - x, center.y + y);
            vp3.emplace_back(center.x - x, center.y - y);
            vp4.emplace_back(center.x + x, center.y - y);
        }

        for (const Point& p : vp1)
            add(p);
        for (auto it = vp2.rbegin(); it != vp2.rend(); ++it)
            add(*it);
        for (const Point& p : vp3)
            add(p);
        for (auto it = vp4.rbegin(); it != vp4.rend(); ++it)
            add(*it);
    }


//------------------------------------------------------------------------------

    Column_Graph::Column_Graph(
            Point top_left_point, int width, int height, const vector<double>& data,
            const vector<string>& labels, const string& title,
            double d_min, double d_max, int bar_width, int font_size)
            :data_(data),
             border_(top_left_point, width, height),
             title_(Point(top_left_point.x + 50, top_left_point.y - 20), title) {
        if (data.empty())
            error("Column_Graph: data is empty");
        if (data.size() != labels.size())
            error("Column_Graph: data and column_data_labels have different sizes");
        if (bar_width <= 0 || width < bar_width * (data.size() + 1))
            error("Column_Graph: invalid bar width");

        d_min_ = min(d_min, *min_element(data.begin(), data.end()));
        d_max_ = max(d_max, *max_element(data.begin(), data.end()));
        title_.set_font_size(font_size);

        int y_bottom = top_left_point.y + height;
        for (int i = 0; i < data.size(); ++i) {
            int x = top_left_point.x + (i + 1) * width / (data.size() + 1);
            int y = data_to_coordinate(data[i]);

            Rectangle* bar = new Rectangle(Point(x - bar_width / 2, y), Point(x + bar_width / 2, y_bottom));
            bar->set_color(Color::invisible);
            bar->set_fill_color(Color::black);
            bars_.push_back(bar);

            Text* bar_label = new Text(Point(x - bar_width / 2, y_bottom + 20), labels[i]);
            bar_label->set_font_size(font_size);
            bar_labels_.push_back(bar_label);

            std::ostringstream oss;
            oss << data[i];
            Text* data_label = new Text(Point(x - bar_width / 2, y - 20), oss.str());
            data_label->set_font_size(font_size);
            data_labels_.push_back(data_label);
        }
    }

    void Column_Graph::set_fill_color(Color color) {
        for (int i = 0; i < bars_.size(); ++i)
            bars_[i].set_fill_color(color);
    }

    void Column_Graph::move(int dx, int dy) {
        error("Column_Graph is too heavy to move");
    }

    void Column_Graph::draw_lines() const {
        for (int i = 0; i < bars_.size(); ++i) {
            bars_[i].draw();
            bar_labels_[i].draw();
            data_labels_[i].draw();
        }
        border_.draw();
        title_.draw();
    }

    int Column_Graph::data_to_coordinate(double d) const {
        int y_bottom = border_.point(0).y + border_.height();
        // (d - axis_data_min) / (axis_data_max - axis_data_min) = (y - y_bottom) / (y_top - y_bottom) = -(y - y_bottom) / height
        return y_bottom - max(1, int(border_.height() * (d - d_min_) / (d_max_ - d_min_)));
    }

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------






}

// end of namespace Graph_lib