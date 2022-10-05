#include <any>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using mojamapa_t = std::map<std::string, std::vector<double>>;
using mojafunkcja_t = std::function<std::double_t (std::vector<double>)>;
void wypisz(mojamapa_t mapa, mojafunkcja_t fun) {
    using namespace std;
    for (auto kv : mapa) {
        auto [k, v] = kv;
        cout << "klucz: " << k << " \nwartosc " << fun(v) << endl;
    }
}
int main(int argc, char **argv) {
    using namespace std;
    map<string, mojafunkcja_t> formatery;
    formatery["sin"] = [](vector<double> x) { return sin(x[0]); };
    formatery["add"] = [](vector<double> x) { return x[0] + x[1]; };
    formatery["mod"] = [](vector<double> x) { return (int)x[0] % (int)x[1]; };
    try {
        vector<string> argumenty(argv, argv + argc);
        string key = argumenty[1];
        vector<double> wartosci;
        for (int i = 2; i < argc; ++i) {
            wartosci.push_back(stod(argumenty[i]));
        }
        mojamapa_t mojamapa = {{key, wartosci}};
        wypisz(mojamapa, formatery[key]);
    } catch (std::exception exception) {
        if(argc == 2 ){
           cout << "0" <<endl;
        }else{
            cout <<"Blad kod: 1"<<endl;
        }
    }
    return 0;
}