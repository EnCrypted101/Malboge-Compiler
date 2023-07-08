// After running the compiler save your malboge code as .mb file and run it by opening it on the running compiler code

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

const char* opcodes = "ji*p</vo";
const char* table = "5z]&gqtyfr$(we4{WP)H-Zn,[%\3dL+Q;>U!pJS72Fh"
                    "OA1CB6v^=I_0/8|jsb9m<.TVac`uY*MK'X~xDl}REokN:#?G\"i@";
const int rotate[10] = {1, 3, 0, 7, 4, 2, 6, 9, 5, 8};
const int memsize = 59049;
vector<int> memory(memsize);
int a = 0, c = 0, d = 0;
map<char, int> instr;

void init_instr() {
    for (int i = 0; i < 8; i++) {
        instr[opcodes[i]] = i;
    }
}

void read_source(const char* filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error: cannot open file " << filename << endl;
        exit(1);
    }
    string line;
    int i = 0;
    while (getline(fin, line)) {
        for (char ch : line) {
            if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') continue;
            if (ch < 33 || ch > 126) {
                cerr << "Error: invalid character " << ch << endl;
                exit(1);
            }
            memory[i++] = ch;
            if (i >= memsize) {
                cerr << "Error: source code too large" << endl;
                exit(1);
            }
        }
    }
    fin.close();
}

string translate(int x) {
    switch (x) {
        case 0:
            return "goto label" + to_string(d) + ";";
        case 1:
            return "cout << char(a);";
        case 2:
            return "cin >> a;";
        case 3:
            return "memory[d] = rotate[memory[d] % 10];";
        case 4:
            return "d = memory[d];";
        case 5:
            return "a = (memory[d] * a + memory[d] + a) % memsize;";
        case 6:
            return "return 0;";
        case 7:
            return "";
        default:
            cerr << "Error: unknown instruction " << x << endl;
            exit(1);
    }
}

void generate_code() {
    cout << "#include <iostream>" << endl;
    cout << "#include <vector>" << endl;
    cout << "using namespace std;" << endl;
    cout << "const int rotate[10] = {1, 3, 0, 7, 4, 2, 6, 9, 5, 8};" << endl;
    cout << "const int memsize = 59049;" << endl;
    cout << "vector<int> memory(memsize);" << endl;
    cout << "int a = 0, c = 0, d = 0;" << endl;
    cout << "int main() {" << endl;
    for (int i = 0; i < memsize; i++) {
        if (memory[i] != 0) {
            cout << "memory[" << i << "] = " << memory[i] << ";" << endl;
        }
    }
    while (true) {
        cout << "label" << c << ":" << endl;
        int x = instr[memory[c]];
        cout << translate(x) << endl;
        memory[c] = table[memory[c] - 33];
        c = (c + 1) % memsize;
        d = (d + 1) % memsize;
    }
    cout << "}" << endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <source file>" << endl;
        return 1;
    }
    init_instr();
    read_source(argv[1]);
    generate_code();
    return 0;
}
