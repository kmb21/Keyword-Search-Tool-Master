/*
  Copyright (c) 2023
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, L. Fontes, L. Meeden, Z. Palmer, A. Soni
  Distributed as course material for Spring 2023
  CPSC 035: Data Structures and Algorithms
*/

#include <algorithm>
#include <fstream>
#include <random>
#include <string>
#include <vector>

#include <UnitTest++/UnitTest++.h>

#include "adts/dictionary.h"
#include "hashFunctions.h"

// This class is used in the hashtable tests below.  To allow referring to its
// hash function via explicit global namespacing (as in "::hash(myBadHash)")
// which some people may do, we must declare it *before* the definition of the
// template.  (This is only necessary because of the explicit namespacing; if we
// say "hash(myBadHash)", then late declarations are allowed.  But implicit
// namespacing requires us to have our declarations in a row before the
// template.
class BadHash {
    public:
    BadHash(int x) {
        this->x = x;
    }
    BadHash() {
        // do nothing
    }
    int x;
    bool operator==(BadHash& h) {
        return x == h.x;
    }
    bool operator!=(BadHash& h) {
        return x != h.x;
    }
    bool operator<(const BadHash& h) const {
        return x < h.x;
    }
};

int hash(BadHash h, int range) { return 0; }

#include "hashTable.h"
#include "keywordSearcher.h"
#include "linearDictionary.h"

using namespace std;

/* helper check function that verifies that a vector is ordered by the second
   part of its pair elements */
template <typename T, typename U>
void CHECK_VECTOR_ORDER_BY_SECOND(std::vector<pair<T, U>>);

/* helper check function that verifies that two vectors are set-equivalent. An
   item is in the first vector iff it is in the second vector.  */
template <typename T>
void CHECK_VECTORS_SET_EQUAL(std::vector<T> first, std::vector<T> second);

SUITE(linearDictionary) {
    TEST(canCreateLinearDictionary) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        delete dictionary;
    }

    TEST(linearDictionaryTypesGeneric) {
        Dictionary<string, int>* dictionary =
            new LinearDictionary<string, int>();
        delete dictionary;
    }

    TEST(linearDictionarySingleMapping) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        dictionary->insert(3, "three");
        CHECK_EQUAL("three", dictionary->get(3));
        delete dictionary;
    }

    TEST(linearDictionaryManyMappings) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        for (int i = 0; i < 4; i++) {
            dictionary->insert(i, to_string(i));
        }
        for (int i = 0; i < 4; i++) {
            CHECK_EQUAL(to_string(i), dictionary->get(i));
        }
        delete dictionary;
    }

    TEST(linearDictionaryDoubleInsertFailure) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        dictionary->insert(0, "0");
        CHECK_THROW(dictionary->insert(0, "1"), std::exception);
        delete dictionary;
    }

    TEST(linearDictionaryMissingUpdateFailure) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        CHECK_THROW(dictionary->update(0, "1"), std::exception);
        delete dictionary;
    }

    TEST(linearDictionaryUpdate) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        dictionary->insert(0, "0");
        dictionary->update(0, "1");
        CHECK_EQUAL("1", dictionary->get(0));
        dictionary->remove(0);
        CHECK_THROW(dictionary->get(0), std::exception);
        delete dictionary;
    }

    TEST(linearDictionaryRemoveException) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        CHECK_THROW(dictionary->remove(0), std::exception);
        delete dictionary;
    }

    TEST(linearDictionaryKeySet) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        for (int i = 0; i < 10; i++) {
            dictionary->insert(i, to_string(i));
        }
        vector<int> keys = dictionary->getKeys();
        for (int i = 0; i < 10; i++) {
            CHECK(std::find(keys.begin(), keys.end(), i) != keys.end());
        }
        delete dictionary;
    }

    TEST(linearDictionaryMoreMappings) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        for (int i = 0; i < 1000; i++) {
            dictionary->insert(i, to_string(i));
        }
        for (int i = 0; i < 1000; i++) {
            CHECK_EQUAL(to_string(i), dictionary->get(i));
        }
        CHECK_EQUAL(1000, dictionary->getSize());
        delete dictionary;
    }

    TEST(linearDictionaryGetKeysAndGetItems) {
        Dictionary<int, string>* dictionary =
            new LinearDictionary<int, string>();
        vector<int> values, keys, itemKeys;
        default_random_engine generator;
        uniform_int_distribution<int> distribution(-2000000000, 2000000000);
        int i = 0;
        while (i < 1000) {
            int r = distribution(generator);
            if (!dictionary->contains(r)) {
                values.push_back(r);
                dictionary->insert(r, to_string(r));
                i++;
            }
        }

        keys = dictionary->getKeys();
        vector<pair<int, string>> items = dictionary->getItems();
        for (i = 0; i < 1000; i++) {
            itemKeys.push_back(items[i].first);
        }
        sort(values.begin(), values.end());
        sort(keys.begin(), keys.end());
        sort(itemKeys.begin(), itemKeys.end());
        if (keys.size() == 1000 && itemKeys.size() == 1000) {
            for (i = 0; i < 1000; i++) {
                CHECK_EQUAL(values[i], keys[i]);
                CHECK_EQUAL(values[i], itemKeys[i]);
            }
        } else {
            CHECK_EQUAL(keys.size(), 1000);
            CHECK_EQUAL(itemKeys.size(), 1000);
        }
        delete dictionary;
    }
}

SUITE(hashTable) {
    TEST(canCreateHashTable) {
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        delete dictionary;
    }

    TEST(hashTableTypesGeneric) {
        Dictionary<string, int>* dictionary = new HashTable<string, int>();
        delete dictionary;
    }

    TEST(hashTableSingleMapping) {
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        dictionary->insert(3, "three");
        CHECK_EQUAL("three", dictionary->get(3));
        delete dictionary;
    }

    TEST(hashTableManyMappings) {
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        for (int i = 0; i < 4; i++) {
            dictionary->insert(i, to_string(i));
        }
        for (int i = 0; i < 4; i++) {
            CHECK_EQUAL(to_string(i), dictionary->get(i));
        }
        delete dictionary;
    }

    TEST(hashTableCollision) {
        // Unless the table has a hefty, massive starting capacity, one of these
        // passes will trigger a collision.
        for (int inc = 1; inc <= 20; inc++) {
            Dictionary<int, string>* dictionary = new HashTable<int, string>();
            for (int i = 0; i < inc * 3; i += inc) {
                dictionary->insert(i, to_string(i));
            }
            for (int i = 0; i < inc * 3; i += inc) {
                CHECK_EQUAL(to_string(i), dictionary->get(i));
            }
            delete dictionary;
        }
    }

    TEST(doubleInsertFailure) {
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        dictionary->insert(0, "0");
        CHECK_THROW(dictionary->insert(0, "1"), std::exception);
        delete dictionary;
    }

    TEST(missingUpdateFailure) {
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        CHECK_THROW(dictionary->update(0, "1"), std::exception);
        delete dictionary;
    }

    TEST(hashTableUpdate) {
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        dictionary->insert(0, "0");
        dictionary->update(0, "1");
        CHECK_EQUAL("1", dictionary->get(0));
        dictionary->remove(0);
        CHECK_THROW(dictionary->get(0), std::exception);
        delete dictionary;
    }

    TEST(hashTableRemoveException) {
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        CHECK_THROW(dictionary->remove(0), std::exception);
        delete dictionary;
    }

    TEST(hashTableKeySet) {
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        for (int i = 0; i < 10; i++) {
            dictionary->insert(i, to_string(i));
        }
        vector<int> keys = dictionary->getKeys();
        for (int i = 0; i < 10; i++) {
            CHECK(std::find(keys.begin(), keys.end(), i) != keys.end());
        }
        delete dictionary;
    }

    TEST(hashTableLotsOfMappings) {
        cout << "Beginning hashTableLotsOfMappings" << endl;
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        for (int i = 0; i < 100000; i++) {
            dictionary->insert(i, to_string(i));
        }
        for (int i = 0; i < 100000; i++) {
            CHECK_EQUAL(to_string(i), dictionary->get(i));
        }
        CHECK_EQUAL(100000, dictionary->getSize());
        delete dictionary;
        cout << "Finished hashTableLotsOfMappings" << endl;
    }

    TEST(hashTableLotsOfRandomMappings) {
        cout << "Beginning hashTableLotsOfRandomMappings" << endl;
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        vector<int> values;
        default_random_engine generator;
        uniform_int_distribution<int> distribution(-2000000000, 2000000000);
        int i = 0;
        while (i < 100000) {
            int r = distribution(generator);
            if (!dictionary->contains(r)) {
                values.push_back(r);
                dictionary->insert(r, to_string(r));
                i++;
            }
        }
        for (i = 0; i < 100000; i++) {
            CHECK_EQUAL(to_string(values[i]), dictionary->get(values[i]));
        }
        CHECK_EQUAL(100000, dictionary->getSize());
        delete dictionary;
        cout << "Finished hashTableLotsOfRandomMappings" << endl;
    }

    TEST(hashTableGetKeysAndGetItems) {
        Dictionary<int, string>* dictionary = new HashTable<int, string>();
        vector<int> values, keys, itemKeys;
        default_random_engine generator;
        uniform_int_distribution<int> distribution(-2000000000, 2000000000);
        int i = 0;
        while (i < 1000) {
            int r = distribution(generator);
            if (!dictionary->contains(r)) {
                values.push_back(r);
                dictionary->insert(r, to_string(r));
                i++;
            }
        }

        keys = dictionary->getKeys();
        vector<pair<int, string>> items = dictionary->getItems();
        for (i = 0; i < 1000; i++) {
            itemKeys.push_back(items[i].first);
        }
        sort(values.begin(), values.end());
        sort(keys.begin(), keys.end());
        sort(itemKeys.begin(), itemKeys.end());
        if (keys.size() == 1000 && itemKeys.size() == 1000) {
            for (i = 0; i < 1000; i++) {
                CHECK_EQUAL(values[i], keys[i]);
                CHECK_EQUAL(values[i], itemKeys[i]);
            }
        } else {
            CHECK_EQUAL(keys.size(), 1000);
            CHECK_EQUAL(itemKeys.size(), 1000);
        }
        delete dictionary;
    }

//     TEST(hashTableWithBadHash) {
//         cout << "Beginning hashTableWithBadHash" << endl;
//         Dictionary<BadHash, int>* dictionary = new HashTable<BadHash, int>();
//         for (int i = 0; i < 1000; i++) {
//             dictionary->insert(BadHash(i), i);
//         }
//         for (int i = 0; i < 1000; i++) {
//             REQUIRE CHECK_EQUAL(i, dictionary->get(BadHash(i)));
//         }
//         dictionary->remove(0);
//         for (int i = 1; i < 1000; i++) {
//             REQUIRE CHECK_EQUAL(i, dictionary->get(BadHash(i)));
//         }
//         delete dictionary;
//         cout << "Finished hashTableWithBadHash" << endl;
//     }
}

#define TEST_KS(name, file, word, answer)                                      \
    TEST(keywordSearch##name) {                                                \
        cout << "Beginning keywordSearch"#name << endl;                        \
        KeywordSearcher ks;                                                    \
        ks.loadWords("test_data/" #file ".txt");                               \
        auto result = ks.search(#word);                                        \
        CHECK_VECTOR_ORDER_BY_SECOND(result);                                  \
        CHECK_VECTORS_SET_EQUAL(vector<pair<int, int>> answer, result);        \
        cout << "Finished keywordSearch"#name << endl;                         \
    }

#define TEST_KS3(name, file, word1, answer1, word2, answer2, word3, answer3)   \
    TEST(keywordSearch##name) {                                                \
        cout << "Beginning keywordSearch"#name << endl;                        \
        KeywordSearcher ks;                                                    \
        ks.loadWords("test_data/" #file ".txt");                               \
        auto result = ks.search(#word1);                                       \
        CHECK_VECTOR_ORDER_BY_SECOND(result);                                  \
        CHECK_VECTORS_SET_EQUAL(vector<pair<int, int>> answer1, result);       \
        result = ks.search(#word2);                                            \
        CHECK_VECTOR_ORDER_BY_SECOND(result);                                  \
        CHECK_VECTORS_SET_EQUAL(vector<pair<int, int>> answer2, result);       \
        result = ks.search(#word3);                                            \
        CHECK_VECTOR_ORDER_BY_SECOND(result);                                  \
        CHECK_VECTORS_SET_EQUAL(vector<pair<int, int>> answer3, result);       \
        cout << "Finished keywordSearch"#name << endl;                         \
    }

SUITE(keywordSearch) {
    TEST(keywordSearchEmpty) {
        KeywordSearcher ks;
        CHECK_VECTORS_SET_EQUAL({}, ks.search("word"));
    }

    TEST(keywordSearchLoadFile) {
        KeywordSearcher ks;
        ks.loadWords("test_data/small.txt");
    }

    TEST_KS(SingleResult, small, different, ({{2, 1}}))

    TEST_KS(MultipleResult, small, page, ({{2, 3}, {1, 1}, {3, 1}}))

    TEST_KS(OverlappingResult, small, more, ({{2, 1}, {4, 1}}))

    TEST_KS3(ManyOps, small,
        different, ({{2, 1}}),
        page, ({{2, 3}, {1, 1}, {3, 1}}),
        more, ({{2, 1}, {4, 1}}))

    TEST_KS(Cap10Result, ATranslationGuideFromPython2ToC++, python,
        ({  {9, 9},
            {8, 9},
            {5, 8},
            {15, 6},
            {17, 6},
            {31, 6},
            {58, 6},
            {54, 6},
            {23, 6},
            {7, 6}
        }))

    TEST_KS(ShortResult, ATranslationGuideFromPython2ToC++, module,
            ({{63, 2}, {67, 2}, {9, 2}, {10, 1}, {22, 1}}))

    TEST_KS3(ManyOpsTranslation, ATranslationGuideFromPython2ToC++,
        python, ({
            {9, 9},
            {8, 9},
            {5, 8},
            {15, 6},
            {17, 6},
            {31, 6},
            {58, 6},
            {54, 6},
            {23, 6},
            {7, 6}
        }),
        module, ({{63, 2}, {67, 2}, {9, 2}, {10, 1}, {22, 1}}),
        trampoline, ({}))

    TEST_KS(Cap10Result2, DataStructuresAndAlgorithmAnalysis, algorithm,
            ({{95, 17},
              {83, 13},
              {430, 14},
              {516, 15},
              {80, 18},
              {515, 14},
              {109, 12},
              {36, 13},
              {96, 12},
              {424, 13}}))

    TEST_KS(NoResult, DataStructuresAndAlgorithmAnalysis, mushroom, ({}))

    TEST_KS3(RepetitionDataStructures, DataStructuresAndAlgorithmAnalysis,
        algorithm, ({{95, 17},
                     {83, 13},
                     {430, 14},
                     {516, 15},
                     {80, 18},
                     {515, 14},
                     {109, 12},
                     {36, 13},
                     {96, 12},
                     {424, 13}}),
        mushroom, {},
        algorithm, ({{95, 17},
                     {83, 13},
                     {430, 14},
                     {516, 15},
                     {80, 18},
                     {515, 14},
                     {109, 12},
                     {36, 13},
                     {96, 12},
                     {424, 13}}))
}

int main() {
    return UnitTest::RunAllTests();
}

string show(int n) {
    return to_string(n);
}
string show(float n) {
    return to_string(n);
}
string show(long n) {
    return to_string(n);
}
string show(double n) {
    return to_string(n);
}
string show(bool b) {
    return b ? "true" : "false";
}
string show(string s) {
    return s;
}

template <typename T, typename U> string show(pair<T, U> p) {
    return "(" + show(p.first) + ", " + show(p.second) + ")";
}

template <typename T, typename U>
void CHECK_VECTOR_ORDER_BY_SECOND(std::vector<pair<T, U>> vec) {
    if (vec.size() > 0) {
        for (int i = 1; i < vec.size(); i++) {
            if (vec[i - 1].second < vec[i].second) {
                CHECK_EQUAL(show(vec[i]) + " to appear before " +
                                show(vec[i - 1]) + " in vector",
                            show(vec[i - 1]) + " appearing before " +
                                show(vec[i]) + " in vector");
            }
        }
    }
}

template <typename T>
void CHECK_VECTORS_SET_EQUAL(std::vector<T> first, std::vector<T> second) {
    auto expected = first;
    auto actual = second;
    CHECK_EQUAL(expected.size(), actual.size());
    for (int i = 0; i < expected.size(); i++) {
        if (std::find(actual.begin(), actual.end(), expected[i]) ==
            actual.end()) {
            CHECK_EQUAL(show(expected[i]) + " to be in vector", "not");
        }
    }
    for (int i = 0; i < actual.size(); i++) {
        if (std::find(expected.begin(), expected.end(), actual[i]) ==
            expected.end()) {
            CHECK_EQUAL(show(actual[i]) + " not to be in vector",
                        show(actual[i]) + " in vector");
        }
    }
}
