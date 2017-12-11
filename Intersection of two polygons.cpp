#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

//общая идея: находим сумму Минковского первой и разности второй фигуры,
//затем проверяем лежит ли (0, 0) в получившейся сумме

struct Point {
    Point(const double &_x, const double &_y) : x(_x), y(_y) {}

    static double angle(const Point &a, const Point &b);

    double x;
    double y;
};

//угол между векторами Oa и Ob
static double Point::angle(const Point &a, const Point &b) {
    double alpha = acos((b.x - a.x) / sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
    if (b.y - a.y < 0) {
        return 2 * M_PI - alpha;
    }
    return alpha;
}

struct Segment {
    Segment(const Point &_A, const Point &_B);
    Point A;
    Point B;

    //Классификация положения точки (0,0)
    //1 - слева от AB
    //2 - справа от AB
    //3 - позади от AB
    //4 - впереди от AB
    //5 - совпадает с точкой А
    //6 - совпадает с точкой B
    //7 - лежит на AB
    int class_of_zero_location;
};

Segment::Segment(const Point &_A, const Point &_B) : A(_A), B(_B) {
    Point a(B.x - A.x, B.y - A.y);
    Point b(0 - A.x, 0 - A.y);
    double sa = a.x * b.y - b.x * a.y;
    if (sa > 0) {
        class_of_zero_location = 1;
    }
    if (sa < 0) {
        class_of_zero_location = 2;
    }
    if (a.x * b.x < 0 or a.y * b.y < 0) {
        class_of_zero_location = 3;
    }
    if (pow(a.x, 2) + pow(a.y, 2) < pow(b.x, 2) + pow(b.y, 2)) {
        class_of_zero_location = 4;
    }
    if (A.x == 0 and A.y == 0) {
        class_of_zero_location = 5;
    }
    if (B.x == 0 and B.y == 0) {
        class_of_zero_location = 6;
    }
    class_of_zero_location = 7;
}

class Polygon {
public:
    Polygon(vector<Point> &_polygon);
    static bool Intersect(const Polygon &A, const Polygon &B);
private:
    vector<Point> dots;
    void Sort();
    int polygon_size;
};

Polygon::Polygon(vector<Point> &_polygon) {
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
    vector<Point> X;
    for (int i = min_pos; i >= 0; i--) {
        X.push_back(dots[i]);
    }
    for (int i = polygon_size - 1; i > min_pos; i--) {
        X.push_back(dots[i]);
    }
    dots = move(X);
}

static bool Polygon::Intersect(const Polygon &A, const Polygon &B) {
    vector<Point> C;

    int i = 0;
    int j = 0;

    while (i < A.polygon_size or j < B.polygon_size) {
        Point dot(A.dots[i].x + B.dots[j].x, A.dots[i].y + B.dots[j].y);
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
            int flag = AB.class_of_zero_location;
            if (flag == 5 or flag == 6 or flag == 7) {
                result = true;
                break;
            }
            if (flag == 1) {
                result = true;
            }
            if (flag == 2) {
                result = false;
                break;
            }
        }
    }
    if (result) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    return false;
}

int main() {
    int n;
    cin >> n;
    vector<Point> A;
    for (int i = 0; i < n; i++) {
        Point dot(0, 0);
        cin >> dot.x;
        cin >> dot.y;
        A.push_back(dot);
    }
    int m;
    cin >> m;
    vector<Point> B;
    for (int i = 0; i < m; i++) {
        Point dot(0, 0);
        cin >> dot.x;
        dot.x = dot.x * (-1);
        cin >> dot.y;
        dot.y = dot.y * (-1);
        B.push_back(dot);
    }
    Polygon::Intersect(A, B);
    return 0;
}