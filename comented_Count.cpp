#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>    // we use an unordered_map for fast insertion and search of large amounts of data
#include <vector>   // the vector and algorithm libraries are used to sort the results
#include <algorithm>
using namespace std;

#define wordLen 50

class word_counter{
    private:
        // map that stores words and their counters
        unordered_map<string,int32_t> words;

        // nested class for file management
        class fileManagement{
            public:
                ifstream file;

                // opens a file with the specified name
                void fileOpen(string name){
                    file.open(name.c_str());
                    if (!file.is_open()) {
                        cerr << "Error opening file " << name << endl;
                        exit(EXIT_FAILURE);
                    }
                }

                // closes the file
                void fileClose(){
                    file.close();
                }
        };

        fileManagement file;

        // normalizes a line by removing punctuation and converting letters to lowercase
        void normalize(string *line){
            transform(line->begin(), line->end(), line->begin(), ::tolower);
            line->erase(remove_if(line->begin(), line->end(), ::ispunct), line->end());
        }

        // splits a line into words and stores them in the map
        void split(string line, regex expresion){
            // inserts a word in the map and returns a pair indicating the success of the insertion
            pair<unordered_map<string,int32_t>::iterator, bool> ret;
            smatch matches;
            normalize(&line);
            if( regex_search(line,matches,expresion) ){
                // while there are matches in the line
                while (regex_search(line, matches, expresion)) {
                    // we process each match
                    for (auto match : matches) {
                        // we insert the word in the map and get the iterator and a success indicator
                        ret = words.insert( pair<string,int32_t> (match.str(),(int32_t)1) );

                        // if the word already exists in the map, we increase its counter
                        if( !(ret.second) ){
                            ret.first->second = ret.first->second + (int32_t)1;
                        }
                    }
                    // we search for matches in the rest of the line
                    line = matches.suffix().str();
                }            
            }    
        }
    public:
        // constructor that opens the file with the specified name
        word_counter(string fname){
            file.fileOpen(fname);
        }

        // counts the words in the file and stores them in the map
        void countWords(){
            string line;
            // regular expression that finds all words
            regex expresion("\\w+");

            // while there are lines in the file
            while( getline(file.file,line) ){
                // we split the line into words and store them in the map
                split(line,expresion);
            }
        }

        // prints the words and their counters in alphabetical order
        void printWordCount() {
            // we use a vector to store the (value, key) pairs of the map
            vector<pair<int32_t, string>> pares;
            // we fill the vector with the pairs of the map
            for (auto& par : words) {
                pares.emplace_back(par.second, par.first);
            }
            // we sort the vector according to the value of the pairs
            sort(pares.begin(), pares.end());
            // we print the contents of the vector
            for (auto& par : pares) {
                cout << par.second << ": " << par.first << endl;
            }
        }

        // closes the file
        void close(){
            file.fileClose();
        }
};

int main(void){
    string fname;

    cout << "enter the file name: ";
    cin >> fname;

    // create an instance of word_counter for the specified file
    word_counter WC(fname);
    // count the words in the file and store them in the map
    WC.countWords();
    // print the words and their counters in alphabetical order
    WC.printWordCount();

    // close the file
    WC.close();

    return 0;
}
