#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>
#include <regex>

bool isSpecialCharacter(char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

bool compareWordOccurrences(const std::pair<std::string, std::pair<int, std::set<int>>>& a,
    const std::pair<std::string, std::pair<int, std::set<int>>>& b) {
    return a.second.first > b.second.first;
}

std::regex urlRegex(R"(\b[a-zA-Z]+\.[a-zA-Z]+\b)");
std::regex shortUrlRegex(R"((?:\b|\.)(?:bit\.ly|goo\.gl)\S*)");

int main() {
    std::ifstream file("Tekstas.txt");
    std::ofstream rezultatai("Rezultatai.txt");
    if (!file) {
        std::cerr << "Nepavyko atidaryti failo" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::pair<std::string, std::pair<int, std::set<int>>>> wordOccurrences;
    std::vector<std::string> urls;

    int lineNumber = 1;
    while (std::getline(file, line)) {
        std::string word;
        std::istringstream iss(line);
        while (iss >> word) {
            word.erase(std::remove_if(word.begin(), word.end(), isSpecialCharacter), word.end());

            if (!word.empty()) {
                std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
                    return std::tolower(c);
                    });

                auto it = std::find_if(wordOccurrences.begin(), wordOccurrences.end(), [&](const auto& entry) {
                    return entry.first == word;
                    });

                if (std::regex_match(word, urlRegex)|| std::regex_match(word, shortUrlRegex)) {
                    urls.push_back(word);
                }

                if (it != wordOccurrences.end()) {
                    it->second.second.insert(lineNumber);
                    it->second.first++;
                }
                else {
                    wordOccurrences.emplace_back(word, std::make_pair(1, std::set<int>{lineNumber}));
                }
            }
        }
        lineNumber++;
    }

    std::sort(wordOccurrences.begin(), wordOccurrences.end(), compareWordOccurrences);

    rezultatai << "Cross-Reference Table:" << std::endl;
    for (const auto& entry : wordOccurrences) {
        if (entry.second.first > 1)
            rezultatai << entry.first << ": " << entry.second.first << " pasikartojimas(-ai) - ";
        for (int lineNumber : entry.second.second) {
            if (entry.second.first > 1)
                rezultatai << lineNumber << " ";
        }
        if(entry.second.first > 1)
        rezultatai << std::endl;
    }
    rezultatai << std::endl;

    rezultatai << "Detected URLs:" << std::endl;
    for (const std::string& url : urls) {
        rezultatai << url << std::endl;
    }

    return 0;
}
