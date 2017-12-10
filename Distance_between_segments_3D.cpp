#include <iomanip>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

struct Point {
    Point(const double &_x, const double &_y, const double &_z) : x(_x), y(_y), z(_z) {}

    Point() : x(0), y(0), z(0) {}

    double x;
    double y;
    double z;
};

struct CVector {
    CVector(const vector<double> &v) {
        x = v[0];
        y = v[1];
        z = v[2];
        double sum = 0;
        for (int i = 0; i < 3; i++) {
            sum += pow(v[i], 2);
        }
        len = sqrt(sum);
        pow_len = sum;
    }

    CVector(const Point &A, const Point &B) {
        x = B.x - A.x;
        y = B.y - A.y;
        z = B.z - A.z;
        double sum = 0;
        sum += x * x;
        sum += y * y;
        sum += z * z;
        len = sqrt(sum);
        pow_len = sum;
    }

    CVector(const double &_x, const double &_y, const double &_z) : x(_x), y(_y), z(_z) {
        double sum = 0;
        sum += x * x;
        sum += y * y;
        sum += z * z;
        len = sqrt(sum);
        pow_len = sum;
    }

    static double scalar(const CVector &v1, const CVector &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    double update_len() {
        double sum = 0;
        sum += x * x;
        sum += y * y;
        sum += z * z;
        len = sqrt(sum);
        pow_len = sum;
        return len;
    }

    double x;
    double y;
    double z;
    double len;
    double pow_len;
};

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
    double s = (scalar_ca_cd * ab.pow_len + scalar_ab_cd * scalar_ac_ab) /
               (ab.pow_len * cd.pow_len - scalar_ab_cd * scalar_ab_cd);
    double t = (scalar_ac_ab * cd.pow_len + scalar_ab_cd * scalar_ca_cd) /
               (ab.pow_len * cd.pow_len - scalar_ab_cd * scalar_ab_cd);

    //Случай, когда концы вектора минимальной длины между прямыми ab и cd лежат на отрезках ab и cd
    if (s <= 1 and s >= 0 and t <= 1 and t >= 0) {
        double x, y, z;
        x = c.x + cd.x * s - a.x - ab.x * t;
        y = c.y + cd.y * s - a.y - ab.y * t;
        z = c.z + cd.z * s - a.z - ab.z * t;
        CVector answ(x, y, z);
        return answ.len;
    } else {
        //будем искать минимум среди длин рассматриваемых далее отрезков
        double dist = 1000000000;
        double x0, y0, z0;
        double q;
        CVector answ(0, 0, 0);

        //расстояние от a до прямой cd
        x0 = a.x;
        y0 = a.y;
        z0 = a.z;
        q = ((x0 - c.x) * cd.x + (y0 - c.y) * cd.y + (z0 - c.z) * cd.z) / cd.pow_len;
        answ.x = c.x + cd.x * q - x0;
        answ.y = c.y + cd.y * q - y0;
        answ.z = c.z + cd.z * q - z0;
        if (answ.update_len() < dist and q >= 0 and q <= 1) {
            dist = answ.update_len();
        }

        //расстояние от b до прямой cd
        x0 = a.x + ab.x;
        y0 = a.y + ab.y;
        z0 = a.z + ab.z;
        q = ((x0 - c.x) * cd.x + (y0 - c.y) * cd.y + (z0 - c.z) * cd.z) / cd.pow_len;
        answ.x = c.x + cd.x * q - x0;
        answ.y = c.y + cd.y * q - y0;
        answ.z = c.z + cd.z * q - z0;
        if (answ.update_len() < dist and q >= 0 and q <= 1) {
            dist = answ.update_len();
        }

        //расстояние от c до прямой ab
        x0 = c.x;
        y0 = c.y;
        z0 = c.z;
        q = ((x0 - a.x) * ab.x + (y0 - a.y) * ab.y + (z0 - a.z) * ab.z) / ab.pow_len;
        answ.x = a.x + ab.x * q - x0;
        answ.y = a.y + ab.y * q - y0;
        answ.z = a.z + ab.z * q - z0;
        if (answ.update_len() < dist and q >= 0 and q <= 1) {
            dist = answ.update_len();
        }

        //расстояние от d до прямой ab
        x0 = c.x + cd.x;
        y0 = c.y + cd.y;
        z0 = c.z + cd.z;
        q = ((x0 - a.x) * ab.x + (y0 - a.y) * ab.y + (z0 - a.z) * ab.z) / ab.pow_len;
        answ.x = a.x + ab.x * q - x0;
        answ.y = a.y + ab.y * q - y0;
        answ.z = a.z + ab.z * q - z0;
        if (answ.update_len() < dist and q >= 0 and q <= 1) {
            dist = answ.update_len();
        }

        //попарные расстояния между концами векторов
        //вектор ac создан ранее
        CVector ad(a, d);
        CVector bc(b, c);
        CVector bd(b, d);
        vector<const CVector *> pairwise_vectors = {&ac, &ad, &bc, &bd};
        for (auto v : pairwise_vectors) {
            if (v->len < dist) {
                dist = v->len;
            }
        }
        return dist;
    }
}

int main() {
    Point a, b, c, d;
    cin >> a.x >> a.y >> a.z;
    cin >> b.x >> b.y >> b.z;
    cin >> c.x >> c.y >> c.z;
    cin >> d.x >> d.y >> d.z;

    cout << fixed;
    cout << setprecision(8) << Find_len_between_line_segments(a, b, c, d);
    return 0;
}