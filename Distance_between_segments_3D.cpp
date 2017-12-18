#include <iomanip>
#include <iostream>
#include <math.h>
#include <vector>

struct Point {
    Point(const double &_x, const double &_y, const double &_z) : x(_x), y(_y), z(_z) {}

    Point() : x(0), y(0), z(0) {}

    Point operator+(const Point &dot, const CVector &vec) const {
        Point d(dot.x + vec.x, dot.y + vec.y, dot.z + vec.z);
        return d;
    }

    Point operator-(const Point &dot2) const {
        Point d(dot2.x - this->x, dot2.y - this->y, dot2.z - this->z);
        return d;
    }

    double x;
    double y;
    double z;
};

struct CVector {

    CVector() {}

    CVector(const std::vector<double> &v) {
        x = v[0];
        y = v[1];
        z = v[2];
    }

    CVector(const Point &A, const Point &B) {
        x = B.x - A.x;
        y = B.y - A.y;
        z = B.z - A.z;
    }

    CVector(const double &_x, const double &_y, const double &_z) : x(_x), y(_y), z(_z) {}

    static double scalar(const CVector &v1, const CVector &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    static double scalar(const Point &dot, const CVector &v) {
        return dot.x * v.x + dot.y * v.y + dot.z * v.z;
    }

    double len() const {
        double sum = 0;
        sum += x * x;
        sum += y * y;
        sum += z * z;
        return sqrt(sum);

    }

    double pow_len() const {
        double sum = 0;
        sum += x * x;
        sum += y * y;
        sum += z * z;
        return sum;
    }

    double x;
    double y;
    double z;
};

CVector refresh_solution(const Point &dot, double q, const CVector &kl) {
    CVector temp;
    temp.x = dot.x + q * kl.x;
    temp.y = dot.y + q * kl.y;
    temp.z = dot.z + q * kl.z;
    return temp;
}

double Find_len_between_line_segments(const Point &a, const Point &b, const Point &c, const Point &d) {
    //Задача решена чисто математическим способом.
    //Ищем экстремум длины вектора, концы которого
    //лежат на прямых, соответствующих данным векторам

    const CVector ab(a, b);
    const CVector cd(c, d);
    const CVector ac(a, c);
    double scalar_ab_cd = CVector::scalar(ab, cd);
    double scalar_ac_ab = CVector::scalar(ac, ab);
    double scalar_ca_cd = (-1) * CVector::scalar(ac, cd);

    //применяем формулу для параметров s и t, с помощью которых заданы ab и cd соответственно
    double s = (scalar_ca_cd * ab.pow_len() + scalar_ab_cd * scalar_ac_ab) /
               (ab.pow_len() * cd.pow_len() - scalar_ab_cd * scalar_ab_cd);
    double t = (scalar_ac_ab * cd.pow_len() + scalar_ab_cd * scalar_ca_cd) /
               (ab.pow_len() * cd.pow_len() - scalar_ab_cd * scalar_ab_cd);

    //Случай, когда концы вектора минимальной длины между прямыми ab и cd лежат на отрезках ab и cd
    if (s <= 1 and s >= 0 and t <= 1 and t >= 0) {
        double x, y, z;
        x = c.x + cd.x * s - a.x - ab.x * t;
        y = c.y + cd.y * s - a.y - ab.y * t;
        z = c.z + cd.z * s - a.z - ab.z * t;
        CVector answ(x, y, z);
        return answ.len();
    } else {
        //будем искать минимум среди длин рассматриваемых далее отрезков
        double dist = 1000000000;
        double q;
        CVector answ(0, 0, 0);

        //расстояние от a до прямой cd

        q = CVector::scalar(a - c, cd) / cd.pow_len();
        answ = refresh_solution(c - a, q, cd);
        if (answ.len() < dist and q >= 0 and q <= 1) {
            dist = answ.len();
        }

        //расстояние от b до прямой cd
        q = CVector::scalar(b - c, cd) / cd.pow_len();
        answ = refresh_solution(c - b, q, cd);
        if (answ.len() < dist and q >= 0 and q <= 1) {
            dist = answ.len();
        }

        //расстояние от c до прямой ab
        q = CVector::scalar(c - a, ab) / ab.pow_len();
        answ = refresh_solution(a - c, q, ab);
        if (answ.len() < dist and q >= 0 and q <= 1) {
            dist = answ.len();
        }

        //расстояние от d до прямой ab
        q = CVector::scalar(d - a, ab) / ab.pow_len();
        answ = refresh_solution(a - d, q, ab);
        if (answ.len() < dist and q >= 0 and q <= 1) {
            dist = answ.len();
        }

        //попарные расстояния между концами векторов
        //вектор ac создан ранее
        CVector ad(a, d);
        CVector bc(b, c);
        CVector bd(b, d);
        std::vector<const CVector *> pairwise_vectors = {&ac, &ad, &bc, &bd};
        for (auto v : pairwise_vectors) {
            if (v->len() < dist) {
                dist = v->len();
            }
        }
        return dist;
    }
}

int main() {
    Point a, b, c, d;
    std::cin >> a.x >> a.y >> a.z;
    std::cin >> b.x >> b.y >> b.z;
    std::cin >> c.x >> c.y >> c.z;
    std::cin >> d.x >> d.y >> d.z;

    std::cout << std::fixed;
    std::cout << std::setprecision(8) << Find_len_between_line_segments(a, b, c, d);
    return 0;
}