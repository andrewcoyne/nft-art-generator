#pragma once
#include <string>

// Checks if a generated NFT has already been made
// Implements a trie for efficient checking
class DuplicateChecker {
    private:
        struct TrieNode;
        struct TrieNode* makeNodes (std::string image_layers []);
        void insert (std::string image_layers []);

        struct TreeNode* root;
    public:
        DuplicateChecker ();

        // Search for duplicate
        // Returns true if provided image layer combination has been created already
        // Returns false if not
        bool search (std::string image_layers []);
};