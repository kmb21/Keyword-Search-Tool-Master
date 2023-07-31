/*
  Copyright (c) 2023
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, L. Fontes, L. Meeden, Z. Palmer, A. Soni
  Distributed as course material for Spring 2023
  CPSC 035: Data Structures and Algorithms
*/

#include <stdexcept>

template <typename K, typename V> LinearDictionary<K, V>::LinearDictionary() {
    
}

template <typename K, typename V> LinearDictionary<K, V>::~LinearDictionary() {
    
}

template <typename K, typename V> int LinearDictionary<K, V>::getSize() {
    return this->myVector.size();
}
template <typename K, typename V> bool LinearDictionary<K, V>::isEmpty() {
    return this->myVector.size() == 0;
}

template <typename K, typename V>
void LinearDictionary<K, V>::insert(K key, V value) {
    for (int i = 0; i < this->myVector.size(); i++){
        if (key == this->myVector[i].first){
            throw std::runtime_error("LinearDictionary<K, V> :: insert, no duplicates allowed");
        }
    }
    pair<K, V> my_pair(key,value);
    this->myVector.push_back(my_pair);
    
}

template <typename K, typename V>
void LinearDictionary<K, V>::update(K key, V value) {
    bool finish = true;
    if (!this->contains(key)){
        throw std::runtime_error("LinearDictionary<K, V> :: update, key not found");
    }
    for (int i = 0; i < this->myVector.size(); i++){
        if (key == this->myVector[i].first){
            this->myVector[i].second = value;
        }
    }
    
}

template <typename K, typename V> V LinearDictionary<K, V>::get(K key) {
    for (int i = 0; i < this->myVector.size(); i++){
        if(this->myVector[i].first == key){
            return this->myVector[i].second;
        }
    }
    throw std::runtime_error("LinearDictionary<K, V> :: get, key not found");
}

template <typename K, typename V> bool LinearDictionary<K, V>::contains(K key) {
    for (int i = 0; i < this->myVector.size(); i++){
        if(this->myVector[i].first == key){
            return true;
        }
    }
    return false;
}

template <typename K, typename V> void LinearDictionary<K, V>::remove(K key) {
    bool finish = true;
    for (int i = 0; i < this->myVector.size(); i++){
        if(this->myVector[i].first == key){
            this->myVector.erase(myVector.begin()+i,myVector.begin()+i +1);
            finish = false;
        }
    }
    if (finish){
        throw std::runtime_error("LinearDictionary<K, V> :: remove, key not found");
    }
}

template <typename K, typename V> vector<K> LinearDictionary<K, V>::getKeys() {
    vector<K> ourVector;
    for (int i = 0; i<this->myVector.size(); i++){
        ourVector.push_back(this->myVector[i].first);
    }
    return ourVector;
}

template <typename K, typename V>
vector<pair<K, V>> LinearDictionary<K, V>::getItems() {
    return this->myVector;
}
