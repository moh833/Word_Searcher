#include "process_directory.h"
#include <QDir>

void process_file(string path, Trie &root, int file_id){
  string punc = ".,;:'\"?()!";
  ifstream file(path);
  for(string line; getline(file, line);){
    for(unsigned int i=0; i<punc.size(); i++)
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
// takes directory path, empty map, root
// creates trie root and mapped_files which maps from file_id to file_name
/*void process_from_directory(string path, map <int, string> &mapped_files, Trie &root){
    string str = path + "/*.*";
    QDir dir(QString::fromStdString(path));
    dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
    wstring widestr = std::wstring(str.begin(), str.end());
    const wchar_t* search_path = widestr.c_str();

    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path, &fd);
    int file_id = 1;
    do {
    if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
      wstring ws(fd.cFileName);
      string file_name(ws.begin(), ws.end());
      mapped_files[file_id] = file_name;
      process_file(path + '\\' + file_name, root, file_id++);
    }
    }while(::FindNextFile(hFind, &fd));
    ::FindClose(hFind);

}
*/
// takes the path of the directory to save, mapped files, the root of the trie and a file_path to be created
// creates file_path file with all the data
void save_to_file(string directory, map<int, string> mapped_files, Trie root, string file_path){
  ofstream file(file_path);
  file << directory << endl;
  file << mapped_files.size() << endl;
  for(auto it = mapped_files.begin();it!=mapped_files.end();it++)
    file << it->first << " " << it->second << endl;
  root.traverse_to_file(file, "");
  file.close();
}
// takes empty map, root and the path of the saved file
// creates trie root and mapped_files
// returns the directory path of that saved data
string load_from_file(map<int, string> &mapped_files, Trie &root, string file_path){
  ifstream file(file_path);
  string directory;
  file >> directory;
  int len; int file_id; string file_name;
  file >> len;
  while(len--){
    file >> file_id;
    file >> file_name;
    mapped_files[file_id] = file_name;
  }
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

  return directory;
}
