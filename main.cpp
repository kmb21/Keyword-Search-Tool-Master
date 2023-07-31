/*
  Copyright (c) 2023
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, L. Fontes, L. Meeden, Z. Palmer, A. Soni
  Distributed as course material for Spring 2023
  CPSC 035: Data Structures and Algorithms
*/

#include <iostream>
#include <string>
#include <vector>

#include "keywordSearcher.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2){
    throw runtime_error("Insufficient arguments, command line expects two arguments");
  }
  string word;
  string name = argv[1];
  KeywordSearcher my_searcher;
  
  my_searcher.loadWords(name);
  cout<<"Please enter a search word or '!' to quit: ";
  cin>>word;
  while(word!= "!"){
    vector<pair<int, int>>result_vector= my_searcher.search(word);
    if(result_vector.size() != 0){
      cout<<"The word priority appears in the file..."<<endl;
      for(int i= 0; i<result_vector.size(); i++){
        cout<<"   on page " << result_vector[i].first << "(" << result_vector[i].second << " occurrences)"<<endl;
      }
      cout << endl;
    }
    else{
      cout<<"The word " + word+  " does not appear in the provided file"<<endl;
      cout << endl;
    }
    
    cout<<"Please enter a search word or '!' to quit: ";
    cin>>word;
    }
    cout <<endl;
    cout << "Goodbye!" << endl;
  return 0;
    
}
