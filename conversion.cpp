#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>

char SEPARATE_WORD_PLACEMENT = '#';
char SEPARATE_RESULT = '>';
char SEPARATE_INDEX = '<';

bool findValue(const std::string& value, const std::map<std::string, std::string>& result) {
    return result.find(value) != result.end();
}

bool findValueVct(const std::string& value, const std::map<std::string, std::vector<int>>& result) {
    return result.find(value) != result.end();
}

void coutDicoStr(const std::map<std::string, std::string>& dico) {
    for (const auto& pair : dico) {
        std::cout << "Key: " << pair.first << " - Valeur: " << pair.second << std::endl;
    }
}

void coutVectorInt(const std::vector<int>& vecteur) {
    for (int i = 0; i < size(vecteur); ++i) {
        std::cout << vecteur[i] << " ; ";
    }
}

void coutDicoInt(const std::map<std::string, std::vector<int>>& dico) {
    //affiche un dictionnaire possedeant string et vecteur[int]
    for (const auto& pair : dico) {
        std::cout << "Key: " << pair.first << " - Valeur: ";
        coutVectorInt(pair.second);
        std::cout << std::endl;
    }
}

std::map<std::string, std::string> separateWordAndSaveIndex(std::string& message) {
    // separateWordAndSaveIndex : 
    //      separe les mots quand il detecte un espace
    //      ajoute les index ou il a trouve les valuers (separes par char SEPARATE_WORD_PLACEMENT = '#';
    //                                                               char SEPARATE_RESULT = '>';
    //                                                               char SEPARATE_INDEX = '<';)
    message.push_back(' ');
    message.push_back('E');
    message.push_back('N');
    message.push_back('D');

    int last_index = 0;
    std::string tempWord = "";
    std::map<std::string, std::string> result;
    int placement = 0;
    for (int i = 0; i < message.length(); i++) {
        if (message[i] == ' ') {
            //ajoute toutes les letres de j -> i pour faire le mot tempWord = mot
            for (int j = last_index; j < i; j++) {tempWord += message[j];}
            //verifie l'existence de la cle si elle existe pas alors il ajoute une chaine vide
            if (!findValue(tempWord, result)) { result[tempWord] = ""; }
            //ajoute le placement du mot
            result[tempWord] += std::to_string(placement) + SEPARATE_INDEX;
            last_index = i + 1;
            tempWord = "";
            placement += 1;
        }
    }
    return result;
}
std::string Compresser(std::string& message) {
    auto start = std::chrono::high_resolution_clock::now();

    std::map<std::string, std::string> dico = separateWordAndSaveIndex(message);
    std::string result;
    for (auto& pairs : dico) {
        if (!pairs.second.empty()) {
            pairs.second.pop_back();
        }
        result += pairs.first + SEPARATE_WORD_PLACEMENT + pairs.second + SEPARATE_RESULT;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    // Stats
    std::cout << std::endl << "--- STATS" << std::endl;
    std::cout << "Taille du message avant : " << message.size() * 8 << " bits" << std::endl;
    std::cout << "Taille du message apres : " << result.size() * 8 << " bits" << std::endl;
    std::cout << "Temps de compression : " << elapsed.count() << " ms" << std::endl;
    std::cout << "Amelioration : " << (int(message.size() * 8) - int(result.size() * 8 )) << " bits" << std::endl;
    std::cout << "--- STATS" << std::endl;
    // Fin Stats

    return result;
}


std::vector<std::string> SeparateWord(std::string& compressed_message) {
    //Prends le message compresse et separe les mots qu'il detecte
    int last_index = 0;
    std::string tempWord = "";
    std::vector<std::string> separatedWord;
    for (int i = 0; i < compressed_message.length(); ++i) {
        if (compressed_message[i] == SEPARATE_RESULT) {
            for (int j = last_index; j < i; ++j) {
                if (compressed_message[j] == SEPARATE_WORD_PLACEMENT) {break;}
                if (compressed_message[j] == SEPARATE_RESULT) { continue; }
                tempWord.push_back(compressed_message[j]);
            }
            separatedWord.push_back(tempWord);
            tempWord = "";
            last_index = i;
        }
    }
    return separatedWord;
}

std::map<std::string, std::vector<int>> GenerateMap(std::string& compressed_message){
    std::vector<std::string> word_list = SeparateWord(compressed_message);
    int last_index = 0;
    int temp_index = 0;
    std::string tempNumber = "";
    std::map<std::string, std::vector<int>> result;
    for (int i = 0; i < size(compressed_message); ++i) {

        if (compressed_message[i] == SEPARATE_RESULT) {
            
            for (int j = last_index; j < i; ++j) {
                if (compressed_message[j] == SEPARATE_WORD_PLACEMENT) {
                    
                    for (int u = j; u < i; ++u) {
                        if (compressed_message[u] == SEPARATE_RESULT) { break; }
                        if (compressed_message[u] == SEPARATE_WORD_PLACEMENT) { continue; }
                        
                        for (int y = u; y < i; ++y) {
                            if (compressed_message[y] == SEPARATE_INDEX) { break; }
                            if (compressed_message[y] == SEPARATE_RESULT) { continue; }
                            
                            tempNumber += compressed_message[y];
                            u += 1;
                        }
                        if (!findValueVct(word_list[temp_index], result)) {
                            result[word_list[temp_index]] = {};
                        }
                        /*result[word_list[temp_index]].push_back()
                        temp_index += 1;*/
                        if (tempNumber.length() > 0) {
                            result[word_list[temp_index]].push_back(std::stoi(tempNumber));
                        }
                        
                        tempNumber = "";
                    }
                    temp_index += 1;
                }
            }
            last_index = i;
        }
    }
    return result;
}

std::string DeCompresser(std::string& compressed_message) {
    std::map<int, std::string> sorted_pairs_second;
    std::map<std::string, std::vector<int>> result = GenerateMap(compressed_message);
    for (const auto pairs : result) {
        for (int i = 0; i < size(pairs.second); ++i) {
            sorted_pairs_second[pairs.second[i]] = pairs.first;
        }
    }
    std::string message = "";
    for (const auto pair : sorted_pairs_second) {
        message += pair.second + " ";
    }
    return message;
}


int main()
{
    std::string message = "TEST TEST REPETITION REPETITION REPETITION REPETITION REPETITION REPETITION  TEST TEST TEST TEST";
    std::string x = Compresser(message);
    std::string result = DeCompresser(x);
}
