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
    NUM_DIRS = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_build_btn_clicked()
{
    QString dir_s;

    if (NUM_DIRS > 100){
        ui->out_tab1->setText("You can't add more than 100 directories");
    }
    else if (ui->dir_info->selectedItems().isEmpty()){
        ui->out_tab1->setText("No directories added, invalid operation");
    }
    else{
        int dir_i = 1;
        while (!ui->dir_info->selectedItems().isEmpty()){
            dir_s =(ui->dir_info->selectedItems()[0]->text(0));

            ui->out_tab1->setText("Processing " + dir_s);
            ui->out_tab1->append("Directory " + QString::number(dir_i) + "/" + QString::number(NUM_DIRS));

            arr_of_dirs[MAX_INDEX] = dir_s.toStdString();
            process_from_directory(dir_s.toStdString(),arr_of_maps[MAX_INDEX],arr_of_tries[MAX_INDEX]);
            ui->dir_info_readonly->append(dir_s) ;
            delete ui->dir_info->selectedItems()[0] ;
            MAX_INDEX++ ;

            dir_i++;
        }
        ui->out_tab1->setText("Built succefully") ;
        HAS_BEEN_BUILT = 1;
    }
}
void MainWindow::on_remove_btn_clicked()
{
     if (ui->dir_info->selectedItems().isEmpty()){
         ui->out_tab1->setText("Empty list, invalid operation");
     }else{
         delete ui->dir_info->selectedItems()[0] ;
         NUM_DIRS--;
     }
}
void MainWindow::on_add_dir_btn_clicked()
{
    QDir dir ;
    dir.setPath(ui->dir_include->toPlainText());
    for (int i = 0 ; i < MAX_INDEX ; i++){
        if (arr_of_dirs[i] == ui->dir_include->toPlainText().toStdString()){
            ui->out_tab1->setText("Directory already exists, please add another then proceed.");
            ui->dir_include->clear() ;
            return;
        }
    }
     if (!ui->dir_info->findItems(ui->dir_include->toPlainText(),0,0).isEmpty()){
         ui->out_tab1->setText("Directory already exists, please add another then proceed");
         ui->dir_include->clear() ;
        return;
     }
    if (dir.exists() ){

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->dir_info);
        // item->setText(0,ui->dir_include->toPlainText());
        item->setText(0,dir.path());
        ui->dir_info->setCurrentItem(item) ;
        NUM_DIRS++;

        if(ui->add_subs->isChecked()){
            // WARNING: can be added multiple times
          dir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );
          QDirIterator it(dir, QDirIterator::Subdirectories);
          while(it.hasNext()){
              QDir sub_dir(it.next());
              QTreeWidgetItem *item = new QTreeWidgetItem(ui->dir_info);
              item->setText(0,sub_dir.path());
              ui->dir_info->setCurrentItem(item) ;
              NUM_DIRS++;
          }
        }

        ui->dir_include->clear() ;
        ui->out_tab1->setText("Directory added successfuly");
    }
    else{
        ui->dir_include->clear() ;
        ui->out_tab1->setText("Directory does not exist, please enter a new directory to proceed.");
    }
}
void MainWindow::on_reset_btn_clicked()
{
    ui->out_tab1->clear();
    ui->out_tab2->clear();
    ui->dir_include->clear();
    ui->dir_info_readonly->clear();
    ui->files_info->clear();
    ui->file_content->clear();

    arr_of_tries = new Trie[100] ;
    arr_of_maps = new map<int,string>[100];
    arr_of_dirs = new string[100] ;
    MAX_INDEX = 0;
    NUM_DIRS = 0;

    ui->out_tab1->setText("All Cleared successfully.");
    while (!ui->dir_info->selectedItems().isEmpty()){
        delete ui->dir_info->selectedItems()[0] ;
    }
    if (HAS_BEEN_BUILT){
        QMessageBox msg(this);
        //add message to warn him of the deleteion of the trie
        HAS_BEEN_BUILT = 0;
    }
}
void MainWindow::on_next_btn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}
void MainWindow::on_search_btn_clicked()
{
    ui->file_content->clear();
    ui->files_info->clear();
    /*while (!ui->files_info->selectedItems().isEmpty()){
        delete ui->files_info->selectedItems()[0];
    }*/

    string word = ui->word_field->toPlainText().toStdString();
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    WORD_USED = word ;

     char cstr[word.size() + 1];
     word.copy(cstr, word.size() + 1);
     cstr[word.size()] = '\0';
     int NUM_FILES = 0 ;

    if(ui->prefix_check->isChecked()){
        for (int i = 0 ; i < MAX_INDEX ;i++){
           Trie* temp = arr_of_tries[i].prefix_exists(cstr);
           if(temp){
               set <int> files_id ;
               temp->traverse_prefix_search(files_id, cstr);
               NUM_FILES +=files_id.size() ;
               for (auto it = files_id.begin() ; it!=files_id.end() ; it++){
                   QTreeWidgetItem *item = new QTreeWidgetItem(ui->files_info) ;
                   item->setText(0,QString::fromStdString(arr_of_dirs[i])+"/" +QString::fromStdString(arr_of_maps[i][*it]));
                   ui->files_info->setCurrentItem(item);
               }
           }
        }
     ui->out_tab2->setText("Number of files found using prefix search: " +QString::number(NUM_FILES));
    }
    else{
       vector <int> files_id ;
       for (int i = 0 ; i < MAX_INDEX ;i++){
          files_id = arr_of_tries[i].word_exist(cstr);
          NUM_FILES +=files_id.size() ;
          for (auto it = files_id.begin() ; it!=files_id.end() ; it++){
              QTreeWidgetItem *item = new QTreeWidgetItem(ui->files_info) ;
              item->setText(0,QString::fromStdString(arr_of_dirs[i])+"/" +QString::fromStdString(arr_of_maps[i][*it]));
              ui->files_info->setCurrentItem(item);
          }
        }
     ui->out_tab2->setText("Number of files found using word search: " +QString::number(NUM_FILES));
    }
//    ui->word_field->clear();

}
void MainWindow::on_open_file_btn_clicked()
{
    if(ui->files_info->selectedItems().count())
        ui->file_content->setText(linear_search(ui->files_info->selectedItems()[0]->text(0).toStdString(), WORD_USED));
    else
        ui->out_tab2->setText("No files to view.");
}

void MainWindow::on_save_btn_clicked()
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
    if(MAX_INDEX)
        ui->out_tab1->setText("Saved successfully");
    else
        ui->out_tab1->setText("Nothing to save.");
}
void MainWindow::on_load_btn_clicked()
{
    QFile file(ui->load_field->toPlainText());
    if (file.exists()){
        file.open(QIODevice::ReadOnly);
        QString dir = file.readLine();
        dir = dir.split("\r")[0];
        file.close();
        for (int i = 0 ; i < MAX_INDEX ; i++){
            if (dir.toStdString() == arr_of_dirs[i]){
                ui->load_field->clear() ;
                ui->out_tab1->setText("Directory already added");
                return;
            }
        }

        if(!ui->update_check->isChecked()){
            arr_of_dirs[MAX_INDEX] = load_from_file(arr_of_maps[MAX_INDEX],arr_of_tries[MAX_INDEX],ui->load_field->toPlainText().toStdString());
            ui->dir_info_readonly->append(QString::fromStdString(arr_of_dirs[MAX_INDEX])) ;
            MAX_INDEX++ ;

            ui->load_field->clear() ;
            ui->out_tab1->setText("Loaded successfully.");

        }
        else{
            string directory;
            if(update_laoded_file(arr_of_dirs[MAX_INDEX], arr_of_maps[MAX_INDEX], arr_of_tries[MAX_INDEX], ui->load_field->toPlainText().toStdString())){
//                save_to_file(arr_of_dirs[MAX_INDEX], arr_of_maps[MAX_INDEX], arr_of_tries[MAX_INDEX], string(current_path) + "\\saved.txt");
//                save_to_file(arr_of_dirs[MAX_INDEX],arr_of_maps[MAX_INDEX],arr_of_tries[MAX_INDEX],(path + "/" + sh.dirName() +".txt").toStdString());
                ui->dir_info_readonly->append(QString::fromStdString(arr_of_dirs[MAX_INDEX])) ;
                MAX_INDEX++ ;

                ui->load_field->clear() ;
                ui->out_tab1->setText("Updated successfully, Make sure to save this updated version!");
            }
            else{
                ui->load_field->clear() ;
                ui->out_tab1->setText("Corrupted saved file.\n");
                ui->out_tab1->append("You have deleted files, Please build it again and make sure to save it.");
                // remove("D:\\work\\Data_Structures_Project\\DS\\DS_vector\\saved.txt");
            }
        }

    }
    else{
        ui->load_field->clear() ;
        ui->out_tab1->setText("file doesn't exist");
    }
}
void MainWindow::on_browse_dir_btn_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(this,tr("Select directory"));
    ui->dir_include->setText(dirname);
}
void MainWindow::on_brwose_file_btn_clicked()
{
    QDir dir(QDir::currentPath()+"/saved");
    if (!dir.exists()){
        ui->out_tab1->setText("No saved files to load");
        return;
    }
    QString filename=QFileDialog::getOpenFileName(this,tr("Select file"),QDir::currentPath()+"/saved","Text file (*.txt)");
    ui->load_field->setText(filename);
}

QString MainWindow::linear_search(string path,string word_search){
    string punc = ".,;:'\"?()<>!";
    QString return_str;
    ifstream file(path);
    string word;
    string or_word;

    while(file >> word){
      or_word = word;
      for(unsigned int i=0; i<punc.size(); i++)
          word.erase(remove(word.begin(), word.end(), punc[i]), word.end());
      transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (word.rfind(word_search, 0) == 0)
            return_str += "<span style='color:red;'>" + QString::fromStdString(or_word) + "</span>" ;
        else
            return_str += QString::fromStdString(or_word) ;

        return_str += " " ;

    }

    file.close();
    return return_str;
}

void MainWindow::process_from_directory(string path, map <int, string> &mapped_files, Trie &root){

      ui->progress->setValue(0);

      QDir dir(QString::fromStdString(path));
      dir.setFilter( QDir::Files | QDir::NoDotAndDotDot );
      unsigned int number_of_files = dir.count();

      QDirIterator it(QString::fromStdString(path), QDir::Files | QDir::NoDotAndDotDot);
      int file_id = 1;
        while(it.hasNext()){
            QFile file(it.next());
            mapped_files[file_id] = file.fileName().section("/",-1,-1).toStdString();
            process_file(file.fileName().toStdString(), root, file_id++);

            if (file_id % 100 == 0)
                ui->progress->setValue(100*((float)file_id/number_of_files));

        }

        ui->progress->setValue(100);
}

