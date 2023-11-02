#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdexcept>

using namespace std;

class parametrs {
public:
    string comand;
    vector<double> param;

    parametrs() {}
    ~parametrs() {}
};
vector<parametrs> separation(ifstream& file) {
    string line;
    vector<parametrs> v;
    parametrs bufferPara;
    size_t j;

    while (getline(file, line)) {
        vector<string> parts;
        string part;

        for (char c : line) {
            if (c == ' ') {
                parts.push_back(part);
                part = "";
            } else {
                part += c;
            }
        }
        parts.push_back(part); 

        if (!parts.empty()) {
            bufferPara.comand = parts[0];
            bufferPara.param.clear();
            for (j = 1; j < parts.size(); j++) {
                try {
                    double newd = stod(parts[j]); 
                    bufferPara.param.push_back(newd);
                } catch (const invalid_argument& e) {
                    continue;
                }
            }
            v.push_back(bufferPara);
        }
    }
    return v;
}

/*int main() {
    ifstream file("comand.txt");
    if (!file.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }
    vector<parametrs> v = separation(file);
    file.close();

    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i].comand << endl;
        cout << "\n";
        for (size_t j = 0; j < v[i].param.size(); j++) {
            cout << v[i].param[j] << " ";
        }
        cout << "\n";
        cout << "\n";
    }
    return 0;
}*/
