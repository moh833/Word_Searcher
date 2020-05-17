#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_build_btn_pressed()
{
    string abs_path = "D:/Data_Structures_Project/word_searcher/";

    Trie root;
    map <int, string> mapped_files;

    clock_t start = clock();
    process_from_directory(abs_path+"questions", mapped_files, root);
    clock_t end = clock();
    cout<<"Time consumed for making the trie = "<<((double)end-start)/CLOCKS_PER_SEC<<" seconds"<<endl;

    start = clock();
    save_to_file(abs_path+"questions", mapped_files, root, abs_path+"saved/saved.txt");
    end = clock();
    cout<<"Time consumed for saving data = "<<((double)end-start)/CLOCKS_PER_SEC<<" seconds"<<endl;

    Trie loaded_root;
    map <int, string> loaded_mapped_files;

    start = clock();
    string directory = load_from_file(loaded_mapped_files, loaded_root, abs_path+"saved/saved.txt");
    end = clock();
    cout<<"Time consumed for loading data = "<<((double)end-start)/CLOCKS_PER_SEC<<" seconds"<<endl;
}
