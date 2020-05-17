#include "trie.h"

  Trie::Trie(){
    files = vector <int>();
  }

  void Trie::insert(char *str, int file_id){
    if(*str == '\0'){
      if(!files.empty() && files[files.size() - 1] == file_id)
        return;
      files.push_back(file_id);
    }
    else {
      if(children.count(*str) == 0)
        children[*str] = new Trie();
      children[*str]->insert(str+1, file_id);
    }
  }

  void Trie::insert(char *str, vector <int> files_to_insert){
    if(*str == '\0'){
       files.insert(files.begin(), files_to_insert.begin(), files_to_insert.end());
    }
    else {
      if(children.count(*str) == 0)
        children[*str] = new Trie();
      children[*str]->insert(str+1, files_to_insert);
    }
  }

  vector <int> Trie::word_exist(char* str){
    if(*str == '\0')
      return files;

    if(children.count(*str) == 0)
      return vector <int>();
    return children[*str]->word_exist(str+1);
  }

  bool Trie::prefix_exist(char* str){
    if(*str == '\0')
      return true;

    if(children.count(*str) == 0)
      return false;
    return children[*str]->prefix_exist(str+1);
  }

  void Trie::traverse(string word){
    if(!files.empty()){
      cout << word;
      for(auto it=files.begin(); it!=files.end(); it++)
        cout << " " << *it;
      cout << endl;
    }
    for(auto it=children.begin(); it!=children.end(); it++)
      it->second->traverse(word + it->first);
  }

  void Trie::traverse_to_file(ofstream &save_file, string word){
    if(!files.empty()){
      save_file << word;
      for(auto it=files.begin(); it!=files.end(); it++)
        save_file << " " << *it;
      save_file << endl;
    }
    for(auto it=children.begin(); it!=children.end(); it++)
      it->second->traverse_to_file(save_file, word + it->first);
  }