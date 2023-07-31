#pragma once

/*
  Copyright (c) 2023
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, L. Fontes, L. Meeden, Z. Palmer, A. Soni
  Distributed as course material for Spring 2023
  CPSC 035: Data Structures and Algorithms
*/

#include <string>

using std::string;

// NOTE:
// This file declares a number of different functions all called "hash".  C++
// will determine which function you mean to call based upon the type of the
// argument you give; that is, if you call hash and pass it an int, you'll get
// the int version of hash.  This behavior is called "overloading".

/**
 * Generates a hash code for an integer.
 * @param data The integer to hash.
 * @param range The range of integers to hash to.
 * @return The hash of the provided integer.
 */
int hash(int data, int range);

/**
 * Generates a hash code for a string.
 * @param data The string to hash.
 * @param range The range of integers to hash to.
 * @return The hash of the provided string.
 */
int hash(string data, int range);

// NOTE:
// You could add as many hash functions as you wanted here.  If you wanted to
// be able to hash objects of some class, for instance, you could add such a
// hash function here.  You *must* have a hash function *somewhere* for each
// type of key your HashTable will have, though.
