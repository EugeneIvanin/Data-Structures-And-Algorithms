#include <iostream>
#include <vector>

enum TFunctionType {
    FT_PrefixFunction, FT_ZFunction
};

class P_function_handler {
public:
    P_function_handler(const int &_n, const std::string &_str) {
        n = _n;
        str = _str;
    }

    P_function_handler() {}

    void Str_to_Pi();

    std::string Pi_to_Str();

    void Print_Pi();

    std::vector<int> Pi_to_Z();

protected:

    //the vector of prefix-function
    std::vector<int> pi;

private:
    int n;
    std::string str;
};

void P_function_handler::Str_to_Pi() {
    pi[0] = 0;

    for (int i = 1; i < n; i++) {
        int k = pi[i - 1];
        do {
            if (str[i] == str[k]) {
                pi[i] = k + 1;
                break;
            } else if (pi[i - 1] == 0) {
                pi[i] = 0;
                break;
            } else {
                k = pi[k - 1];
                if (str[i] == str[k]) {
                    pi[i] = k + 1;
                    break;
                }
            }
        } while (k > 0);
    }
}

void P_function_handler::Print_Pi() {
    for (int i = 0; i < n; i++) {
        std::cout << pi[i] << " ";
    }
    std::cout << std::endl;
}

std::string P_function_handler::Pi_to_Str() {
    str += 'a';

    std::vector<std::vector<bool> > alpha(n);
    for (int i = 0; i < n; i++) {
        alpha[i].resize(26);
    }
    for (int i = 1; i < n; i++) {
        if (pi[i] != 0) {
            str += str[pi[i] - 1];
        } else {
            alpha[i][0] = true;
            int k = pi[i - 1];

            while (k > 0) {
                alpha[i][str[k] - 'a'] = true;
                k = pi[k - 1];
            }

            for (int j = 0; j < 26; j++) {
                if (alpha[i][j] == false) {
                    str += 'a' + j;
                    break;
                }
            }
        }
    }
}

std::vector<int> P_function_handler::Pi_to_Z() {
    std::vector<int> z(n);
    z[0] = n;

    for (int i = 1; i < n; i++) {
        if (pi[i] > 0) {
            z[i - pi[i] + 1] = pi[i];
        }
    }
    int k = 1;
    while (k < n) {
        int t = k;
        if (z[k] > 0) {
            for (int j = 1; j < z[k]; j++) {
                if (z[k + j] > z[j]) {
                    break;
                }
                z[k + j] = std::min(z[j], z[k] - j);
                t = k + j;
            }
        }
        k = t + 1;
    }
    return z;
}

class Z_function_handler : P_function_handler {
public:
    Z_function_handler(const int &_n, const std::string &_str) {
        n = _n;
        str = _str;
    }

    void Str_to_Z();

    std::string Z_to_Str();

    void Print_Z();

    std::vector<int> Z_to_Pi();

protected:

//the vector of z-function
    std::vector<int> z;

private:
    int n;
    std::string str;

};

void Z_function_handler::Print_Z() {
    for (int i = 0; i < n; i++) {
        std::cout << z[i] << " ";
    }
    std::cout << std::endl;
}

void Z_function_handler::Str_to_Z() {
    z[0] = n;
    int l = 0;
    int r = 0;

    for (int i = 1; i < n; i++) {
        if (l <= i and i <= r) {
            if (z[i - l] <= r - i) {
                z[i] = z[i - l];
            } else {
                int j = 1;
                while (j + r < n and str[r + j] == str[r - i + j]) {
                    j++;
                }
                z[i] = r - i + j;

                //refresh left and right borders
                l = i;
                r = r + j - 1;
            }
        } else {
            int j = 0;
            while (j + i < n and str[i + j] == str[j]) {
                j++;
            }
            z[i] = j;

            //refresh left and right borders
            l = i;
            r = i + j - 1;
        }
    }
}

std::string Z_function_handler::Z_to_Str() {
    Z_to_Pi();
    Pi_to_Str();
}

std::vector<int> Z_function_handler::Z_to_Pi() {
    std::vector<int> pi(n);
    pi[0] = 0;

    for (int i = 1; i < n; i++) {
        if (z[i] != 0) {
            for (int j = z[i] - 1; j >= 0 and pi[i + j] == 0; j--) {
                pi[i + j] = j + 1;
            }
        }
    }
    return pi;
}

class Transformations {
public:
    //accepts the length of X-function, its vector and type
    Transformations(int _n, std::vector<int> &x, TFunctionType type);

    Transformations() {}

    void Print_Str() {
        for (int i = 0; i < n; i++) {
            std::cout << str[i];
        }
        std::cout << std::endl;
    }

    //the line
    std::string str;

    //the length of function
    int n;
};

Transformations::Transformations(int _n, std::vector<int> &x, TFunctionType type) {
    n = _n;
    str = "";
    switch (type) {
        case FT_PrefixFunction: {
            P_function_handler obj(n, str);
            str = obj.Pi_to_Str();
            Print_Str();
            break;
        }
        case FT_ZFunction: {
            Z_function_handler obj(n, str);
            str = obj.Z_to_Str();
            Print_Str();
            break;
        }
    }
}

int main() {
    int num;

    //the vector for reading the prefix-function or the z-function
    std::vector<int> func;
    while (std::cin >> num) {
        func.push_back(num);
    }
    TFunctionType func_type;
    std::cout << "Please, select the type of function:" << std::endl;
    std::cout << "1)Prefix-function" << std::endl;
    std::cout << "2)Z-function" << std::endl;
    short select = 0;
    std::cin >> select;
    switch (select) {
        case 1: {
            func_type = FT_PrefixFunction;
            break;
        }
        case 2: {
            func_type = FT_ZFunction;
            break;
        }
        default: {
            std::cerr << "Incorrect choice" << std::endl;
            exit(0);
        }
    }

    Transformations obj(static_cast<int>(func.size()), func, func_type);

    return 0;
}