#include "process_directory.h"
#include "trie.h"
#include <ctime>

#include <cerrno>
#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif
 
int main(){

    Trie root;
    // process_file("test.txt", root);

    char current_path[FILENAME_MAX];
    if (!GetCurrentDir(current_path, sizeof(current_path)))
        return errno;
    // current_path[sizeof(current_path) - 1] = '\0'; /* not really required */

    clock_t start = clock();
    process_from_directory(string(current_path) + "\\questions", root);
    // map <int, string> mapped_files = process_from_directory(string(current_path) + "\\questions", root);
    // map <int, string> mapped_files = process_from_directory(string(current_path) + "\\data", root);
    clock_t end = clock();
    cout<<"Time consumed for making the trie = "<<((double)end-start)/CLOCKS_PER_SEC<<" seconds"<<endl;


    // for(auto it=mapped_files.begin(); it!=mapped_files.end(); it++)
    //     cout << it->first << " - " << it->second << endl;

    start = clock();
    save_to_file(string(current_path) + "\\saved.txt", root);
    end = clock();
    cout<<"Time consumed for saving data = "<<((double)end-start)/CLOCKS_PER_SEC<<" seconds"<<endl;

    start = clock();
    load_from_file(string(current_path) + "\\saved.txt", root);
    end = clock();
    cout<<"Time consumed for loading data = "<<((double)end-start)/CLOCKS_PER_SEC<<" seconds"<<endl;

    char word[50];
    vector <int> files;
    while(cin >> word && strcmp(word, "q")){

        cout << "Searching for: " << word << endl;

        files = root.word_exist(word);
        if(files.empty())
            cout << "word doesn't exist" << endl;
        else {
            for(auto it=files.begin(); it!=files.end(); it++)
            cout << "file " << *it << endl;
        }
    }


    return 0;
}