#include <mutex>
#include <iostream>
#include "DuplicateChecker.hpp"

DuplicateChecker::DuplicateChecker () {
    root = std::make_shared<DuplicateChecker::TrieNode>();
}

struct DuplicateChecker::TrieNode {
    std::string val = "";
    bool is_end = false;
    std::vector<std::shared_ptr<DuplicateChecker::TrieNode>> children;
};

/*
bool DuplicateChecker::binary_search (std::vector<std::shared_ptr<TrieNode> >& children, std::string key) {
    for (auto& i : children) {
        if (i->val == key) {
            return true;
        }
    }

    return false;
}
*/

bool DuplicateChecker::binary_search (std::vector<std::shared_ptr<TrieNode> >& children, std::string key) {
    if (children.size() == 0) { return false; }
    int a = 0, b, c = children.size() - 1;

    while (a < c) {
        int b = (c + a) / 2;

        if (children[b]->val == key) {
            return true;
        } else if (children[b]->val < key) {
            a = b + 1;
        }
        else {
            c = b;
        }
    }
    return children[a]->val == key || children[c]->val == key;
}



int DuplicateChecker::lower_bound (std::vector<std::shared_ptr<TrieNode> >& children, std::string key) {
    int a = 0, b, c = children.size();
 
    while (a < c) {
        b = a + (c - a) / 2;

        if (children[b]->val == key) {
            return b;
        } else if (children[b]->val < key) {
            a = b + 1;
        }
        else {
            c = b;
        }
    }

    return a < children.size() - 1 ? a : children.size() - 1;
}


// Prevent concurrent modification issues with the trie
std::mutex dc_mutex;

void DuplicateChecker::insert (std::vector<std::string>& traits) {
    //dc_mutex.lock(); unnecessary because insert() is only called by search(), which already has dc_mutex locked
    
    std::shared_ptr<DuplicateChecker::TrieNode> node (root);

    // For further optimization, add linear search for vectors below a certain size?
    for (std::string& i : traits) {
        // position of the closest element to i in node->children
        int pos_to_insert = lower_bound(node->children, i);

        // see if the given trait (i) is in node->children
        if (binary_search(node->children, i)) {
            node = node->children.at(pos_to_insert);
            continue;
        }

        // If the given trait isn't in node->children, add it
        auto new_node = std::make_shared<DuplicateChecker::TrieNode>();
        new_node->val = i;
        new_node->is_end = false;
        node->children.insert(node->children.begin() + pos_to_insert, new_node);

        node = node->children.at(pos_to_insert);
    }

    node->is_end = true;

    //dc_mutex.unlock();
}

bool DuplicateChecker::search (std::vector<std::string>& traits) {
    dc_mutex.lock();

    std::shared_ptr<DuplicateChecker::TrieNode> node (root);

    for (std::string& i : traits) {
        // see if the given trait (i) isn't in node->children
        if (!binary_search(node->children, i)) {
            insert(traits);
            dc_mutex.unlock();
            return false;
        }

        // if it is, set node to that trait's node
        int pos_of_match = lower_bound(node->children, i);
        node = node->children.at(pos_of_match);
    }

    dc_mutex.unlock();
    return node->is_end;
}