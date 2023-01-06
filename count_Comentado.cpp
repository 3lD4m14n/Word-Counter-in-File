#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>    // utilizamos unordered_map para la inserción y búsqueda rápidas de grandes cantidades de datos
#include <vector>   // las librerías vector y algorithm se utilizan para ordenar los resultados
#include <algorithm>
using namespace std;

#define wordLen 50

class word_counter{
    private:
        // mapa que almacena las palabras y sus contadores
        unordered_map<string,int32_t> words;

        // clase anidada para la gestión de archivos
        class fileManagement{
            public:
                ifstream file;

                // abre un archivo con el nombre especificado
                void fileOpen(string name){
                    file.open(name.c_str());
                    if (!file.is_open()) {
                        cerr << "Error opening file " << name << endl;
                        exit(EXIT_FAILURE);
                    }
                }

                // cierra el archivo
                void fileClose(){
                    file.close();
                }
        };

        fileManagement file;

        // normaliza una línea eliminando símbolos de puntuación y convirtiendo las letras a minúsculas
        void normalize(string *line){
            transform(line->begin(), line->end(), line->begin(), ::tolower);
            line->erase(remove_if(line->begin(), line->end(), ::ispunct), line->end());
        }

        // divide una línea en palabras y las almacena en el mapa
        void split(string line, regex expresion){
            // inserta una palabra en el mapa y devuelve un par que indica si la inserción fue exitosa
            pair<unordered_map<string,int32_t>::iterator, bool> ret;
            smatch matches;
            normalize(&line);
            if( regex_search(line,matches,expresion) ){
                // mientras queden coincidencias en la línea
                while (regex_search(line, matches, expresion)) {
                    // procesamos cada coincidencia
                    for (auto match : matches) {
                        // insertamos la palabra en el mapa y obtenemos el iterador y un indicador de éxito
                        ret = words.insert( pair<string,int32_t> (match.str(),(int32_t)1) );

                        // si la palabra ya existía en el mapa, aumentamos su contador
                        if( !(ret.second) ){
                            ret.first->second = ret.first->second + (int32_t)1;
                        }
                    }
                    // buscamos coincidencias en el resto de la línea
                    line = matches.suffix().str();
                }
            }
        }
    public:
        // constructor que abre el archivo con el nombre especificado
        word_counter(string fname){
            file.fileOpen(fname);
        }

        // cuenta las palabras del archivo y las almacena en el mapa
        void countWords(){
            string line;
            // expresión regular que encuentra todas las palabras
            regex expresion("\\w+");

            // mientras haya líneas en el archivo
            while( getline(file.file,line) ){
                // dividimos la línea en palabras y las almacenamos en el mapa
                split(line,expresion);
            }
        }

        // imprime las palabras y sus contadores en orden alfabético
        void printWordCount() {
            // utilizamos un vector para almacenar los pares (valor, clave) del mapa
            vector<pair<int32_t, string>> pares;
            // llenamos el vector con los pares del mapa
            for (auto& par : words) {
                pares.emplace_back(par.second, par.first);
            }
            // ordenamos el vector según el valor de los pares
            sort(pares.begin(), pares.end());
            // imprimimos el contenido del vector
            for (auto& par : pares) {
                cout << par.second << ": " << par.first << endl;
            }
        }

        // cierra el archivo
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

