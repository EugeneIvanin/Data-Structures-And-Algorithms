#include <iostream>
#include <math.h>
#include <vector>

//общая идея: находим сумму Минковского первой и разности второй фигуры,
//затем проверяем лежит ли (0, 0) в получившейся сумме

enum location {
    LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION
};

struct Point {
    Point(const double &_x, const double &_y) : x(_x), y(_y) {}

    Point operator+(const Point &b) const {
        Point dot(x + b.x, y + b.y);
        return dot;
    }

    void invert() {
        x = (-1)*x;
        y = (-1)*y;
    }

    double length() {
        return x * x + y * y;
    }

    static double angle(const Point &a, const Point &b) {
        double alpha = acos((b.x - a.x) / sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
        if (b.y - a.y < 0) {
            return 2 * M_PI - alpha;
        }
        return alpha;
    }

    double x;
    double y;
};

double det_2(const Point &a, const Point &b) {
    return a.x * b.y - b.x * a.y;
}

struct Segment {
    Segment(const Point &_A, const Point &_B);

    Point A;
    Point B;

    //Классификация положения точки (0,0)
    location class_of_zero_location;
};

Segment::Segment(const Point &_A, const Point &_B) : A(_A), B(_B) {
    Point a(B.x - A.x, B.y - A.y);
    Point b(0 - A.x, 0 - A.y);
    double sa = det_2(a, b);
    if (sa > 0) {
        class_of_zero_location = LEFT;
    }
    if (sa < 0) {
        class_of_zero_location = RIGHT;
    }
    if (a.x * b.x < 0 or a.y * b.y < 0) {
        class_of_zero_location = BEHIND;
    }
    if (a.length() < b.length()) {
        class_of_zero_location = BEYOND;
    }
    if (A.x == 0 and A.y == 0) {
        class_of_zero_location = ORIGIN;
    }
    if (B.x == 0 and B.y == 0) {
        class_of_zero_location = DESTINATION;
    }
    class_of_zero_location = BETWEEN;
}

class Polygon {
public:
    Polygon(std::vector<Point> &_polygon);

    static bool Intersect(const Polygon &A, const Polygon &B);

private:
    std::vector<Point> dots;

    void Sort();

    int polygon_size;
};

Polygon::Polygon(std::vector<Point> &_polygon) {
    dots = move(_polygon);
    Sort();
    polygon_size = static_cast<int>(dots.size());
}

void Polygon::Sort() {
    double minim_y = dots[0].y;
    double minim_x = 10000000;
    int min_pos = 0;
    for (int k = 0; k < polygon_size; k++) {
        if (dots[k].y < minim_y) {
            minim_y = dots[k].y;
        }
    }
    for (int k = 0; k < polygon_size; k++) {
        if (dots[k].y == minim_y) {
            if (dots[k].x < minim_x) {
                minim_x = dots[k].x;
                min_pos = k;
            }
        }
    }
    std::vector<Point> X;
    for (int i = min_pos; i >= 0; i--) {
        X.push_back(dots[i]);
    }
    for (int i = polygon_size - 1; i > min_pos; i--) {
        X.push_back(dots[i]);
    }
    dots = move(X);
}

bool Polygon::Intersect(const Polygon &A, const Polygon &B) {
    std::vector<Point> C;

    int i = 0;
    int j = 0;

    while (i < A.polygon_size or j < B.polygon_size) {
        Point dot = A.dots[i] + B.dots[j];
        C.push_back(dot);
        if (Point::angle(A.dots[i], A.dots[i + 1]) < Point::angle(B.dots[j], B.dots[j + 1])) {
            if (i != A.polygon_size) {
                i++;
            } else {
                j++;
            }
        } else if (Point::angle(A.dots[i], A.dots[i + 1]) > Point::angle(B.dots[j], B.dots[j + 1])) {
            if (j != B.polygon_size) {
                j++;
            } else {
                i++;
            }
        } else {
            i++;
            j++;
        }
    }
    bool result = false;

    //вырожденный случай
    if (C.size() - 1 == 0) {
        if (A.dots[0].x == B.dots[0].x * (-1) and A.dots[0].y == B.dots[0].y * (-1)) {
            result = true;
        }
    } else {
        for (int z = 0; z < C.size(); z++) {
            Segment AB(C[z], C[(z + 1) % static_cast<int>(C.size())]);
            location flag = AB.class_of_zero_location;
            if (flag == ORIGIN or flag == DESTINATION or flag == BETWEEN) {
                result = true;
                break;
            }
            if (flag == LEFT) {
                result = true;
            }
            if (flag == RIGHT) {
                result = false;
                break;
            }
        }
    }
    if (result) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return false;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Point> A;
    for (int i = 0; i < n; i++) {
        Point dot(0, 0);
        std::cin >> dot.x;
        std::cin >> dot.y;
        A.push_back(dot);
    }
    int m;
    std::cin >> m;
    std::vector<Point> B;
    for (int i = 0; i < m; i++) {
        Point dot(0, 0);
        std::cin >> dot.x;
        std::cin >> dot.y;
        dot.invert();
        B.push_back(dot);
    }
    Polygon::Intersect(A, B);
    return 0;
}