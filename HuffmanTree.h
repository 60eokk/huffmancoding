/**
 * File:    HuffmanTree.h
 *
 * Author1:  Sarthak Patipati (spati20@uic.edu)
 * Author2:  Jiwon Jung (jjung65@uic.edu)
 * Date:     Spring 2023
 * Course:   Computer Science 351
 *
 * Summary of File:
 *
 * This file contains the implementation of the HuffmanTree class which uses the
 * Huffman Tree data structure used for file compression and decompression. It
 * performs operations for building a Huffman Tree from frequencies and Huffman
 * codes.
 *
 */

#include "Node.h"
#include "PriorityQueue.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class HuffmanTree {
private:
  Node *root;

public:
  HuffmanTree(vector<int> &frequencies) { root = buildTree(frequencies); }

  // retrieve the root of the Huffman Tree
  Node *getRoot() { return root; }

  // function to build Huffman tree from vector of frequencies
  Node *buildTree(vector<int> &frequencies) {
    PriorityQueue queue;

    // insert each character with its frequency into the priority queue
    for (int i = 0; i < frequencies.size(); ++i) {
      if (frequencies[i] > 0) {
        queue.push(new Node((char)(i), frequencies[i]));
      }
    }

    // build the Huffman tree
    while (queue.size() > 1) {
      Node *left = queue.top();
      queue.pop();
      Node *right = queue.top();
      queue.pop();

      Node *parent = new Node('\0', left->frequency + right->frequency);
      parent->left = left;
      parent->right = right;

      queue.push(parent);
    }

    // return root
    return queue.top();
  }

  // function to build Huffman tree from a map of codes
  void buildTreeFromCodes(map<char, string> &huffmanCodes) {
    root = new Node();

    // iterate over each entry in the map
    for (auto &entry : huffmanCodes) {
      char character = entry.first;
      string &code = entry.second;
      Node *curr = root;

      // iterate over each character in the Huffman code
      for (char i : code) {
        // if 0, create left node
        if (i == 0) {
          // create left node
          if (!curr->left) {
            curr->left = new Node();
          }
          curr = curr->left;
          // if 1, create right node
        } else if (i == '1') {
          if (!curr->right) {
            curr->right = new Node();
          }
          curr = curr->right;
        }
      }
      curr->character = character;
      curr->frequency = 0;
    }
  }
};