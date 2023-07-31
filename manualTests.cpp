/*
  Copyright (c) 2023
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, L. Fontes, L. Meeden, Z. Palmer, A. Soni
  Distributed as course material for Spring 2023
  CPSC 035: Data Structures and Algorithms
*/

#include <string>
#include "keywordSearcher.h"

using namespace std;

int main(int argc, char** argv) {
    // You can use this main to experiment with the code you have written
    KeywordSearcher ks;                                                    \
    ks.loadWords("test_data/small.txt");
    // cout << ks.outer_table->get("words")->get(1) << endl;
    std::vector<std::pair<int, int>> vec = ks.search("words");

    for (auto v : vec) {
      cout << v.first << " " << v.second << endl;
    }

    return 0;
}
