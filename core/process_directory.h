#ifndef PROCESS_DIR_H  
#define PROCESS_DIR_H

#include <iostream>
#include <map>

#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <filesystem>

using namespace std;

#include "trie.h"

void process_file(string path, Trie &root, int file_id){
  string punc = ".,;:'\"";
  ifstream file(path);
  for(string line; getline(file, line);){

    for(int i=0; i<punc.size(); i++)
        line.erase(remove(line.begin(), line.end(), punc[i]), line.end());
    transform(line.begin(), line.end(), line.begin(), ::tolower); 
    string word;
    istringstream iss(line);
    while(iss >> word){
      char cstr[word.size() + 1];
	    word.copy(cstr, word.size() + 1);
	    cstr[word.size()] = '\0';
        root.insert(cstr, file_id);
        // cout << cstr << endl;
    }
  }
  file.close();
}

void process_from_directory(string path, Trie &root){
    namespace fs = filesystem;
    int file_id = 1;
    for (const auto & entry : fs::directory_iterator(path)){
      process_file(entry.path().string(), root, file_id++);
    }
}

// map <int, string> process_from_directory(string path, Trie &root){
//     namespace fs = filesystem;
//     int file_id = 1;
//     map <int, string> mapped_files;
//     for (const auto & entry : fs::directory_iterator(path)){
//       mapped_files[file_id] = entry.path().filename().string();
//       process_file(entry.path().string(), root, file_id++);
//     }
//     return mapped_files;
// }

void save_to_file(string path, Trie root){
  ofstream file(path);
  root.traverse_to_file(file, "");
  file.close();
}

void load_from_file(string path, Trie &root){

  ifstream file(path);
  for(string line; getline(file, line);){
    string word;
    istringstream iss(line);
    iss >> word;
    char cstr[word.size() + 1];
    word.copy(cstr, word.size() + 1);
    cstr[word.size()] = '\0';

    int file_id;
    vector <int> files;
    while(iss >> file_id)
      files.push_back(file_id);
    root.insert(cstr, files);
  }
  file.close();
}


#endif