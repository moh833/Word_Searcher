#include "process_directory.h"


//void process_file(string path, Trie &root, int file_id){
//  string punc = ".,;:'\"?()!";
//  ifstream file(path);
//  for(string line; getline(file, line);){
//    for(unsigned int i=0; i<punc.size(); i++)
//        line.erase(remove(line.begin(), line.end(), punc[i]), line.end());
//    transform(line.begin(), line.end(), line.begin(), ::tolower);
//    string word;
//    istringstream iss(line);
//    while(iss >> word){
//        char cstr[word.size() + 1];
//        word.copy(cstr, word.size() + 1);
//        cstr[word.size()] = '\0';
//        root.insert(cstr, file_id);
//    }
//  }
//  file.close();
//}


//void process_from_directory(string path, map <int, string> &mapped_files, Trie &root, unsigned int& PROGRESS_VAR_GLOBAL, unsigned int& NUMBER_OF_FILES){
//    string str = path + "/*.*";

//    QDir dir(QString::fromStdString(path));
//    dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
//    NUMBER_OF_FILES = dir.count();

//    wstring widestr = std::wstring(str.begin(), str.end());
//    const wchar_t* search_path = widestr.c_str();

//    WIN32_FIND_DATA fd;
//    HANDLE hFind = ::FindFirstFile(search_path, &fd);
//    int file_id = 1;
//    PROGRESS_VAR_GLOBAL = 0;
//    do {
//    if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
//      wstring ws(fd.cFileName);
//      string file_name(ws.begin(), ws.end());
//      mapped_files[file_id] = file_name;
//      process_file(path + '\\' + file_name, root, file_id++);
//      PROGRESS_VAR_GLOBAL++;
//      qDebug() << "or " << PROGRESS_VAR_GLOBAL << endl;
//    }
//    }while(::FindNextFile(hFind, &fd));
//    ::FindClose(hFind);
//}

void process_file(string path, Trie &root, int file_id){
  string punc = ".,;:'\"?()<>!";
  ifstream file(path);
  string word;
  while(file >> word){
    for(unsigned int i=0; i<punc.size(); i++)
        word.erase(remove(word.begin(), word.end(), punc[i]), word.end());
    transform(word.begin(), word.end(), word.begin(), ::tolower);

    char cstr[word.size() + 1];
    word.copy(cstr, word.size() + 1);
    cstr[word.size()] = '\0';

    root.insert(cstr, file_id);

    }
  file.close();
}

/*
// takes directory path, empty map, root
// creates trie root and mapped_files which maps from file_id to file_name
void process_from_directory(string path, map <int, string> &mapped_files, Trie &root){

//    QDir dir(QString::fromStdString(path));
//    dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
//    dir.setFilter( QDir::Files | QDir::NoDotAndDotDot );
//    NUMBER_OF_FILES = dir.count();

//    QDirIterator it(QString::fromStdString(path), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

      QDirIterator it(QString::fromStdString(path), QDir::Files | QDir::NoDotAndDotDot);
      int file_id = 1;
        while(it.hasNext()){
            QFile file(it.next());
            mapped_files[file_id] = file.fileName().section("/",-1,-1).toStdString();
            process_file(file.fileName().toStdString(), root, file_id++);
        }

}*/


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


bool update_laoded_file(string &directory, map<int, string> &mapped_files, Trie &root, string file_path){

  ifstream file(file_path);
  file >> directory;
  int len; int file_id; string file_name;
  file >> len;
  for(int i=0; i<len; i++){
    file >> file_id;
    file >> file_name;
    mapped_files[file_id] = file_name;
  }


  QDirIterator it(QString::fromStdString(directory), QDir::Files | QDir::NoDotAndDotDot);
  file_id = 1;
  vector <string> new_files;

  while(it.hasNext()){
        QFile file(it.next());
        string file_name_only = file.fileName().section("/",-1,-1).toStdString();

        if(mapped_files[file_id] == file_name_only)
            file_id++;
        else
            new_files.push_back(file_name_only);

  }

  while(file_id-1 < len){
      auto position = find(new_files.begin(), new_files.end(), mapped_files[file_id]);
      if ( position != new_files.end() ){
          new_files.erase(position);
          file_id++;
      }
      else{
          break;
      }
  }



  if(file_id-1 != len){
    qDebug() << "This saved file is no longer valid, you have deleted files" << endl;
    mapped_files.clear();
    return false;
  }


  directory = load_from_file(mapped_files, root, file_path);
  qDebug() << QString::number(new_files.size()) << " new files will be added." << endl;
  for(auto it=new_files.begin(); it!=new_files.end(); it++){
    qDebug() << "Adding new file " << QString::fromStdString(*it) << endl;
    mapped_files[file_id] = *it;
    process_file(directory + '\\' + *it, root, file_id++);
  }
  return true;

}
