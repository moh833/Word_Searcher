#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <time.h>
#include "core/process_directory.h"
#include "core/trie.h"
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <fstream>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>
#include <QDirIterator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_remove_btn_clicked();

    void on_add_dir_btn_clicked();

    void on_reset_btn_clicked();

    void on_next_btn_clicked();

    void on_build_btn_clicked();

    void on_search_btn_clicked();

    void on_open_file_btn_clicked();

    void on_save_btn_clicked();

    void on_load_btn_clicked();

    void on_browse_dir_btn_clicked();

    void on_brwose_file_btn_clicked();

private:
    Ui::MainWindow *ui;
    Trie* arr_of_tries ;
    map <int,string> *arr_of_maps;
    string *arr_of_dirs ;
    bool HAS_BEEN_BUILT;
    unsigned short MAX_INDEX ;
    string WORD_USED ;
    QString linear_search(string path,string word_search);
    void process_from_directory(string path, map <int, string> &mapped_files, Trie &root);
};
#endif // MAINWINDOW_H
