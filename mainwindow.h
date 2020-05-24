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
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_reset_btn_clicked();

    void on_next_btn_clicked();

    void on_build_btn_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

    void on_browse_btn_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    Trie* arr_of_tries ;
    map <int,string> *arr_of_maps;
    string *arr_of_dirs ;
    bool HAS_BEEN_BUILT;
    unsigned short MAX_INDEX ;
    QString WORD_USED ;
    QString linear_search(string path,QString word);
    unsigned int PROGRESS_VAR_GLOBAL ;
    unsigned int NUMBER_OF_FILES ;
};
#endif // MAINWINDOW_H
