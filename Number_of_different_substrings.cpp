#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

//возвращает число, округленное до целого вверх
int upper_integer_part(double x)
{
    if (x - static_cast<int>(x) == 0.0)
    {
        return static_cast<int>(x);
    } else {
        return (static_cast<int>(x) + 1);
    }
}

//обработчик строки
class Line_Handler
{
public:
    Line_Handler(const string &_str);

    //возвращает число различных подстрок
    int Count_of_diff_substr();

private:
    //длина принятой строки
    int str_len;

    //построение суффиксного массива принятой строки
    void Build_Suffix_Vector();

    //построение LCP-вектора (longest common prefix)
    void Build_LCP_Vector();

    //принятая строка (храним позиции каждого символа в начальной строке)
    vector<pair<char, int>> str;

    //суффиксный массив
    vector<int> suf_mas;

    //LCP-массив
    vector<int> lcp;
};

Line_Handler::Line_Handler(const string &_str) {
    str_len = static_cast<int>(_str.size());
    for (int i = 0; i < str_len; i++) {
        str.push_back(make_pair(_str[i], i));
    }
    lcp.resize(str_len-1);
}


void Line_Handler::Build_Suffix_Vector() {

    vector<int> sorted_permutation;
    vector<int> classes(str_len);
    int k_max = upper_integer_part(log(str_len)/log(2));

    vector<vector<int>> pocket_sort(256);
    //случай для k=0, база
    for (int i = 0; i < str_len; i++) {
        pocket_sort[str[i].first].push_back(str[i].second);
    }

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < pocket_sort[i].size(); j++) {
            sorted_permutation.push_back(pocket_sort[i][j]);
        }
    }
    classes[sorted_permutation[0]] = 0;
    int count_classes = 1;
    for (int i = 1; i < str_len; i++ ) {
        if (str[sorted_permutation[i]].first != str[sorted_permutation[i-1]].first) {
            count_classes++;
        }
        classes[sorted_permutation[i]] = count_classes-1;
    }

    //дальше сортируем по индукции, опираясь на базу
    for (int k = 1; k <= k_max; k++) {
        vector<int> old_classes = classes;
        vector<int> old_sorted_permutation = sorted_permutation;
        classes.clear();
        sorted_permutation.clear();
        classes.resize(str_len);
        vector< pair < pair <int, int>, int > > pairs;
        for (int i = 0; i < str_len; i++) {
            pairs.push_back(make_pair(make_pair(old_classes[i], old_classes[i+pow(2, k-1)]), i));
        }

        //модификация старой перестановки
        for (int i = 0 ; i < str_len; i++) {
            old_sorted_permutation[i] = old_sorted_permutation[i] - pow(2, k-1);
            if ( old_sorted_permutation[i] < 0 ) {
                old_sorted_permutation[i] += str_len;
            }
        }
        vector< vector <int> > pockets(str_len);
        for( auto i : old_sorted_permutation) {
            pockets[pairs[i].first.first].push_back(pairs[i].second);
        }
        for( auto i : pockets) {
            for (auto j : i) {
                sorted_permutation.push_back(j);
            }
        }
        classes[sorted_permutation[0]] = 0;
        count_classes = 1;
        for (int i = 1; i < str_len; i++ ) {
            if (pairs[sorted_permutation[i]].first.first != pairs[sorted_permutation[i-1]].first.first
                or
                pairs[sorted_permutation[i]].first.second != pairs[sorted_permutation[i-1]].first.second) {
                count_classes++;
            }
            classes[sorted_permutation[i]] = count_classes-1;
        }
    }
    suf_mas = sorted_permutation;
}

void Line_Handler::Build_LCP_Vector() {
    vector<int> rank(str_len);
    for (int i = 0; i < str_len; i++) {
        rank[suf_mas[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < str_len; ++i)
    {
        if (k > 0) {
            k--;
        }
        if (rank[i] == str_len - 1) {
            lcp[str_len-1] = -1;
            k = 0;
        } else {
            int j = suf_mas[rank[i]+1];
            while (max(i+k, j+k) < str_len
                    and str[i+k].first == str[j+k].first) {
                k++;
            }
            lcp[rank[i]] = k;
        }
    }
}

int Line_Handler::Count_of_diff_substr() {
    Build_Suffix_Vector();
    Build_LCP_Vector();
    int sum1 = 0;
    for (int i = 0; i < lcp.size()+1; i++) {
        sum1 += lcp.size() - suf_mas[i];
    }
    int sum2= 0;
    for (int i = 0; i < lcp.size(); i++) {
        sum2 += lcp[i];
    }
    return sum1-sum2;
}

int main() {
    string s;
    cin >> s;
    s = s + '$';
    Line_Handler obj(s);
    cout << obj.Count_of_diff_substr();
    return 0;
}