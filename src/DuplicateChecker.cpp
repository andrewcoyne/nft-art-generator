#include <mutex>
#include <iostream>
#include "DuplicateChecker.hpp"

// Prevent concurrent modification issues with the hash map
std::mutex dc_mutex;

DuplicateChecker::DuplicateChecker () { }

void DuplicateChecker::insert (std::string& traits) {
    trait_map.insert({traits, 1});
}

bool DuplicateChecker::search (std::vector<std::string>& traits) {
    dc_mutex.lock();

    // Combine traits into one string
    std::string trait_str;
    for (std::string& i : traits) {
        trait_str += i;
    }

    // True if the provided trait combination has been used before, false if not
    bool res = trait_map.count(trait_str) != 0;

    if (!res) {
        insert(trait_str);
    }

    dc_mutex.unlock();

    return res;
}