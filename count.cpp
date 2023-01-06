#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>    //uso unordered_map ya que es mas rapido que map en la insercion y busqueda de grandes cantidades de datos
#include <vector>   //las librerias vector y algorithm son usadas para ordenar los resultados
#include <algorithm>
using namespace std;

#define wordLen 50

class word_counter{
    private:
        unordered_map<string,int32_t> words;

        class fileManagement{
            public:
                ifstream file;

                void fileOpen(string name){
                    file.open(name.c_str());
                    if (!file.is_open()) {
                        cerr << "Error opening file " << name << endl;
                        exit(EXIT_FAILURE);
                    }
                }

                void fileClose(){
                    file.close();
                }
        };

        fileManagement file;

        void normalize(string *line){
            transform(line->begin(), line->end(), line->begin(), ::tolower);
            line->erase(remove_if(line->begin(), line->end(), ::ispunct), line->end());
        }

        void split(string line, regex expresion){
            pair<unordered_map<string,int32_t>::iterator, bool> ret;
            smatch matches;
            normalize(&line);
            if( regex_search(line,matches,expresion) ){
                while (regex_search(line, matches, expresion)) {
                    for (auto match : matches) {
                        ret = words.insert( pair<string,int32_t> (match.str(),(int32_t)1) );

                        if( !(ret.second) ){
                            ret.first->second = ret.first->second + (int32_t)1;
                        }
                    }
                    
                    line = matches.suffix().str();
                }
                
            }
        }
    public:
        word_counter(string fname){
            file.fileOpen(fname);
        }

        void countWords(){
            string line;
            regex expresion("\\w+");

            while( getline(file.file,line) ){
                split(line,expresion);
            }
        }

        void printWordCount() {
            vector<pair<int32_t, string>> pares;
            for (auto& par : words) {
                pares.emplace_back(par.second, par.first);
            }

            sort(pares.begin(), pares.end());

            for (auto& par : pares) {
                cout << par.second << ": " << par.first << endl;
            }
        }

        void close(){
            file.fileClose();
        }
};


int main(void){
    string fname;

    cout << "enter the file name: ";
    cin >> fname;

    word_counter WC(fname);
    WC.countWords();
    WC.printWordCount();

    WC.close();

    return 0;
}