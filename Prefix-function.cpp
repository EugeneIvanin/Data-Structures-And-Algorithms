#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class PrefixFunction {
public:
    //Конструктор принимает строку-шаблон и вектор для хранения её префикс-функции
    PrefixFunction( const string& str, vector<size_t>& pi_func );

    //Данный метод вычисляет префикс-функцию шаблона
    void prefix_func_of_template();

    //Данный метод вычисляет префикс функцию очередного символа текста,
    //в котором ищем вхождение шаблона
    size_t prefix_func_of_symbol( size_t k, char sym );

private:
    //Строка-шаблон
    string s;

    //Вектор, хранящий префикс функцию
    vector<size_t> pi;
};

PrefixFunction::PrefixFunction( const string& str, vector<size_t>& pi_func ) : s(str), pi(pi_func) {}


void PrefixFunction::prefix_func_of_template() {
    size_t n = static_cast<size_t>(s.length());

    for( int i = 1; i < n; i++ ) {
        size_t k = pi[i-1];
        do {
            if( s[i] == s[k] ) {
                pi[i] = k + 1;
                break;
            } else if( pi[i-1] == 0 ) {
                pi[i]=0;
                break;
            } else {
                k = pi[k-1];
                if( s[i] == s[k] ) {
                    pi[i] = k + 1;
                    break;
                }
            }
        } while (k>0);
    }
}

size_t PrefixFunction::prefix_func_of_symbol( size_t k, char sym ) {
    do {
        if( sym == s[k] and k < static_cast<int>(s.length()) ) {
            return k + 1;
        } else if( k == 0 ) {
            return 0;
        } else {
            k = pi[k-1];
            if( sym == s[k] ) {
                return k + 1;
            }
        }
    } while (k>0);
    return 0;
}

int main()
{
    //Строка-шаблон
    string s;
    cin >> s;
    s = s + "&";
    size_t n = static_cast<size_t>(s.length());

    //Текст, в котором будем искать вхождения
    string t;
    cin >> t;

    //Вектор, в котором будем хранить значения префикс-функции шаблона
    vector<size_t> pi(n);

    //Вычисление префикс-функции шаблона
    PrefixFunction obj(s, pi);
    obj.prefix_func_of_template();

    //В k будем хранить значение префикс-функции предыдущего
    //символа в тексте
    size_t k = 0;
    for( int i = 0; i < static_cast<int>(t.size()); i++ ) {
        k = obj.prefix_func_of_symbol(k, t[i]);
        if( k == n-1 ) {
            cout << i - n + 2 << " ";
        }
    }

    return 0;
}