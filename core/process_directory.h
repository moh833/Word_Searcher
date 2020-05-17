#ifndef PROCESS_DIR_H  
#define PROCESS_DIR_H

#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <Windows.h>

#include "trie.h"

using namespace std;


void process_file(string path, Trie &root, int file_id);

void process_from_directory(string path, map <int, string> &mapped_files, Trie &root);

void save_to_file(string directory, map<int, string> mapped_files, Trie root, string file_path);

string load_from_file(map<int, string> &mapped_files, Trie &root, string file_path);


#endif
