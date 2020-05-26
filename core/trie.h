#ifndef TRIE_H  
#define TRIE_H

#include <iostream>
#include <fstream>
#include <memory.h>
#include <map>
#include <set>
#include <vector>
using namespace std;

class Trie{

private:
  map <char, Trie*> children;
  vector <int> files;

public:

  Trie();

  void insert(char *str, int file_id);

  void insert(char *str, vector <int> files_to_insert);

  vector <int> word_exist(char* str);

  void traverse(string word);

  void traverse_to_file(ofstream &save_file, string word);

  Trie* prefix_exists(char* str);

  set <int> traverse_prefix_search(set <int>& found_files, string word="");

};

#endif
