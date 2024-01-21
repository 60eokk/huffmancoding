/**
 * File:    PriorityQueue.h
 *
 * Author1:  Sarthak Patipati (spati20@uic.edu)
 * Author2:  Jiwon Jung (jjung65@uic.edu)
 * Date:     Spring 2023
 * Course:   Computer Science 351
 *
 * Summary of File:
 *
 * This file contains the implementation of a priority queue data structure used
 * to build the Huffman Tree. It uses a vector to represent the inner heap.
 *
 */

#pragma once

#include "Node.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class PriorityQueue {
private:
  vector<Node *> heap;

public:
  // retrieve the index of the parent
  int parent(int i) { return (i - 1) / 2; }

  // retrieve the index of the left child
  int leftChild(int i) { return (2 * i) + 1; }

  // retrieve the index of the right child
  int rightChild(int i) { return (2 * i) + 2; }

  // fix the heap when a new element is pushed
  void shiftUp(int i) {
    while (i > 0 && heap[parent(i)]->frequency > heap[i]->frequency) {
      swap(heap[parent(i)], heap[i]);
      i = parent(i);
    }
  }

  // fix the heap when top element is popped
  void shiftDown(int i) {
    int min = i;
    int left = leftChild(i);
    int right = rightChild(i);

    if (left < size() && heap[left]->frequency < heap[min]->frequency) {
      min = left;
    }

    if (right < size() && heap[right]->frequency < heap[min]->frequency) {
      min = right;
    }

    if (min != i) {
      swap(heap[i], heap[min]);
      shiftDown(min);
    }
  }

  // push new element in the priority queue
  void push(Node *node) {
    heap.push_back(node);
    shiftUp(size() - 1);
  }

  // pop the top node (highest priority)
  void pop() {
    if (size() == 0) {
      cout << "Priority Queue is empty" << endl;
      return;
    }

    heap[0] = heap.back();
    heap.pop_back();
    shiftDown(0);
  }

  // retrieve the top node (highest priority)
  Node *top() {
    if (size() == 0) {
      cout << "Priority Queue is empty" << endl;
      return NULL;
    }

    return heap[0];
  }

  // return the size of the priority queue
  int size() { return heap.size(); }

  // check if priority queue is empty
  bool empty() { return heap.empty(); }
};