/*
  Copyright (c) 2023
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, L. Fontes, L. Meeden, Z. Palmer, A. Soni
  Distributed as course material for Spring 2023
  CPSC 035: Data Structures and Algorithms
*/

#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "hashFunctions.h"

using std::pair;
using std::runtime_error;
using std::vector;

template <typename K, typename V> HashTable<K, V>::HashTable() {
    
    this->maxload= 0.75;
    this->capacity= 10;
    this->size = 0;
    this->table= new LinearDictionary<K, V>[this->capacity];
}

template <typename K, typename V>
HashTable<K, V>::HashTable(float maxLoadFactor) {

    this->table= new LinearDictionary<K, V>[this->capacity];
    this->maxload = maxLoadFactor;
    this->capacity= 10;
    this->size = 0;

}
template <typename K, typename V> HashTable<K, V>::~HashTable() {
    
    delete [] this->table;
    this->table = nullptr;
}

template <typename K, typename V> int HashTable<K, V>::getSize() {
    
    return this->size;
}

template <typename K, typename V> bool HashTable<K, V>::isEmpty() {
    return this->size == 0;
    
}

template <typename K, typename V> void HashTable<K, V>::insert(K key, V value) {

    int index = hash(key, this->capacity);
    this->table[index].insert(key, value);
    this->size++;
    float load = float(this->size)/this->capacity;
    if (load > this->maxload){
        expandCapacity();
    }
    
}
template <typename K, typename V> void HashTable<K, V>::expandCapacity(){

    LinearDictionary<K, V>* new_table= new LinearDictionary<K, V>[2*this->capacity];
    vector<pair<K, V>> our_item = this->getItems();
    delete [] this->table;
    this->capacity= 2*this->capacity;
    this->table= new_table;
    this->size = 0;
    for(int i=0; i<our_item.size(); i++){
        this->insert(our_item[i].first, our_item[i].second);
    }
    
}


template <typename K, typename V> void HashTable<K, V>::update(K key, V value) {  

    int index = hash(key, this->capacity);
    this->table[index].update(key, value);
        
}

template <typename K, typename V> V HashTable<K, V>::get(K key) {

    int index = hash(key, this->capacity);
    return this->table[index].get(key);  
       
}

template <typename K, typename V> bool HashTable<K, V>::contains(K key) {

    int index = hash(key, this->capacity);
    return this->table[index].contains(key);  
       
}


template <typename K, typename V> void HashTable<K, V>::remove(K key) {

    int index = hash(key, this->capacity);
    return this->table[index].remove(key);
    this->size--;  
       
}


template <typename K, typename V> vector<K> HashTable<K, V>::getKeys() {
    vector<K> tablekeys;
    for (int i = 0; i<this->capacity; i++){
        vector<K> my_keys= this->table[i].getKeys();
        for (int j=0; j<my_keys.size();j++){
            tablekeys.push_back(my_keys[j]);
        }
    } 
    return tablekeys;
       
}


template <typename K, typename V>
vector<pair<K, V>> HashTable<K, V>::getItems() {
    vector<pair<K, V>> tableItems;
    for (int i = 0; i<this->capacity; i++){
        vector<pair<K,V>> my_items= this->table[i].getItems();
        for (int j=0; j<my_items.size();j++){
            tableItems.push_back(my_items[j]);
        }
    }
    return tableItems;
}

// TODO: put any other definitions here





