#include <mutex>
#include "DuplicateChecker.hpp"

DuplicateChecker::DuplicateChecker () {
    root = std::make_shared<DuplicateChecker::TrieNode>();
}

struct DuplicateChecker::TrieNode {
    std::string val = "";
    bool is_end = false;
    std::vector<std::shared_ptr<DuplicateChecker::TrieNode>> children;
};

bool DuplicateChecker::binary_search (std::vector<std::shared_ptr<TrieNode> >& children, std::string key) {
    int lo = 0, hi = children.size() - 1;
    int mid;

    while (hi - lo > 1) {
        int mid = (hi + lo) / 2;
        if (children[mid]->val.compare(key) < 0) {
            lo = mid + 1;
        }
        else {
            hi = mid;
        }
    }
    return children[lo]->val.compare(key) == 0 || children[hi]->val.compare(key) == 0;
}

int DuplicateChecker::lower_bound (std::vector<std::shared_ptr<TrieNode> >& children, std::string key) {
    int a = 0, c = children.size();
 
    while (c > 0) {
        int b = a + (c / 2);
        if (children[b]->val.compare(key) < 0) {
            a = ++b;
            c -= b + 1;
        }
        else {
            c = b;
        }
    }
    return a;
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
            return false;
        }

        // if it is, set node to that trait's node
        int pos_of_match = lower_bound(node->children, i);
        node = node->children.at(pos_of_match);
    }

    return node->is_end;

    dc_mutex.unlock();
}