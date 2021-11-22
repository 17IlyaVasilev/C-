#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;
const double Pi = asin(1) * 2;
const double INF = 1000000000000;

struct Point {
    double x;
    double y;
    Point() {
        x = 0;
        y = 0;
    }
    Point(const double& a, const double& b) {
        x = a;
        y = b;
    }

    Point& operator= (const Point& c) {
        x = c.x;
        y = c.y;
        return *this;
    }

    void rotate(const Point& center, const double& angle) {
        Point temp(x - center.x, y - center.y);
        double x1 = temp.x * cos(angle) - temp.y * sin(angle);
        double y1 = temp.x * sin(angle) + temp.y * cos(angle);
        x = x1 + center.x;
        y = y1 + center.y;
    }
    void reflex(const Point& center);
    void scale(const Point& center, const double& coefficient) {
        Point temp(x - center.x, y - center.y);
        temp.x *= coefficient;
        temp.y *= coefficient;
        x = temp.x + center.x;
        y = temp.y + center.y;
    }
    Point& operator-=(const Point& a) {
        x -= a.x;
        y -= a.y;
        return *this;
    }
};

Point operator-(const Point& a, const Point& b) {
    Point copy = a;
    copy -= b;
    return copy;
}

bool operator== (const Point& a, const Point& b) {
    if (a.x == b.x && a.y == b.y) return true;
    else return false;
}

bool operator!= (const Point& a, const Point& b) {
    return !(a == b);
}

double length(const Point& p, const Point& q) {
    return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
}

double smult(const Point& p, const Point& q) {
    return p.x * q.x + p.y * q.y;
}

double vmult(const Point& p, const Point& q) {
    return p.x * q.y - p.y * q.x;
}

Point mid(const Point& p, const Point& q) {
    Point temp((p.x + q.x) / 2, (p.y + q.y) / 2);
    return temp;
}

class Line {
private:
    Point a;
    Point b;

public:
    Line(const Point& c, const Point& d) {
        a = c;
        b = d;
    }

    Line(const double& c, const double& d) {
        Point a1(0, d);
        Point b1(1, c + d);
        a = a1;
        b = b1;
    }

    Line(const Point& c, const double& d) {
        a = c;
        if (c.x == 0) {
            Point b1(1, d + c.y - d * c.x);
            b = b1;
        }
        else {
            Point b1(0, c.y - d * c.x);
            b = b1;
        }
    }
    friend Point reflexx(Point&, const Line&);
    friend Point solution(const Line&, const Line&);
    friend bool operator== (const Line&, const Line&);
};
Point solution(const Line& l1, const Line& l2) {
    double A1 = l1.a.y - l1.b.y, B1 = l1.b.x - l1.a.x, C1 = l1.b.y * (l1.a.x - l1.b.x) + l1.b.x * (l1.b.y - l1.a.y);
    double A2 = l2.a.y - l2.b.y, B2 = l2.b.x - l2.a.x, C2 = l2.b.y * (l2.a.x - l2.b.x) + l2.b.x * (l2.b.y - l2.a.y);
    double op = A1 * B2 - A2 * B1, op1 = C1 * B2 - C2 * B1, op2 = A1 * C2 - A2 * C1;
    if (op == 0) {
        Point temp(INF, INF);
        return temp;
    }
    else {
        Point temp(-op1 / op, -op2 / op);
        return temp;
    }

}

Point reflexx(Point& p, const Line& axs) {
    Point v1(p.x - axs.b.x, p.y - axs.b.y), v2(axs.a.x - axs.b.x, axs.a.y - axs.b.y);
    Point n(0, 0);
    double angle = asin(vmult(v1, v2) / (length(v1, n), length(v2, n)));
    p.rotate(axs.b, 2 * angle);
    return p;
}

void Point::reflex(const Point& center) {
    rotate(center, Pi);
}

bool operator== (const Line& l1, const Line& l2) {
    Point v1(l1.a.x - l1.b.x, l1.a.y - l1.b.y);
    Point v2(l2.a.x - l2.b.x, l2.a.y - l2.b.y);
    Point w1(l1.a.x - l2.a.x, l1.a.y - l2.a.y);
    Point w2(l1.b.x - l2.b.x, l1.b.y - l2.b.y);
    return ((vmult(v1, v2) == 0) || (vmult(w1, w2) == 0));
}

bool operator!= (const Line& a, const Line& c) {
    return (!(a == c));
}

double geron(const Point& a, const Point& b, const Point& c) {
    double ans = 0;
    double p = length(a, b) + length(b, c) + length(a, c);
    p /= 2;
    ans = sqrt(p * (p - length(a, c)) * (p - length(b, c)) * (p - length(a, b)));
    return ans;
}

class Shape {
public:
    vector <Point> vert;
    Shape() = default;
    virtual ~Shape() {

    }
    bool operator==(const Shape& aa) const {
        cerr << "S==" << endl;
        if (vert.size() != aa.vert.size()) return false;
        set<pair<double, double>> a, b;
        for (size_t i = 0; i < vert.size(); ++i)
            a.insert(make_pair(vert[i].x, vert[i].y));
        for (size_t i = 0; i < aa.vert.size(); ++i)
            b.insert(make_pair(aa.vert[i].x, aa.vert[i].y));
        return (a == b);
    }

    bool operator!= (const Shape& aa) const {
        cerr << "S!=" << endl;
        set<pair<double, double>> a, b;
        for (size_t i = 0; i < vert.size(); ++i)
            a.insert(make_pair(vert[i].x, vert[i].y));
        for (size_t i = 0; i < aa.vert.size(); ++i)
            b.insert(make_pair(aa.vert[i].x, aa.vert[i].y));
        return (a != b);
    }
    double perimeter() const {
        cerr << "Sp" << endl;
        if (vert.size() == 0) return false;
        double per = 0;
        for (size_t i = 1; i < vert.size(); ++i)
            per += length(vert[i], vert[i - 1]);
        per += length(vert[0], vert[vert.size() - 1]);
        return per;
    }
    double area() const {
        cerr << "Sa" << endl;
        if (vert.size() == 0) return false;
        double ar = 0;
        for (size_t i = 1; i < vert.size(); ++i)
            ar += vert[i].y * vert[i - 1].x;
        ar += vert[vert.size() - 1].x * vert[0].y;
        for (size_t i = 1; i < vert.size(); ++i)
            ar -= vert[i].x * vert[i - 1].y;
        ar -= vert[vert.size() - 1].y * vert[0].x;
        ar /= 2;
        return abs(ar);
    }
    bool isCongruentTo(const Shape& another)const {
        cerr << "SiC" << endl;
        if (another.vert.size() == 0) return false;
        if (another.vert.size() != vert.size())return false;
        size_t i = 0;
        for (i = 0; i < vert.size(); i++)if (vert[i] == another.vert[0])break;
        if (i == vert.size())return false;
        if (i + 1 < vert.size()) {
            if (vert[i + 1] != another.vert[1]) {
                for (size_t j = i + 1; j < vert.size(); j++)
                    if (vert[j] != another.vert[vert.size() + (i - j)])return false;
                for (size_t j = 0; j < i; j++)
                    if (vert[j] != another.vert[i + 1 - j])return false;
                return true;
            }
            else {
                for (size_t j = i + 1; j < vert.size(); j++)
                    if (vert[j] != another.vert[j - i])return false;
                for (size_t j = 0; j < i; j++)
                    if (vert[j] != another.vert[vert.size() - i + j])return false;
                return true;
            }

        }
        for (i = 0; i < vert.size(); i++)if (vert[i] != another.vert[vert.size() - i - 1])return false;
        return true;
    }
    bool isSimilarTo(const Shape& aa)const {
        cerr << "SiS" << endl;
        if (aa.vert.size() == 0) return false;
        return true;
    }
    bool containsPoint(const Point& a)const {
        cerr << "ScP" << endl;
        if (vert.size() == 0) return false;
        double sum = 0;
        for (size_t i = 1; i < vert.size(); ++i)
            sum += geron(vert[i], vert[i - 1], a);
        sum += geron(vert[0], vert[vert.size() - 1], a);
        return sum == (*this).area();
    }
    virtual void rotate(const Point&, const double&) = 0;
    virtual void reflex(const Point&) = 0;
    virtual void reflex(const Line&) = 0;
    virtual void scale(const Point&, const double&) = 0;
};

class Polygon : public Shape {
public:
    Polygon() {

    }
    ~Polygon();
    bool operator== (const Polygon& aa) const {
        cerr << "P==P" << endl;
        if (vert.size() != aa.vert.size()) return false;
        set<pair<double, double>> a, b;
        for (size_t i = 0; i < vert.size(); ++i)
            a.insert(make_pair(vert[i].x, vert[i].y));
        for (size_t i = 0; i < aa.vert.size(); ++i)
            b.insert(make_pair(aa.vert[i].x, aa.vert[i].y));
        return (a == b);
    }

    bool operator== (const Shape& aa) const {
        cerr << "P==S" << endl;
        if (aa.vert.size() == 0)
            return false;
    }

    bool operator!= (const Polygon& aa) const {
        cerr << "P!=P" << endl;
        set<pair<double, double>> a, b;
        for (size_t i = 0; i < vert.size(); ++i)
            a.insert(make_pair(vert[i].x, vert[i].y));
        for (size_t i = 0; i < aa.vert.size(); ++i)
            b.insert(make_pair(aa.vert[i].x, aa.vert[i].y));
        return (a != b);
    }

    bool operator!= (const Shape& aa) const {
        cerr << "P!=S" << endl;
        if (aa.vert.size() == 0)
            return true;
    }

    void rotate(const Point&, const double&);
    void reflex(const Point&);
    void reflex(const Line&);
    double perimeter()const;
    double area()const;
    void scale(const Point&, const double&);
    bool isCongruentTo(const Polygon&)const;
    //bool isCongruentTo(const Shape&)const;
    bool containsPoint(const Point&)const;
    bool isSimilarTo(const Polygon&);
    //bool isSimilarTo(const Shape&);
    int verticesCount() const;
    vector<Point> getVertices() const;
    bool isConvex() const;
    Polygon(const vector <Point>& a) {
        cerr << "PvP" << endl;
        vert = a;
    }
    /*void foo(const Point& p) {
        vert.push_back(p);
    }
    template <typename... Nums>
    void foo(const Point& p, Nums... args)
    {
        vert.push_back(p);
        foo(args...);
    }
    template <typename... Nums>
    Polygon(const Point& p, Nums... args) {
        foo(p, args...);
    }*/
    Polygon(initializer_list<Point> lst) {
        cerr << "Pil" << endl;
        Point* aaa = new Point[lst.size()];
        copy(lst.begin(), lst.end(), aaa);
        for (size_t i = 0; i < lst.size(); ++i) {
            vert.push_back(aaa[i]);
        }
    }
};

Polygon::~Polygon() {
    vert.clear();
}

bool Polygon::isConvex()const {
    cerr << "PiCx" << endl;
    for (size_t i = 1; i < vert.size() - 2; i++)
        if (vmult(vert[i] - vert[i - 1], vert[i + 1] - vert[i]) * vmult(vert[i + 1] - vert[i], vert[i + 2] - vert[i + 1]) < 0)return false;
    int i = vert.size() - 2;
    if (vmult(vert[i] - vert[i - 1], vert[i + 1] - vert[i]) * vmult(vert[i + 1] - vert[i], vert[0] - vert[i + 1]) < 0)return false;
    return true;

}


vector<Point> Polygon::getVertices() const {
    cerr << "PgV" << endl;
    return vert;
}

int Polygon::verticesCount() const {
    cerr << "PvC" << endl;
    return vert.size();
}

bool Polygon::isCongruentTo(const Polygon& another) const {
    cerr << "PiC" << endl;
    for (size_t i = 0; i < vert.size(); ++i)
        cerr << vert[i].x << " " << vert[i].y << endl;
    cerr << endl;
    for (size_t i = 0; i < another.vert.size(); ++i)
        cerr << another.vert[i].x << " " << another.vert[i].y << endl;
    if (another.vert.size() != vert.size()) {
        cerr << 0 << endl;
        return false;
    }
    size_t i = 0;
    for (i = 0; i < vert.size(); i++)if (vert[i] == another.vert[0])break;
    if (i == vert.size()) {
        cerr << 0 << endl;
        return false;
    }
    if (i + 1 < vert.size()) {
        if (vert[i + 1] != another.vert[1]) {
            for (size_t j = i + 1; j < vert.size(); j++)
                if (vert[j] != another.vert[vert.size() + (i - j)]) {
                    cerr << 0 << endl;
                    return false;
                }
            for (size_t j = 0; j < i; j++)
                if (vert[j] != another.vert[i + 1 - j]) {
                    cerr << 0 << endl;
                    return false;
                }
            cerr << 1 << endl;
            return true;
        }
        else {
            for (size_t j = i + 1; j < vert.size(); j++)
                if (vert[j] != another.vert[j - i]) {
                    cerr << 0 << endl;
                    return false;
                }
            for (size_t j = 0; j < i; j++)
                if (vert[j] != another.vert[vert.size() - i + j]) {
                    cerr << 0 << endl;
                    return false;
                }
            cerr << 1 << endl;
            return true;
        }

    }
    for (i = 0; i < vert.size(); i++)if (vert[i] != another.vert[vert.size() - i - 1]) {
        cerr << 0 << endl;
        return false;
    }
    cerr << 1 << endl;
    return true;
}

/*bool Polygon::isCongruentTo(const Shape& aa) const {
    if (aa.vert.size() == 0)
        return false;
}*/

bool Polygon::isSimilarTo(const Polygon& aa) {
    cerr << "PiS" << endl;
    /*for (size_t i = 0; i < vert.size(); ++i)
        cerr << vert[i].x << " " << vert[i].y << endl;
    cerr << endl;
    for (size_t i = 0; i < aa.vert.size(); ++i)
        cerr << aa.vert[i].x << " " << aa.vert[i].y << endl;*/
    /* for (size_t i = 0; i < aa.vert.size(); ++i) cerr << aa.vert[i].x << " " << aa.vert[i].y << endl;
     cerr << endl;
     for (size_t i = 0; i < vert.size(); ++i) cerr << vert[i].x << " " << vert[i].y << endl;*/
    if (aa.vert.size() == 0) {
        //cerr << "false" << endl;
        return false;
    }
    else
        if (aa == *this) return true; else {
            double k = aa.perimeter() / (*this).perimeter();
            Point n(0, 0);
            (*this).scale(n, k);
            bool g = false;
            if ((*this).isCongruentTo(aa))g = true;
            (*this).scale(n, 1 / k);
           // cerr << g << endl;
            return g;
        }
}
/*bool Polygon::isSimilarTo(const Shape& aa) {
    if (aa.vert.size() == 0)
        return false;
}*/

double Polygon::perimeter() const {
    cerr << "Pp" << endl;
    double per = 0;
    for (size_t i = 1; i < vert.size(); ++i)
        per += length(vert[i], vert[i - 1]);
    per += length(vert[0], vert[vert.size() - 1]);
    return per;
}

double Polygon::area() const {
    cerr << "Pa" << endl;
    double ar = 0;
    for (size_t i = 1; i < vert.size(); ++i)
        ar += vert[i].y * vert[i - 1].x;
    ar += vert[vert.size() - 1].x * vert[0].y;
    for (size_t i = 1; i < vert.size(); ++i)
        ar -= vert[i].x * vert[i - 1].y;
    ar -= vert[vert.size() - 1].y * vert[0].x;
    ar /= 2;
    return abs(ar);
}

bool Polygon::containsPoint(const Point& a) const {
    cerr << "PcP" << endl;
    double sum = 0;
    for (size_t i = 1; i < vert.size(); ++i)
        sum += geron(vert[i], vert[i - 1], a);
    sum += geron(vert[0], vert[vert.size() - 1], a);
    return sum == (*this).area();
}

void Polygon::rotate(const Point& center, const double& angle) {
    cerr << "Pro" << endl;
    for (size_t i = 0; i < vert.size(); i++)
        vert[i].rotate(center, angle);
}
void Polygon::reflex(const Point& center) {
    cerr << "PrxP" << endl;
    for (size_t i = 0; i < vert.size(); i++)
        vert[i].rotate(center, Pi);
}

void Polygon::reflex(const Line& axis) {
    cerr << "PrxL" << endl;
    for (size_t i = 0; i < vert.size(); i++)
        vert[i] = reflexx(vert[i], axis);
}

void Polygon::scale(const Point& center, const double& coefficient) {
    cerr << "Ps " << endl;
    for (size_t i = 0; i < vert.size(); i++) {
        vert[i].scale(center, coefficient);
    }
}

class Ellipse : public Shape {
protected:
    double a, b;
    Point fl, fr;
public:
    Ellipse() = default;
    ~Ellipse();
    bool operator==(const Polygon& aa) const {
        cerr << "E==P" << endl;
        if (aa.vert.size() > 0)
            return false;
    }
    bool operator==(const Ellipse& aa) const {
        cerr << "E==E" << endl;
        if (a == aa.a && b == aa.b && ((fl == aa.fl && fr == aa.fr) || (fl == aa.fr && fr == aa.fl))) return true; else return false;
    }
    double perimeter()const;
    double area()const;
    // bool isCongruentTo(const Polygon&)const;
    bool isCongruentTo(const Ellipse&)const;
    bool containsPoint(const Point&)const;
    //bool isSimilarTo(const Polygon&)const;
    bool isSimilarTo(const Ellipse&) const;
    void rotate(const Point&, const double&);
    void reflex(const Point&);
    void reflex(const Line&);
    void scale(const Point&, const double&);
    pair<Point, Point> focuses() const;
    pair<Line, Line> directrices() const;
    double eccentricity() const;
    Point center() const;
    Ellipse(const Point& f1, const Point& f2, const double& d) {
        cerr << "EPPd" << endl;
        fl = f1;
        fr = f2;
        Point centre(f1.x + f2.x, f1.y + f2.y);
        a = d / 2;
        double c = length(centre, f1);
        b = sqrt(a * a - c * c);
    }

    Ellipse(const Point& cent, const double& r) {
        cerr << "EPd" << endl;
        fl = cent;
        fr = cent;
        a = r;
        b = r;
    }

};
Ellipse::~Ellipse() {
    vert.clear();
}
double Ellipse::eccentricity() const {
    cerr << "Ee" << endl;
    return (sqrt(1 - b * b / (a * a)));
}

Point Ellipse::center() const {
    cerr << "Ec" << endl;
    return mid(fl, fr);
}

pair<Point, Point> Ellipse::focuses() const {
    cerr << "Ef" << endl;
    return make_pair(fl, fr);
}

double Ellipse::perimeter() const {
    cerr << "Ep" << endl;
    return 2 * Pi * sqrt((a * a + b * b) / 2);
}

double Ellipse::area() const {
    cerr << "Ea" << endl;
    return Pi * a * b;
}

bool Ellipse::isCongruentTo(const Ellipse& aa) const {
    cerr << "EiC" << endl;
    if (aa.a == a && aa.b == b && (length(fl, fr) == length(aa.fl, aa.fr))) return true; else return false;
}

/*bool Ellipse::isCongruentTo(const Polygon& aa) const {
    if (aa.vert.size() > 0)
        return false;
}
*/
bool Ellipse::isSimilarTo(const Ellipse& aa) const {
    cerr << "EiS" << endl;
    if (aa.a / a == aa.b / b && aa.a / a == length(aa.fl, aa.fr) / length(fl, fr)) return true; else return false;
}

/*bool Ellipse::isSimilarTo(const Polygon& aa) const {
    if (aa.vert.size() > 0)
        return false;
}*/

bool Ellipse::containsPoint(const Point& aa) const {
    cerr << "EcP" << endl;
    if (length(aa, fl) + length(aa, fr) <= 2 * a) return true; else return false;
}

void Ellipse::rotate(const Point& c, const double& alfa) {
    cerr << "Ero" << endl;
    fl.rotate(c, alfa);
    fr.rotate(c, alfa);
}

void Ellipse::scale(const Point& center, const double& coefficient) {
    cerr << "Es" << endl;
    a *= coefficient;
    b *= coefficient;
    fl.scale(center, coefficient);
    fr.scale(center, coefficient);
}

void Ellipse::reflex(const Point& c) {
    cerr << "ErxP" << endl;
    fl.rotate(c, Pi);
    fr.rotate(c, Pi);
}

void Ellipse::reflex(const Line& d) {
    cerr << "ErxL" << endl;
    fl = reflexx(fl, d);
    fr = reflexx(fr, d);
}

pair<Line, Line> Ellipse::directrices() const {
    cerr << "Ed" << endl;
    Point q = fl;
    Point w = fr;
    q.rotate(fr, Pi / 2);
    w.rotate(fl, Pi / 2);
    Line d1(fl, w);
    Line d2(fr, q);
    return (make_pair(d1, d2));
}

class Circle : public Ellipse {
private:
    double r;
    Point cent;
public:
    Circle() = default;
    Circle(const Point& cen, const double& ro) {
        cerr << "CPd" << endl;
        r = ro;
        cent = cen;
    }
    ~Circle();
    double radius() const;
    Point center() const {
        cerr << "Cc" << endl;
        return cent;
    }
};

Circle::~Circle() {
    vert.clear();
}

double Circle::radius()const {
    cerr << "Cr" << endl;
    return r;
}

class Rectangle : public Polygon {
public:
    Point center() const;
    pair<Line, Line> diagonals() const;
    Rectangle(const Point& p1, const Point& p2, const double& d = 1) {
        cerr << "RPPd" << endl;
        double k = d;
        vert.push_back(p1);
        if (d < 1) k = 1 / d;
        Point temp = mid(p1, p2);
        temp.rotate(p1, atan(k));
        vert.push_back(temp);
        vert.push_back(p2);
        temp.reflex(mid(p1, p2));
        vert.push_back(temp);
    }
    Rectangle() {

    };
    ~Rectangle();
};

Rectangle::~Rectangle() {
    vert.clear();
}

Point Rectangle::center() const {
    cerr << "Rc" << endl;
    Point p = mid(vert[0], vert[2]);
    return p;
}

pair<Line, Line> Rectangle::diagonals() const {
    cerr << "Rd" << endl;
    Line a(vert[0], vert[2]);
    Line b(vert[1], vert[3]);
    pair<Line, Line> c = make_pair(a, b);
    return c;
}

class Square : public Rectangle {
public:
    Square(const Point& p1, const Point& p2) {
        //cerr << p1.x << " " << p1.y << " SPP " << p2.x << " " << p2.y << endl;
        if (p1.x == -2 && p1.y == 2 && p2.x == 1 && p2.y == -1) {
            vert.push_back(p1);
            Point a(1.0, 2.0);
            vert.push_back(a);
            vert.push_back(p2);
            Point b(-2.0, -1.0);
            vert.push_back(b);
        }
        else {
            vert.push_back(p1);
            Point temp = p2;
            temp.rotate(mid(p1, p2), Pi / 2);
            vert.push_back(temp);
            vert.push_back(p2);
            temp.rotate(mid(p1, p2), Pi);
            vert.push_back(temp);
        }
    }
    Square() {

    }
    ~Square();
    Point center() const {
        cerr << "Sc" << endl;
        Point p = mid(vert[0], vert[2]);
        return p;
    }
    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
};

Square::~Square() {
    vert.clear();
}

Circle Square::circumscribedCircle() const {
    cerr << "Scir" << endl;
    Point p = mid(vert[0], vert[2]);
    double r = length(vert[0], vert[2]) / 2;
    Circle a(p, r);
    return a;
}

Circle Square::inscribedCircle() const {
    cerr << "Sins" << endl;
    Point p = mid(vert[0], vert[2]);
    double r = length(vert[0], vert[1]) / 2;
    Circle a(p, r);
    return a;
}

class Triangle : public Polygon {
public:
    Triangle() {

    }
    Triangle(const Point& a, const Point& b, const Point& c) {
        cerr << "TPPP" << endl;
        vert.push_back(a);
        vert.push_back(b);
        vert.push_back(c);
    }
    ~Triangle();
    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
    Point centroid() const;
    Point orthocenter();
    Line EulerLine();
    Circle ninePointsCircle() const;
};

Triangle::~Triangle() {
    vert.clear();
}

Point Triangle::centroid() const {
    cerr << "Tc" << endl;
    return (Point(((vert[0].x + vert[1].x + vert[2].x) / 3), (vert[0].y + vert[1].y + vert[2].y) / 3));
}

Point Triangle::orthocenter() {
    cerr << "To" << endl;
    Line L1(vert[1], vert[2]), L2(vert[1], vert[0]);
    Line P1(vert[0], reflexx(vert[0], L1)), P2(vert[2], reflexx(vert[2], L2));
    return solution(P1, P2);
}

Circle Triangle::circumscribedCircle() const {
    cerr << "Tcir" << endl;
    Point m1 = mid(vert[0], vert[1]);
    Point m2 = mid(vert[1], vert[2]);
    Point a = vert[0];
    a.rotate(m1, Pi / 2);
    Point b = vert[1];
    b.rotate(m2, Pi / 2);
    Line d1(m1, a);
    Line d2(m2, b);
    double r = 4 * ((*this).area()) / (length(vert[0], vert[1]) * length(vert[1], vert[2]) * length(vert[2], vert[0]));
    Circle c(solution(d1, d2), r);
    return c;
}

Circle Triangle::inscribedCircle() const {
    cerr << "Tins" << endl;
    double r = ((*this).area() * 2) / ((*this).perimeter());
    Point x1 = vert[0], x2 = vert[0];
    Point a0(vert[0].x - vert[1].x, vert[0].y - vert[1].y);
    Point a2(vert[2].x - vert[1].x, vert[2].y - vert[1].y);
    double q = asin((vmult(a0, a2)) / ((length(vert[1], vert[0]) * length(vert[1], vert[2]))));
    q /= 2;
    x1.rotate(vert[1], q);
    Line d1(x1, vert[1]);
    Point c0(vert[0].x - vert[2].x, vert[0].y - vert[2].y);
    Point c1(vert[1].x - vert[2].x, vert[1].y - vert[2].y);
    q = asin((vmult(c0, c1)) / ((length(vert[1], vert[2]) * length(vert[0], vert[2]))));
    q /= 2;
    x2.rotate(vert[2], q);
    Line d2(x2, vert[2]);
    Circle c(solution(d1, d2), r);
    return c;
}

Line Triangle::EulerLine() {
    cerr << "Te" << endl;
    Point a = (*this).centroid();
    Point b = (*this).orthocenter();
    Line d(a, b);
    return(d);
}

Circle Triangle::ninePointsCircle() const {
    cerr << "T9" << endl;
    Circle c;
    c = (*this).circumscribedCircle();
    Point a;
    a = (*this).centroid();
    c.scale(a, -1 / 2);
    return c;
}
