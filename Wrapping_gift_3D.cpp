#include <iostream>
#include <math.h>
#include <queue>
#include <vector>

double det_2(const double &a, const double &b, const double &c, const double &d) {
    return a * d - b * c;
}

struct Point {
    Point(double _x, double _y, double _z, int _num) : x(_x), y(_y), z(_z), num(_num) {}

    Point(const Point &_dot) {
        x = _dot.x;
        y = _dot.y;
        z = _dot.z;
        num = _dot.num;
    }

    Point operator-(const Point& b) const {
        Point dot(x-b.x, y-b.y, z-b.z, -1);
        return dot;
    }

    double x;
    double y;
    double z;
    int num;
};

//храним номера точек
struct Segment {
    Segment(int _a, int _b) : a(_a), b(_b) {}

    bool operator==(const Segment &m) {
        if (m.a == this->a and m.b == this->b) {
            return true;
        } else {
            return false;
        }
    }

    int a;
    int b;
};

struct CVector {
    CVector(const Point &a, const Point &b) : x((b-a).x), y((b-a).y), z((b-a).z) {}

    CVector vectorProduct(const CVector &v1, const CVector &v2);
    static double scalar(const CVector &v1, const CVector &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
    static double scalar(const Point &dot, const CVector &v2) {
        return dot.x * v2.x + dot.y * v2.y + dot.z * v2.z;
    }

    static double angle_between(const CVector &v1, const CVector &v2);
    double length() const {return sqrt(x * x + y * y + z * z); };

    double x;
    double y;
    double z;
};

CVector CVector::vectorProduct(const CVector &v1, const CVector &v2) {
    std::vector<double> vp(3);
    vp[0] = det_2(v1.y, v1.z, v2.y, v2.z);
    vp[1] = det_2(v1.z, v1.x, v2.z, v2.x);
    vp[2] = det_2(v1.x, v1.y, v2.x, v2.y);
    Point begin(0, 0, 0, -1);
    Point end(vp[0], vp[1], vp[2], -1);
    CVector answer(begin, end);
    return answer;
}

double CVector::angle_between(const CVector &v1, const CVector &v2) {
    return acos(scalar(v1, v2) / (v1.length() * v2.length()));
}

//грань
struct Face {
    Face(const Point &_A, const Point &_B, const Point &_C) : A(_A), B(_B), C(_C) {
    }

    //нормаль к грани
    CVector normal();

    Point A;
    Point B;
    Point C;
};

CVector Face::normal() {
    double l;
    double m;
    double n;

    l = ((B - A).y) * ((C - A).z) - ((B - A).z) * ((C - A).y);
    m = (-1) * (((B - A).x) * ((C - A).z) - ((B - A).z) * ((C - A).x));
    n = ((B - A).x) * ((C - A).y) - ((B - A).y) * ((C - A).x);
    Point O(0, 0, 0, -1);
    Point N(l, m, n, -1);
    CVector norm(O, N);
    return norm;
}

class Wrapper_3D {
public:

    Wrapper_3D(const std::vector<Point> &_dots) {
        dots = std::move(_dots);
        dots_size = static_cast<int>(dots.size());
        contour.resize(dots_size);
        for (int i = 0; i < dots_size; i++) {
            contour[i].resize(dots_size, false);
        }
    }

    void wrapping_gift();

private:
    std::vector<Point> dots;
    int dots_size;

    //возвращает первую точку первой построенной грани
    Point first_point();

    //возвращает вторую точку первой построенной грани
    Point second_point(const Point &p);

    //возвращает третью точку первой построенной грани
    Point third_point(const Point &p, const Point &q);

    //сортирует точки первую построенной грани против часовой стрелки
    //относительно внешней нормали
    void sorting(Point &p, Point &q, Point &r);

    //в матрице храним информацию: принадлежит ли вектор построенному контуру
    std::vector<std::vector<bool> > contour;
};


void Wrapper_3D::wrapping_gift() {
    //p, q, r - точки первой грани
    Point p = first_point();
    Point q = second_point(p);
    Point r = third_point(p, q);
    sorting(p, q, r);

    //очередь граней для построения
    std::queue<Face> Q;
    Face Start_Face(p, q, r);
    contour[Start_Face.A.num][Start_Face.B.num] = true;
    contour[Start_Face.B.num][Start_Face.C.num] = true;
    contour[Start_Face.C.num][Start_Face.A.num] = true;
    Q.push(Start_Face);

    std::vector<Face> answer;
    answer.push_back(Start_Face);
    while (!Q.empty()) {
        Face current_Face = Q.front();
        Q.pop();

        //инициализируем ребра текущей грани
        std::vector<Segment> edges_of_face;
        Segment s1(current_Face.A.num, current_Face.B.num);
        Segment s2(current_Face.B.num, current_Face.C.num);
        Segment s3(current_Face.C.num, current_Face.A.num);
        edges_of_face.push_back(s1);
        edges_of_face.push_back(s2);
        edges_of_face.push_back(s3);

        for (auto e : edges_of_face) {
            if (contour[e.a][e.b] == true) {
                Point new_dot(0, 0, 0, -1);
                double min_angle = 1000;
                for (auto dot : dots) {
                    if (!(dot.num == current_Face.A.num) and !(dot.num == current_Face.B.num) and
                        !(dot.num == current_Face.C.num)) {
                        Face cur_face(dots[e.b], dots[e.a], dot);
                        double angle = CVector::angle_between(cur_face.normal(), current_Face.normal());
                        if (angle < min_angle) {
                            min_angle = angle;
                            new_dot = dot;
                        }
                    }
                }
                Face new_face(dots[e.b], dots[e.a], new_dot);
                Segment t1(new_face.A.num, new_face.B.num);
                Segment t2(new_face.B.num, new_face.C.num);
                Segment t3(new_face.C.num, new_face.A.num);
                std::vector<Segment> edges_of_new_face;
                edges_of_new_face.push_back(t1);
                edges_of_new_face.push_back(t2);
                edges_of_new_face.push_back(t3);
                for (auto edge : edges_of_new_face) {
                    bool inContour = contour[edge.b][edge.a];
                    if(inContour) {
                        contour[edge.b][edge.a] = false;
                    } else {
                        contour[edge.a][edge.b] = true;
                    }
                }
                answer.push_back(new_face);
                Q.push(new_face);
            }
        }
    }

    //Лексикографическая сортировка + вывод
    int count = answer.size();
    std::cout << count << std::endl;;
    double minimal = 1000;
    int min_index = 0;

    //Сортировка точек внутри каждой грани
    for (int i = 0; i < count; i++) {
        minimal = 1000;
        if (answer[i].A.num < minimal) {
            minimal = answer[i].A.num;
            min_index = 0;
        }
        if (answer[i].B.num < minimal) {
            minimal = answer[i].B.num;
            min_index = 1;
        }
        if (answer[i].C.num < minimal) {
            min_index = 2;
        }
        switch (min_index) {
            case 0: {
                break;
            }
            case 1: {
                double a = answer[i].B.num;
                double b = answer[i].C.num;
                double c = answer[i].A.num;
                answer[i].A.num = a;
                answer[i].B.num = b;
                answer[i].C.num = c;
                break;
            }
            case 2: {
                double a = answer[i].C.num;
                double b = answer[i].A.num;
                double c = answer[i].B.num;
                answer[i].A.num = a;
                answer[i].B.num = b;
                answer[i].C.num = c;
                break;
            }
        }
    }

    //лексикографическая сортировка
    std::vector<bool> color(count, true);
    int min_i = 0;
    for (int j = 0; j < count; j++) {
        std::vector<double> minim(3, 100000);
        for (int i = 0; i < answer.size(); i++) {
            if (answer[i].A.num < minim[0] and color[i] == true) {
                minim[0] = answer[i].A.num;
                minim[1] = answer[i].B.num;
                minim[2] = answer[i].C.num;
                min_i = i;
            } else if (answer[i].A.num == minim[0] and color[i] == true and answer[i].B.num < minim[1]) {
                minim[0] = answer[i].A.num;
                minim[1] = answer[i].B.num;
                minim[2] = answer[i].C.num;
                min_i = i;
            } else if (answer[i].A.num == minim[0] and color[i] == true and answer[i].B.num == minim[1] and
                       answer[i].C.num <= minim[2]) {
                minim[0] = answer[i].A.num;
                minim[1] = answer[i].B.num;
                minim[2] = answer[i].C.num;
                min_i = i;
            }
        }
        std::cout << 3 << " ";
        std::cout << answer[min_i].A.num << " ";
        std::cout << answer[min_i].B.num << " ";
        std::cout << answer[min_i].C.num << std::endl;
        color[min_i] = false;
    }
}

//ищем самую нижнюю точку по Oz, при этом самую дальнюю по Ox и самую левую по Oy
Point Wrapper_3D::first_point() {
    int min_z = dots[0].z;
    std::vector<Point> min_z_points;
    for (int i = 0; i < dots_size; i++) {
        if (dots[i].z <= min_z) {
            min_z = dots[i].z;
        }
    }
    for (int i = 0; i < dots_size; i++) {
        if (dots[i].z == min_z) {
            min_z_points.push_back(dots[i]);
        }
    }
    int min_x = 100000;
    std::vector<Point> min_x_points;
    for (int i = 0; i < min_z_points.size(); i++) {
        if (min_z_points[i].x <= min_x) {
            min_x = min_z_points[i].x;
            min_x_points.push_back(min_z_points[i]);
        }
    }
    for (int i = 0; i < min_z_points.size(); i++) {
        if (min_z_points[i].x == min_x) {
            min_x_points.push_back(min_z_points[i]);
        }
    }
    int min_y = 100000;
    Point answ(0, 0, 0, 0);
    for (int i = 0; i < min_x_points.size(); i++) {
        if (min_x_points[i].y <= min_y) {
            min_y = min_x_points[i].y;
            answ = min_x_points[i];
        }
    }
    return answ;
}

//Ищем точку q, c которой образуется максимальный угол между Oz и pq
Point Wrapper_3D::second_point(const Point &p) {
    double max_angle = -100;
    Point answer(0, 0, 0, -1);
    Point O(0, 0, 0, -1);
    Point Z(0, 0, 100, -1);
    CVector v1(O, Z);
    for (auto i : dots) {
        if (i.num != p.num) {
            CVector v2(p, i);
            if (CVector::angle_between(v1, v2) > max_angle) {
                max_angle = CVector::angle_between(v1, v2);
                answer = i;
            }
        }
    }
    return answer;
}

//ищем точку, с которой образуется грань
Point Wrapper_3D::third_point(const Point &p, const Point &q) {
    for (auto r : dots) {
        bool sign_plus = false;
        bool sign_minus = false;
        if (!(r.num == p.num) and !(r.num == q.num)) {
            Face F(p, q, r);
            for (auto i : dots) {
                if (!(i.num == p.num) and !(i.num == q.num) and !(i.num == r.num)) {
                    double D = (-1) * (CVector::scalar(F.A, F.normal()));
                    if (CVector::scalar(i, F.normal()) + D > 0) {
                        sign_plus = true;
                    } else if (F.normal().x * i.x + F.normal().y * i.y + F.normal().z * i.z + D < 0) {
                        sign_minus = true;
                    }
                }
            }
            switch (sign_plus * sign_minus) {
                case true: {
                    //с данной точкой грани не получится
                    break;
                }
                case false: {
                    return r;
                }
            }
        }
    }
}

void Wrapper_3D::sorting(Point &p, Point &q, Point &r) {
    bool sign_plus = false;
    bool sign_minus = false;

    Face F(p, q, r);
    for (auto i : dots) {
        if (!(i.num == p.num) and !(i.num == q.num) and !(i.num == r.num)) {
            double D = (-1) * (F.A.x * F.normal().x + F.A.y * F.normal().y + F.A.z * F.normal().z);
            if (F.normal().x * i.x + F.normal().y * i.y + F.normal().z * i.z + D > 0) {
                sign_plus = true;
            } else if (F.normal().x * i.x + F.normal().y * i.y + F.normal().z * i.z + D < 0) {
                sign_minus = true;
            }
        }
    }
    switch (sign_plus * sign_minus) {
        case true: {
            break;
        }
        case false: {
            if (sign_plus == true) {
                std::swap(q, r);
            }
            break;
        }
    }
}


int main() {
    int m;
    std::cin >> m; //количество тестов
    for (int i = 0; i < m; i++) {
        int n; //число точек
        std::cin >> n;
        std::vector<Point> dots;
        for (int j = 0; j < n; j++) {
            int x = 0;
            int y = 0;
            int z = 0;
            std::cin >> x;
            std::cin >> y;
            std::cin >> z;
            Point _dot(x, y, z, j);
            dots.push_back(_dot);
        }
        Wrapper_3D obj(dots);
        obj.wrapping_gift();
    }
    return 0;
}