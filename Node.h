/**
 * File:    Node.h
 *
 * Author1:  Sarthak Patipati (spati20@uic.edu)
 * Author2:  Jiwon Jung (jjung65@uic.edu)
 * Date:     Spring 2023
 * Course:   Computer Science 351
 *
 * Summary of File:
 *
 * This file contains the implementation of nodes in the Huffman Tree. The Node
 * struct contains the character, frequency, and pointers to child nodes.
 *
 */

#pragma once

#include <fstream>
#include <iostream>

using namespace std;

struct Node {
  char character;
  int frequency;
  Node *left;
  Node *right;

  Node() {
    this->character = '\0';
    this->frequency = 0;
    this->left = NULL;
    this->right = NULL;
  }

  Node(char character, int frequency) {
    this->character = character;
    this->frequency = frequency;
    this->left = NULL;
    this->right = NULL;
  }

  bool isLeaf() { return !left && !right; }
};

Node *newNode(char character, int frequency) {
  return new Node(character, frequency);
}
