#include <mutex>
#include <algorithm>
#include "DuplicateChecker.hpp"

DuplicateChecker::DuplicateChecker () {
    root = std::make_shared<DuplicateChecker::TrieNode>();
}

struct DuplicateChecker::TrieNode {
    std::string val = "";
    bool is_end = false;
    std::vector<std::shared_ptr<DuplicateChecker::TrieNode>> children;
};

// Prevent concurrent modification issues with the trie
std::mutex dc_mutex;

void DuplicateChecker::insert (std::vector<std::string>& traits) {
    //dc_mutex.lock(); unnecessary because insert() is only called by search(), which already has dc_mutex locked
    
    std::shared_ptr<DuplicateChecker::TrieNode> node (root);

    // For further optimization, add linear search for vectors below a certain size?
    for (std::string& i : traits) {
        // position of the closest element to i in node->children
        int pos_to_insert = std::lower_bound(node->children.begin(), node->children.end(), i, DuplicateChecker::compare) - node->children.begin();

        // see if the given trait (i) is in node->children
        if (std::binary_search(node->children.begin(), node->children.end(), i, DuplicateChecker::compare)) {
            node = node->children.at(pos_to_insert);
            continue;
        }

        // If the given trait isn't in node->children, add it
        auto new_node = std::make_shared<DuplicateChecker::TrieNode>();
        new_node->val = i;
        node->children.insert(node->children.begin() + pos_to_insert, new_node);

        node = node->children.at(pos_to_insert);
    }

    node->is_end = true;

    //dc_mutex.unlock();
}

bool DuplicateChecker::compare (std::shared_ptr<TrieNode> t1, std::string t2) {
    return t1->val.compare(t2) < 0;
}

bool DuplicateChecker::search (std::vector<std::string>& traits) {
    dc_mutex.lock();

    std::shared_ptr<DuplicateChecker::TrieNode> node (root);

    for (std::string& i : traits) {
        // see if the given trait (i) isn't in node->children
        if (!std::binary_search(node->children.begin(), node->children.end(), i, DuplicateChecker::compare)) {
            insert(traits);
            return false;
        }

        // if it is, set node to that trait's node
        int pos_of_match = std::lower_bound(node->children.begin(), node->children.end(), i, DuplicateChecker::compare) - node->children.begin();
        node = node->children.at(pos_of_match);
    }

    return node->is_end;

    dc_mutex.unlock();
}