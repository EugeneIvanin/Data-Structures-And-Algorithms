#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//the vertex of the "Bor"
struct Node {
    //accepts the parent and the symbol on which we went to this node
    Node(int p, char c);

    //on i-position we have the vertex, into which we pass along the i-th symbol of the alphabet
    vector<int> next_node;

    //is the vertex terminal?
    bool term;

    //suffix link
    int suffix_link;

    //best suffix link (terminal)
    int best_suffix_link;

    //the parent of this vertex
    int parent;

    //the symbol by which we came from the parent
    char sym_in;

    //the numbers of patterns that we denote by this vertex
    vector<int> pat_num;

    //transitions for the best-suffix-link search
    vector<int> auto_move;
};

Node::Node(int p, char c) {
    term = false;
    next_node.resize(26, -1);
    auto_move.resize(26, -1);
    suffix_link = -1;
    best_suffix_link = -1;
    parent = p;
    sym_in = c;
}

class Bor {
public:
    //конструктор принимает вектор шаблонов
    Bor(vector<char> &_patterns, vector<pair<int, int> > &_P);

    Bor() {}

    //building "Bor"
    void Build();

    //the function of the "Bor" extension in a machine with suffix links
    void Create_Automat();

protected:
    //function of finding suffix links
    int Recursive_Auto(int vertex);

    //transition on the machine in search of a suitable suffix link
    int Get_move(int parent, char sym);

    //search of the best suffix-links
    int Fill_best_suffix_links(int vertex);

    vector<Node> bor;

    //masked line with patterns
    vector<char> patterns;

    //positions of patterns
    vector<pair<int, int> > P;
};

Bor::Bor(vector<char> &_patterns, vector<pair<int, int> > &_P) {
    patterns = move(_patterns);
    P = move(_P);
}

void Bor::Build() {

    //creation of the root
    Node root(0, '%');
    bor.push_back(root);

    int count_patterns = P.size();

    for (int i = 0; i < count_patterns; i++) {
        //start the processing of the pattern from the root of "Bor"
        int vertex = 0;

        for (int j = 0; j < P[i].second - P[i].first + 1; j++) {
            //get the number of the character in the alphabet
            char current_symbol = patterns[P[i].first + j] - 'a';

            //add the new vertex
            if (bor[vertex].next_node[current_symbol] == -1) {
                Node x(vertex, current_symbol);
                bor.push_back(x);

                //we indicate that from the current vertex it is possible to come in just
                // created vertex
                bor[vertex].next_node[current_symbol] = static_cast<int>(bor.size() - 1);
            }

            //we pass on the current symbol to an existing or newly created node
            vertex = bor[vertex].next_node[current_symbol];
        }

        bor[vertex].term = true;
        bor[vertex].pat_num.push_back(i);
    }

    Create_Automat();
}

void Bor::Create_Automat() {
    //for counting the number of pattern meetings
    vector<int> patternOccurenceCount;

    //begin with root
    int u = 0;

    //the count of processed characters
    int countOfProcessedChar = 0;

    //we will read one character of the text in the cycle
    char sym;
    while (cin >> sym) {
        countOfProcessedChar++;
        patternOccurenceCount.push_back(0);
        u = Get_move(u, sym - 'a');

        for (int k = u; k != 0; k = Fill_best_suffix_links(k)) {
            if (bor[k].term == true) {
                for (auto j : bor[k].pat_num) {
                    int f = countOfProcessedChar - 1 - static_cast<int>(P[j].second - P[j].first + 1 ) + 1 - P[j].first;
                    if (f >= 0) {
                        patternOccurenceCount[f]++;
                    }
                }
            }
        }
    }

    int str_size = static_cast<int>(patterns.size());
    //search for suitable values ​​and output of positions
    for (int i = 0; i < countOfProcessedChar - str_size + 1 and i < countOfProcessedChar; i++) {
        if (patternOccurenceCount[i] == P.size()) {
            cout << i << endl;
        }
    }
}

//jump search function
int Bor::Get_move(int v, char sym) {
    if (bor[v].auto_move[sym] == -1) {
        if (bor[v].next_node[sym] == -1) {
            if (v == 0) {
                bor[v].auto_move[sym] = 0;
                return 0;
            } else {
                bor[v].auto_move[sym] = Get_move(Recursive_Auto(v), sym);
                return bor[v].auto_move[sym];
            }
        } else {
            bor[v].auto_move[sym] = bor[v].next_node[sym];
            return bor[v].auto_move[sym];
        }
    }
    return bor[v].auto_move[sym];
}

//search of suffix links
int Bor::Recursive_Auto(int vertex) {
    if (bor[vertex].suffix_link != -1) {
        return bor[vertex].suffix_link;
    } else {
        const int p = bor[vertex].parent;
        const char c = bor[vertex].sym_in;

        if (p == 0 or vertex == 0) {
            bor[vertex].suffix_link = 0;
            return 0;
        } else {
            bor[vertex].suffix_link = Get_move(Recursive_Auto(p), c);
            return bor[vertex].suffix_link;
        }
    }
}

//search of the best suffix links
int Bor::Fill_best_suffix_links(int vertex) {
    if (bor[vertex].best_suffix_link == -1) {
        int u = Recursive_Auto(vertex);
        if (bor[u].term == true) {
            bor[vertex].best_suffix_link = u;
        } else if (u == 0) {
            bor[vertex].best_suffix_link = 0;
        } else {
            bor[vertex].best_suffix_link = Fill_best_suffix_links(u);
        }
    }
    return bor[vertex].best_suffix_link;
}


int main() {
    std::ios::sync_with_stdio(false);

    //the line with mask
    string str_with_mask;
    cin >> str_with_mask;
    int str_size;
    str_size = static_cast<int>(str_with_mask.size());

    //auxiliary variables for parsing a string
    vector<char> temp_str;
    int temp_pos = 0;

    //the positions of the occurrences of the template
    vector<pair<int, int> > P;

    //parsing
    for (int i = 0; i < str_size; i++) {
        if (str_with_mask[i] != '?') {
            temp_str.push_back(str_with_mask[i]);
        } else {
            if (static_cast<int>(temp_str.size()) > 0) {
                temp_pos = i - static_cast<int>(temp_str.size());
                P.push_back(make_pair(temp_pos, i - 1));
            }
            temp_str.clear();
            temp_pos = 0;
        }
    }
    if (static_cast<int>(temp_str.size()) > 0) {
        temp_pos = str_size - static_cast<int>(temp_str.size());
        P.push_back(make_pair(temp_pos, str_size - 1));
    }

    //Solution
    vector<char> str(str_with_mask.begin(), str_with_mask.end());
    Bor obj(str, P);
    obj.Build();

    return 0;
}