#pragma once

#include <vector>
#include <cmath>


const double EPS = 1e-9;


struct Point {
    double x;
    double y;

    Point() : x(0), y(0) {}

    Point(double x, double y) : x(x), y(y) {}

    bool operator==(const Point& rhs) const {
        return ((abs(this->x - rhs.x) < EPS) and (abs(this->y - rhs.y) < EPS));
    }

    bool operator!=(const Point& rhs) const {
        return !(*this == rhs);
    }
};


double calcSqrSum(const double& x, const double& y) {  // calculate sum of squares
    return x * x + y * y;
}


double calcDistance(const Point& p1, const Point& p2) {  // calculate distance between two points
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}


class Line {
public:
    Line(const Point &p1, const Point &p2) {  // (y2 - y1) * x + (x1 - x2) * y + (y1 * x2 - x1 * y2) = 0
        this->a = p2.y - p1.y;
        this->b = p1.x - p2.x;
        this->c = p1.y * p2.x - p1.x * p2.y;
    }

    Line(double k, double b) : a(k), b(-1), c(b) {}  // k * x - y + b = 0

    Line(const Point &p, double k) {  // y = k * x + b
        this->a = k;
        this->b = -1;
        this->c = p.y - k * p.x;
    }

    std::vector<double> getLineCoeffs() const {
        std::vector<double> coeffs{a, b, c};
        return coeffs;
    }

    bool operator==(const Line &rhs) const {
        double cf1 = abs(rhs.a - 0.0) >= EPS ? this->a / rhs.a : this->b / rhs.b;
        double cf2 = abs(rhs.b - 0.0) >= EPS ? this->b / rhs.b : cf1;
        double cf3 = abs(rhs.c - 0.0) >= EPS ? this->c / rhs.c : cf1;
        return (abs(cf1 / cf2 - 1.0) < EPS) and (abs(cf1 / cf3 - 1.0) < EPS);
    }

    bool operator!=(const Line &rhs) const {
        return !(*this == rhs);
    }

private:
    double a, b, c;  // line coefficients: a * x + b * y + c = 0
};


class Shape {
public:
    virtual double perimeter() const { return 0; }
    virtual double area() const { return 0; }
    virtual bool operator==(const Shape& rhs) { return false; }
    virtual bool operator!=(const Shape& rhs) { return false; }
    virtual void rotate(Point center, double angle) {}
    virtual void reflex(Point center) {}
    virtual void reflex(Line axis) {}
    virtual void scale(Point center, double coeff) {}
};


double rotateX (const double& x, const double& y, const double& angle) {
    return x * cos(angle) - y * sin(angle);  // x coordinate after rotation
}


double rotateY (const double& x, const double& y, const double& angle) {
    return x * sin(angle) + y * cos(angle);  // y coordinate after rotation
}


class Polygon: public Shape {
public:
    explicit Polygon(std::vector<Point> points) {
        this->vertices = std::move(points);
    }

    size_t verticesCount() const {
        return this->vertices.size();
    }

    const std::vector<Point>& getVertices() const {
        return this->vertices;
    }

    double perimeter() const override {
        double res = 0;
        for (size_t i = 0; i < this->vertices.size(); ++i) {
            res += calcDistance(this->vertices[i], this->vertices[(i + 1) % this->vertices.size()]);
        }
        return res;
    }

    double area() const override {
        double res = 0;
        for (size_t i = 1; i < this->verticesCount() - 1; ++i) {
            double c1 = (this->vertices[i].x - this->vertices[0].x) * (this->vertices[i + 1].y - this->vertices[0].y);
            double c2 = (this->vertices[i + 1].x - this->vertices[0].x) * (this->vertices[i].y - this->vertices[0].y);
            res += 0.5 * abs(c1 - c2);
        }
        return res;
    }

    bool operator==(const Polygon& rhs) {
        if (this->vertices.size() != rhs.vertices.size()) {
            return false;
        }
        size_t vcnt = rhs.vertices.size();
        size_t j = 0;
        for (size_t i = 0; i < vcnt; ++i) {
            if (this->vertices[0] != rhs.vertices[j]) {
                j += 1;
            }
        }
        for (size_t i = 0; i < vcnt; ++i) {
            if ((this->vertices[i] != rhs.vertices[(j + i) % vcnt]) and
                (this->vertices[i] != rhs.vertices[(j - i + vcnt) % vcnt])) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Polygon& rhs) {
        return !(*this == rhs);
    }

    void rotate(Point center, double angle) override {
        for (size_t i = 0; i < this->verticesCount(); ++i) {
            vertices[i].x -= center.x;
            vertices[i].y -= center.y;
        }
        for (size_t i = 0; i < this->verticesCount(); ++i) {
            double x1 = rotateX(vertices[i].x, vertices[i].y, angle);
            double y1 = rotateY(vertices[i].x, vertices[i].y, angle);
            vertices[i].x = x1;
            vertices[i].y = y1;
        }
        for (size_t i = 0; i < this->verticesCount(); ++i) {
            vertices[i].x += center.x;
            vertices[i].x += center.y;
        }
    }

    void scale(Point center, double coeff) override {
        for (size_t i = 0; i < this->verticesCount(); ++i) {
            vertices[i].x -= center.x;
            vertices[i].x -= center.y;
        }
        for (size_t i = 0; i < this->verticesCount(); ++i) {
            vertices[i].x *= coeff;
            vertices[i].y *= coeff;
        }
        for (size_t i = 0; i < this->verticesCount(); ++i) {
            vertices[i].x += center.x;
            vertices[i].x += center.y;
        }
    }

    void reflex(Point center) override {
        this->scale(center, 1.0);
    }

    void reflex(Line axis) override {
        std::vector<double> coeffs = axis.getLineCoeffs();
        double a = coeffs[0];
        double b = coeffs[1];
        double c = coeffs[2];
        for (size_t i = 0; i < this->verticesCount(); ++i) {
            double d = a * this->vertices[i].y - b * this->vertices[i].x;
            double y1 = (b * c - a * d) / calcSqrSum(a, b);
            double x1 = (a * y1 - d) / b;
            this->vertices[i].x = 2 * x1 - this->vertices[i].x;
            this->vertices[i].y = 2 * y1 - this->vertices[i].y;
        }
    }

private:
    std::vector<Point> vertices;
};


class Ellipse: public Shape {
public:
    Ellipse(Point f1, Point f2, double a2) : f1(f1), f2(f2), a2(a2) {}

    std::pair<Point, Point> focuses() const {
        return std::make_pair(f1, f2);
    }

    double eccentricity() const {
        double c = calcDistance(this->f1, this->f2) * 0.5;
        double a = this->a2 * 0.5;
        return c / a;
    }

    Point center() const {
        Point res((this->f1.x + this->f2.x) * 0.5, (this->f1.y + this->f2.y) * 0.5);
        return res;
    }

    double radius() const {
        return this->a2 * 0.5;
    }

    double perimeter() const override {
        double a = this->a2 * 0.5;
        double c = calcDistance(this->f1, this->f2) * 0.5;
        double b = sqrt(a * a - c * c);
        double d = 3 * pow((a - b) / (a + b), 2);
        return M_PI * (a + b) * (1 + d / (10 + sqrt(4 - d)));  // Ramanujan approx
    }

    double area() const override {
        double a = this->a2 * 0.5;
        double c = calcDistance(this->f1, this->f2) * 0.5;
        double b = sqrt(a * a - c * c);
        return M_PI * a * b;
    }

    bool operator==(const Ellipse& rhs) {
        if (this->a2 == rhs.a2) {
            if ((this->f1 == rhs.f1) and (this->f2 == rhs.f2)) {
                return true;
            }
            if ((this->f1 == rhs.f2) and (this->f2 == rhs.f1)) {
                return true;
            }
        }
        return false;
    }

    bool operator!=(const Ellipse& rhs) {
        return !(*this == rhs);
    }

    void rotate(Point center, double angle) override {
        f1.x -= center.x;  // first focus
        f1.y -= center.y;
        double x1 = rotateX(f1.x, f1.y, angle);
        double y1 = rotateY(f1.x, f1.y, angle);
        f1.x = x1 + center.x;
        f1.y = y1 + center.y;
        f2.x -= center.x;  // second focus
        f2.y -= center.y;
        x1 = rotateX(f2.x, f2.y, angle);
        y1 = rotateY(f2.x, f2.y, angle);
        f2.x = x1 + center.x;
        f2.y = y1 + center.y;
    }

    void reflex(Point center) override {
        this->scale(center, 1.0);
    }

    void reflex(Line axis) override {
        std::vector<double> coeffs = axis.getLineCoeffs();
        double a = coeffs[0];
        double b = coeffs[1];
        double c = coeffs[2];
        double d = a * this->f1.y - b * this->f1.x;  // first focus
        double y1 = (b * c - a * d) / calcSqrSum(a, b);
        double x1 = (a * y1 - d) / b;
        this->f1.x = 2 * x1 - this->f1.x;
        this->f1.y = 2 * y1 - this->f1.y;
        d = a * this->f2.y - b * this->f2.x;  // second focus
        y1 = (b * c - a * d) / calcSqrSum(a, b);
        x1 = (y1 * a - d) / b;
        this->f2.x = 2 * x1 - this->f2.x;
        this->f2.y = 2 * y1 - this->f2.y;
    }

    void scale(Point center, double coeff) override {
        this->f1.x = coeff * (this->f1.x - center.x) + center.x;
        this->f1.y = coeff * (this->f1.y - center.y) + center.y;
        this->f2.x = coeff * (this->f2.x - center.x) + center.x;
        this->f2.y = coeff * (this->f2.y - center.y) + center.y;
        this->a2 *= coeff;
    }

private:
    Point f1, f2;  // f1, f2: ellipse focuses
    double a2;  // a2 = 2 * a: ellipse width
};


class Circle: public Ellipse {
public:
    Circle(Point center, double radius) : Ellipse(center, center, radius * 2) {}
};


class Rectangle: public Polygon {
public:
    Rectangle(Point p1, Point p3, double coeff) : Polygon(initPoints(p1, p3, coeff)) {}

    Point center() {
        auto vertices = this->getVertices();
        Point res((vertices[0].x + vertices[2].x) * 0.5, (vertices[0].y + vertices[2].y) * 0.5);
        return res;
    }

    std::pair<Line, Line> diagonals() {
        auto vertices = this->getVertices();
        Line diag1 = Line(vertices[0], vertices[2]);
        Line diag2 = Line(vertices[1], vertices[3]);
        return std::make_pair(diag1, diag2);
    }

private:
    static std::vector<Point> initPoints(Point p1, Point p3, double coeff) {
        if (coeff < 1) {
            coeff = 1 / coeff;
        }
        double angle = atan(coeff);
        double x3 = p3.x - p1.x;
        double y3 = p3.y - p1.y;
        double k = (coeff * coeff + 1);
        double x2 = rotateX(x3, y3, angle) / k + p1.x;
        double y2 = rotateY(x3, y3, angle) / k + p1.y;
        Point p2(x2, y2);
        Point p4(p3.x + p1.x - x2, p3.y + p1.y - y2);
        std::vector<Point> res({p1, p2, p3, p4});
        return res;
    }
};


class Square: public Rectangle {
public:
    Square(Point p1, Point p3) : Rectangle(p1, p3, 1) {}

    Circle circumscribedCircle() {
        double radius = sqrt(2) * this->perimeter() * 0.25;
        Circle res(this->center(), radius);
        return res;
    }

    Circle inscribedCircle() {
        Circle res(this->center(), this->perimeter() * 0.25);
        return res;
    }
};


class Triangle: public Polygon {
public:
    Triangle(Point& p1, Point& p2, Point& p3) : Polygon({p1, p2, p3}) {}

    Circle circumscribedCircle() {
        auto vertices = this->getVertices();
        Point v1(vertices[0].x, vertices[0].y);
        Point v2(vertices[1].x - v1.x, vertices[1].y - v1.y);
        Point v3(vertices[2].x - v1.x, vertices[2].y - v1.y);
        double k = 2 * (v2.x * v3.y - v2.y * v3.x);
        double x = (v3.y * calcSqrSum(v2.x, v2.y) - v2.y * calcSqrSum(v3.x, v3.y)) / k;
        double y = (v2.x * calcSqrSum(v3.x, v3.y) - v3.x * calcSqrSum(v2.x, v2.y)) / k;
        double radius = sqrt(calcSqrSum(x, y));
        Point center(x + v1.x, y + v1.y);
        Circle res(center, radius);
        return res;
    }

    Circle inscribedCircle() {
        auto vertices = this->getVertices();
        double a = calcDistance(vertices[1], vertices[2]);
        double b = calcDistance(vertices[0], vertices[2]);
        double c = calcDistance(vertices[0], vertices[1]);
        double p = a + b + c;
        double x = (a * vertices[0].x + b * vertices[1].x + c * vertices[2].x) / p;
        double y = (a * vertices[0].y + b * vertices[1].y + c * vertices[2].y) / p;
        double radius = sqrt((p * 0.5 - a) * (p * 0.5 - b) * (p * 0.5 - c) / (p * 0.5));
        Point center(x, y);
        Circle res(center, radius);
        return res;
    }

    Point centroid() {
        auto vertices = this->getVertices();
        Point v1 = vertices[0];
        Point v2 = vertices[1];
        Point v3 = vertices[2];
        Point res((v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3);
        return res;
    }

    Point orthocenter() {
        Point cs_center = this->circumscribedCircle().center();
        Point res = cs_center;
        auto vertices = this->getVertices();
        for (size_t i = 0; i < 3; ++i) {
            res.x += (vertices[i].x - cs_center.x);
            res.y += (vertices[i].y - cs_center.y);
        }
        return res;
    }

    Line EulerLine() {
        Line res(this->orthocenter(), this->centroid());
        return res;
    }

    Circle ninePointsCircle() {
        Point orthocenter = this->orthocenter();
        Point cs_center = this->circumscribedCircle().center();
        Point center((orthocenter.x + cs_center.x) * 0.5, (orthocenter.y + cs_center.y) * 0.5);
        double radius = this->circumscribedCircle().radius() * 0.5;
        Circle res(center, radius);
        return res;
    }
};