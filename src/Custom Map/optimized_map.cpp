#include "optimized_map.h"

#include <string.h>

optimized_map::optimized_map() {
    m_hashArray = new Tree*[500];
    m_hashArraySize = 500;
}

//int& optimized_map::operator[] (int key) {
//    int index = hash(key);
//
//    // Increase size to index if hash array size too small
//    if (index > m_hashArraySize) {
//        Tree** newArray = new Tree*[index];
//        memcpy(m_hashArray, newArray, sizeof m_hashArray);
//        delete m_hashArray;
//        m_hashArray = newArray;
//        m_hashArraySize = index;
//    }
//
//    // if hash array entry not empty
//    if (m_hashArray[index] != nullptr) {
//        
//    }
//
//
//}

unsigned int optimized_map::hash(int key) {
    /*key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;*/
    return key / 3;
}
