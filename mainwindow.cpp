#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    arr_of_tries = new Trie[100] ;
    arr_of_maps = new map<int,string>[100];
    arr_of_dirs = new string[100] ;
    HAS_BEEN_BUILT = 0 ;
    MAX_INDEX = 0;
    PROGRESS_VAR_GLOBAL = 0 ;
    NUMBER_OF_FILES = 0 ;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_build_btn_clicked()
{
    QString dir_s;
    QDir dir_;
    int NUM = ui->dir_info->currentIndex().row()+1 ;
    if (NUM > 100){
        ui->out_tab1->append("You cant add more than 100 directories");
    }
    else if (ui->dir_info->selectedItems().isEmpty()){
        ui->out_tab1->append("No directories added, invalid operation");
    }
    else{
        while (!ui->dir_info->selectedItems().isEmpty()){
            dir_s =(ui->dir_info->selectedItems()[0]->text(0));
            arr_of_dirs[MAX_INDEX] = dir_s.toStdString();
            process_from_directory(dir_s.toStdString(),arr_of_maps[MAX_INDEX],arr_of_tries[MAX_INDEX]);
            ui->dir_info_readonly->append(dir_s) ;
            delete ui->dir_info->selectedItems()[0] ;
            MAX_INDEX++ ;
        }
        ui->out_tab1->clear() ;
        ui->out_tab1->append("Index built succefully") ;
        HAS_BEEN_BUILT = 1;
        //timer->stop();
    }
}
void MainWindow::on_pushButton_2_clicked()
{
     if (ui->dir_info->selectedItems().isEmpty()){
         ui->out_tab1->append("Empty list, invalid operation");
     }else{
         delete ui->dir_info->selectedItems()[0] ;
     }
}
void MainWindow::on_pushButton_clicked()
{
    QDir dir ;
    dir.setPath(ui->dir_include->toPlainText());
    for (int i = 0 ; i < MAX_INDEX ; i++){
        if (arr_of_dirs[i] == ui->dir_include->toPlainText().toStdString()){
            ui->out_tab1->append("Directory already exists, please add another then proceed");
            ui->dir_include->clear() ;
            return;
        }
    }
     if (!ui->dir_info->findItems(ui->dir_include->toPlainText(),0,0).isEmpty()){
         ui->out_tab1->append("Directory already exists, please add another then proceed");
         ui->dir_include->clear() ;
        return;
     }
    if (dir.exists() ){
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->dir_info);
        item->setText(0,ui->dir_include->toPlainText());
        ui->dir_info->setCurrentItem(item) ;
        ui->dir_include->clear() ;
        ui->out_tab1->append("Directory added successfuly");
    }
    else{
        ui->out_tab1->append("Directory does not exist, please enter a new directory to proceed");
    }
}
void MainWindow::on_reset_btn_clicked()
{
    ui->out_tab1->clear();
    ui->dir_include->clear();
    while (!ui->dir_info->selectedItems().isEmpty()){
        delete ui->dir_info->selectedItems()[0] ;
    }
    if (HAS_BEEN_BUILT){
        QMessageBox msg(this);
        //add message to warn him of the deleteion od the trie
    }
}
void MainWindow::on_next_btn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}
void MainWindow::on_pushButton_4_clicked()
{
    ui->file_content->clear();
    ui->files_info->clear();
    /*while (!ui->files_info->selectedItems().isEmpty()){
        delete ui->files_info->selectedItems()[0];
    }*/
    string word = ui->word_field->toPlainText().toStdString();
    WORD_USED = ui->word_field->toPlainText() ;
    char cstr[word.size() + 1];
    vector <int> files_id ;
     word.copy(cstr, word.size() + 1);
     cstr[word.size()] = '\0';
     int NUM_FILES = 0 ;
    for (int i = 0 ; i < MAX_INDEX ;i++){
       files_id = arr_of_tries[i].word_exist(cstr);
       NUM_FILES +=files_id.size() ;
       for (unsigned int j = 0 ; j < files_id.size() ; j++){
           QTreeWidgetItem *item = new QTreeWidgetItem(ui->files_info) ;
           item->setText(0,QString::fromStdString(arr_of_dirs[i])+"/" +QString::fromStdString(arr_of_maps[i][files_id[j]]));
           ui->files_info->setCurrentItem(item);
       }
    }
    ui->out_tab2->setText("Number of files found : " +QString::number(NUM_FILES));
}
void MainWindow::on_pushButton_5_clicked()
{
    ui->file_content->setText(linear_search(ui->files_info->selectedItems()[0]->text(0).toStdString(),ui->word_field->toPlainText()));
}
QString MainWindow::linear_search(string path,QString word_search){
    string punc = ".,;:'\"?()!";
    QString return_str;
    ifstream file(path);
    for(string line; getline(file, line);){

      for(unsigned int i=0; i<punc.size(); i++)
          line.erase(remove(line.begin(), line.end(), punc[i]), line.end());
      transform(line.begin(), line.end(), line.begin(), ::tolower);
      string word;
      istringstream iss(line);
      while(iss >> word){
            if (word == word_search.toStdString()){
                return_str += "<span style='color:red;'>" + word_search + "</span>" ;
            }
            else{
                return_str += QString::fromStdString(word) ;
            }
            return_str += " " ;
      }
    }
    file.close();
    return return_str;
}
void MainWindow::on_pushButton_3_clicked()
{
    QDir dir(QDir::currentPath());
    if (!QDir(dir.absolutePath()+"/saved").exists()){
        dir.mkdir("saved");
    }
    QString path = dir.absolutePath() + "/saved" ;
    for (int i = 0 ; i < MAX_INDEX ; i++){
        QDir sh = (QString::fromStdString(arr_of_dirs[i]));
        save_to_file(arr_of_dirs[i],arr_of_maps[i],arr_of_tries[i],(path + "/" + sh.dirName() +".txt").toStdString());
    }
}
void MainWindow::on_pushButton_7_clicked()
{
    QFile file(ui->load_field->toPlainText());
    if (file.exists()){
        file.open(QIODevice::ReadOnly);
        QString dir = file.readLine();
        dir = dir.split("\r")[0];
        file.close();
        for (int i = 0 ; i < MAX_INDEX ; i++){
            if (dir.toStdString() == arr_of_dirs[i]){
                ui->out_tab1->append("Directory already added");
                return;
            }
        }
        arr_of_dirs[MAX_INDEX] = load_from_file(arr_of_maps[MAX_INDEX],arr_of_tries[MAX_INDEX],ui->load_field->toPlainText().toStdString());
        ui->dir_info_readonly->append(QString::fromStdString(arr_of_dirs[MAX_INDEX])) ;
        MAX_INDEX++ ;
    }
    else{
        ui->out_tab1->append("file doesn't exist");
    }
}
void MainWindow::on_browse_btn_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(this,tr("Select directory"));
    ui->dir_include->setText(dirname);
}
void MainWindow::on_pushButton_6_clicked()
{
    QDir dir(QDir::currentPath()+"/saved");
    if (!dir.exists()){
        ui->out_tab1->append("No saved files to load");
        return;
    }
    QString filename=QFileDialog::getOpenFileName(this,tr("Select file"),QDir::currentPath()+"/saved","Text file (*.txt)");
    ui->load_field->setText(filename);
}

