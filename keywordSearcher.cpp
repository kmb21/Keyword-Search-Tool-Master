/*
  Copyright (c) 2023
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, L. Fontes, L. Meeden, Z. Palmer, A. Soni
  Distributed as course material for Spring 2023
  CPSC 035: Data Structures and Algorithms
*/

#include "keywordSearcher.h"
#include <stdexcept>
#include <fstream>
using namespace std;

KeywordSearcher::KeywordSearcher() {

    this->outer_table= new HashTable<std::string, HashTable<int,int>*>();
    
}

KeywordSearcher::~KeywordSearcher() {
    vector<pair<std::string, HashTable<int,int>*>> items = this->outer_table->getItems();
    for (int i = 0; i<items.size();i++){
        delete items[i].second;
    }
    delete this->outer_table;
    // TODO: KeywordSearcher::~KeywordSearcher
}

void KeywordSearcher::loadWords(string filename) {
    std::ifstream myfile;
    std::string word;
    int page_num;
    myfile.open(filename);

    if(!myfile.is_open()){
        throw runtime_error("file "+ filename + " failed to open" );
    }
    myfile>>word;
    while(!myfile.eof()){
        if (word=="$$@@$$PAGE:"){
            myfile>>word;
            page_num = stoi(word);
            // cout << page_num << endl;
            myfile>>word;
            continue;
        }
        if (!this->outer_table->contains(word)){
            HashTable<int, int>* inner_table= new HashTable<int, int>();
            this->outer_table->insert(word, inner_table);
        }
        if (this->outer_table->get(word)->contains(page_num)){
            // cout << page_num << endl;
            this->outer_table->get(word)->update(page_num, this->outer_table->get(word)->get(page_num)+1);
        }
        else{
            // cout << page_num << endl;
            this->outer_table->get(word)->insert(page_num,1);
        }
        myfile>>word;
    }
    
}

vector<pair<int, int>> KeywordSearcher::search(string word) {
    if( !outer_table->contains(word)){
        vector <pair<int,int>> empty;
        return empty;
    }
    
    HashTable<int,int>* innerTable= this->outer_table->get(word);
    vector<pair<int,int>>occur_vector= innerTable->getItems();
    for (int i= 0; i<occur_vector.size();i++ ){
        std::swap(occur_vector[i].first, occur_vector[i].second);
    }
 
    STLMaxPriorityQueue<int, int> my_queue(occur_vector);
    vector<pair<int,int>>temp_vector;
    int queue_size = my_queue.getSize();
    
    if (queue_size > 10){
        for(int i= 0; i<10; i++){
            int temp_prio= my_queue.peekPriority();
            pair<int, int> result(my_queue.remove(),temp_prio);
            temp_vector.push_back(result);
        }
    }else{
        for(int i= 0; i<queue_size; i++){
            int temp_prio= my_queue.peekPriority();
            pair<int, int> result(my_queue.remove(),temp_prio);
            temp_vector.push_back(result);
        }
    }
    
    return temp_vector;
}
