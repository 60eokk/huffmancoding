// #include <bitset>
// #include <cctype>
// #include <cstring>
// #include <fstream>
// #include <iostream>
// #include <vector>

// using namespace std;

// int main() {
//   vector<int> freq;

//   for (int i = 0; i < 128; i++) {
//     freq.push_back(0);
//   }

//   char ch;
//   fstream infile;
//   cout << "Enter filename:";
//   string fname = "zipdelim.txt";
//   cin >> fname;

//   infile.open(fname, fstream::in);
//   if (!infile.is_open()) {
//     cout << "could not open file: " << fname << endl;
//     exit(0);
//   }

//   int charCount = 0;

//   while (infile >> noskipws >> ch) {
//     charCount++;
//     int chint = ch;
//     if (chint > 127)
//       cout << "Non ASCII Character: " << chint << endl;
//     else
//       freq[chint]++;
//   }
//   cout << "File " << fname << " has " << charCount << " characters." << endl;

//   for (int i = 0; i < 128; i++) {
//     cout << i << "    " << freq[i];
//     char ch = i;
//     if (isprint(i))
//       cout << "   " << ch;
//     cout << "  " << freq[i] * 1.0 / charCount << endl;
//   }

//   return 0;
// }