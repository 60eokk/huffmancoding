/**
 * File:    filecompress.cpp
 *
 * Author1:  Sarthak Patipati (spati20@uic.edu)
 * Author2:  Jiwon Jung (jjung65@uic.edu)
 * Date:     Spring 2023
 * Course:   Computer Science 351
 *
 * Summary of File:
 *
 * This file performs file compression and decompression using the Huffman
Coding technique. It has functions to perform the 4 operations:
1. Create the needed Huffman Coding information based on the frequencies used in
a given file and store this information in a file for future use.
2. Import the Huffman Code information from a file created by part 1 to use for
file compression/decompression in operations 3 and 4.
3. Use the imported Huffman Code information to compress a given file.
4. Use the imported Huffman Code information to decompress a given file.
 *
 */

#include "HuffmanTree.h"
#include "Node.h"
#include "PriorityQueue.h"
#include <bitset>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/**
 *
 * void generateCode(Node *root, string &code, map<char, string> &codes)
 *
 * Summary of the generateCode function:
 *
 *    The generateCode function generates the Huffman code for a given
 * character.
 */
void generateCode(Node *root, string &code, map<char, string> &codes) {
  if (root == NULL) {
    return;
  }

  // if current node is leaf, store character code in map
  if (root->isLeaf()) {
    codes[root->character] = code;
    // build code for character recursively
  } else {
    string lcode = code + "0";
    string rcode = code + "1";
    generateCode(root->left, lcode, codes);
    generateCode(root->right, rcode, codes);
  }
}

/**
 *
 * void createHuffmanInfo(string &input)
 *
 * Summary of the createHuffmanInfo function:
 *
 *    The createHuffmanInfo creates the needed Huffman code information based
 * on the frequencies used in a given file and store this information in a file
 * for future use.
 *
 */
void createHuffmanInfo(string &input) {
  fstream inputFile;
  inputFile.open(input, fstream::in);

  if (!inputFile.is_open()) {
    cout << "Could not open file: " << input << endl;
    return;
  }

  // calculate the frequency of each ASCII character
  vector<int> freq(128, 0);
  char c;
  int count = 0;
  while (inputFile >> noskipws >> c) {
    count++;
    int chint = c;
    if (chint > 127)
      cout << "Non ASCII Character: " << chint << endl;
    else
      freq[chint]++;
  }
  inputFile.close();

  // if file is empty
  if (count == 0) {
    cout << "Input file is empty." << endl;
    return;
  }

  // create Huffman tree from frequency vector
  HuffmanTree tree(freq);

  // generate Huffman codes for each character
  map<char, string> codes;
  string code = "";
  generateCode(tree.getRoot(), code, codes);

  // create output file (.hi)
  string filename = input + ".hi";
  ofstream outputFile(filename, ios::binary);

  if (!outputFile.is_open()) {
    cout << "Could not create file:" << filename << endl;
    return;
  }

  // write Huffman codes of each character to .hi file
  for (int i = 0; i < 128; i++) {
    outputFile << i << "    ";
    auto it = codes.find((char)(i));
    if (it != codes.end()) {
      outputFile << it->second;
    }
    outputFile << endl;
  }

  outputFile.close();
}

/**
 *
 * void importHuffmanInfo(string &input)
 *
 * Summary of the importHuffmanInfo function:
 *
 *    The importHuffmanInfo function imports the Huffman code information from a
 * file created by Operation 1 to use for file compression/decompression in
 * Operations 3 and 4.
 */
HuffmanTree *importHuffmanInfo(string &input) {
  fstream infile;
  infile.open(input, fstream::in);

  if (!infile.is_open()) {
    cout << "could not open file: " << input << endl;
    return nullptr;
  }

  // read the frequency data
  vector<int> freq(128, 0);
  for (int i = 0; i < 128; i++) {
    int data;
    string line;
    getline(infile, line);
    stringstream ss(line);
    ss >> data;
    freq[i] = data;
  }

  infile.close();

  // create new Huffman tree
  HuffmanTree *huffmanTree = new HuffmanTree(freq);

  map<char, string> huffmanCodes;
  string code = "";
  generateCode(huffmanTree->getRoot(), code, huffmanCodes);

  return huffmanTree;
}

/**
 *
 * void compress(string &input, HuffmanTree *tree)
 *
 * Summary of the compress function:
 *
 *    The compress function uses the uimported Huffman code information to
 * compress a given file.
 *
 */
void compress(string &input, HuffmanTree *tree) {
  // check if Operation 2 has been done
  if (tree == NULL) {
    cout << "Error: Import a .hi file via operation 2" << endl;
    return;
  }

  // open input file
  fstream infile;
  infile.open(input, fstream::in);
  if (!infile.is_open()) {
    cout << "Could not open file: " << input << endl;
    return;
  }

  // generate code for each character
  map<char, string> codes;
  string code = "";
  generateCode(tree->getRoot(), code, codes);

  // create output file with extension .hc
  string filename = input + ".hc";
  ofstream output(filename, ios::binary);
  if (!output.is_open()) {
    cout << "Could not create file:" << filename << endl;
    infile.close();
    return;
  }

  // find codes longer than 8 bits (for padding)
  string bits;
  for (auto &c : codes) {
    if (c.second.length() > 8) {
      bits = c.second;
      break;
    }
  }

  // encode data and write to output file
  char ch;
  unsigned char buffer = 0;
  int count = 0;
  int ibytes = 0;
  int obytes = 0;

  while (infile.get(ch)) {
    ibytes++;
    string &code = codes[ch];

    for (char bit : code) {
      buffer = (buffer << 1) | (bit == '1');
      count++;

      if (count == 8) {
        output.put(buffer);
        obytes++;
        buffer = 0;
        count = 0;
      }
    }
  }

  // if buffer not full, then add remaining bits
  while (count > 0 && count < 8) {
    buffer = (buffer << 1) | (bits[count - 1] == '1');
    count++;

    if (count == 8) {
      output.put(buffer);
      obytes++;
    }
  }

  infile.close();
  output.close();

  // calculate and display the compression info
  double compressionRatio = (double)(ibytes) / obytes;
  double spaceSaving = (1.0 - (double)(obytes) / ibytes) * 100.0;

  cout << "The number of characters/bytes in the original file: " << ibytes
       << endl;
  cout << "The number of bytes in the compressed file: " << obytes << endl;
  cout << "The compression ratio: " << fixed << setprecision(5)
       << compressionRatio << endl;
  cout << "The space saving as a percentage: " << fixed << setprecision(5)
       << spaceSaving << "%" << endl;
}

/**
 *
 * void decompress(string &input, HuffmanTree *tree)
 *
 * Summary of the decompress function:
 *
 *    The decompress function takes in the imported Huffman code information to
 * decompress a given file.
 *
 */
void decompress(string &input, HuffmanTree *tree) {
  // check if Operation 2 has been done
  if (tree == NULL) {
    cout << "Error: Import a .hi file via operation 2" << endl;
    return;
  }

  // open the compressed file
  ifstream cfile(input, ios::binary);
  if (!cfile.is_open()) {
    cout << "Could not open file: " << input << endl;
    return;
  }

  // create output file (i.e decompressed)
  string output_name = input.substr(0, input.find_last_of('.'));
  ofstream dfile(output_name);
  if (!dfile.is_open()) {
    cout << "Could not create file: " << output_name << endl;
    cfile.close();
    return;
  }

  // read-in the file bit-by-bit and traverse down the Huffman Tree one level
  // for each bit
  Node *curr = tree->getRoot();
  char c;
  while (cfile.get(c)) {
    for (int i = 7; i >= 0; i--) {
      // if the  bit is a 0, traverse to the left child
      // if the bit is a  traverse to the right child
      if ((c >> i) & 1) {
        curr = curr->right;
      } else {
        curr = curr->left;
      }

      // when a leaf node is encountered, output the ASCII character associated
      // with the leaf node and begin the next traversal from the top of the
      // tree
      if (curr->isLeaf()) {
        dfile.put(curr->character);
        curr = tree->getRoot();
      }
    }
  }

  cfile.close();
  dfile.close();
}

void displayCommands() {
  cout << "\nOperation are given by digits 1 through 5\n\n";
  cout << "  1 <filename> - create a new Huffman Information file from an "
          "original file\n";
  cout << "  2 <filename> - load a Huffman Information file \n";
  cout << "  3 <filename> - compress a file using the current Huffman "
          "Information file\n";
  cout << "  4 <filename> - decompress a file using the current Huffman "
          "Information file\n";
  cout << "  5            - quit the program\n\n";
}

int main(int argc, char **argv) {

  cout << "Welcome to File Compression program\n";

  displayCommands();

  bool done = false;
  string line;
  string input;

  HuffmanTree *tree = NULL;

  do {
    cout << "cmd> ";

    // read a complete line
    getline(cin, line);

    // now create a "stringstream" on the line just read
    stringstream ss(line);

    // clear input from any previous value
    input = "";

    // get command character
    char command;
    ss >> command;
    // cout << "Debug command:" << command << "***\n";

    if (command == '1') {
      ss >> input;
      // build a new .hi file using the information in the file
      createHuffmanInfo(input);
    }

    if (command == '2') {
      ss >> input;
      if (tree) {
        delete tree;
      }
      // load a .hi file to later perform compression and decompression
      tree = importHuffmanInfo(input);
    }

    if (command == '3') {
      ss >> input;
      // use the current .hi file to compress the file in input
      compress(input, tree);
    }

    if (command == '4') {
      ss >> input;
      // use the current .hi file to decompress the file in input
      // note if the file was not compressed using the same .hi file, the
      // output will be meaningless
      decompress(input, tree);
    }

    if (command == '5' || command == 'q') {
      done = true;
    }

  } while (!done && !cin.eof());

  if (tree) {
    delete tree;
  }

  return 0;
}
