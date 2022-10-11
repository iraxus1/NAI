#include <any>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using mojamapa_t = map<string, vector<double>>;
using mojafunkcja_t = function<double_t (vector<double>)>;
void wypisz(const mojamapa_t& mapa, const mojafunkcja_t& fun) {
    for (auto& [nazwa, wektor] : mapa) {
        cout << nazwa << ": ";
        for (auto& x : wektor){
            cout << x << " ";
        }
        cout << " -> " << fun(wektor) << endl;
        cout << " 0: OK" << endl;
    }
}
int main(int argc, char **argv) {
    map<string, mojafunkcja_t> formatery;
    formatery["sin"] = [](vector<double> x) { return sin(x[0]); };
    formatery["add"] = [](vector<double> x) { return x[0] + x[1]; };
    formatery["mod"] = [](vector<double> x) { return (int)x[0] % (int)x[1]; };
    basic_string<char> key;// = np "sin";
    try {
        vector<string> argumenty(argv, argv + argc);//
        key = argumenty[2];//2 bo na 1 jest lab1 np "sin";
        vector<double> wartosci;// = wartosci np { 1.0, 2.0 };
        for (int i = 3; i < argc; ++i) {
            wartosci.push_back(stod(argumenty[i]));//stod - string to double
        }
        wypisz({{key, wartosci}}, formatery[key]);//wypisz({{"sin", {1.0, 2.0}}}, formatery["sin"]);
    } catch ( ... ) {
        if (formatery.find(key) == formatery.end()) {
            cout << "\n1: Blad: Nie ma takiej funkcji" <<  endl;// Nie ma takiej funkcji
        } else {
            cout << "\n1: Blad: Niepoprawne argumenty" <<  endl;// Niepoprawne argumenty
        }
    }
    return 0;
}