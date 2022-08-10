#pragma once
#include <string>
#include <vector>
#include <memory>

// Checks if a generated NFT has already been made
// Implements a trie for efficient checking
class DuplicateChecker {
    private:
        // Trie implementation
        struct TrieNode;

        // Insert provided image layer combination into trie
        void insert (std::vector<std::string>& traits);

        // Returns true if t1 precedes t2 in an ordered vector
        bool compare (std::shared_ptr<TrieNode> t1, std::string t2);

        std::shared_ptr<TrieNode> root;
    public:
        DuplicateChecker ();

        /* Search for duplicate
           Returns true if provided image layer combination has been created already
           Returns false if not
        */
        bool search (std::vector<std::string>& traits);
};