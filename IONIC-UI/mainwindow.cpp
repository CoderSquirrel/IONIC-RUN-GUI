#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include <dirent.h>
#include <stdio.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <unistd.h>
int OS=0;
std::string slash = "/";
std::string PROJECT_PATH = "";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    slash="\\";
    OS = 0;
#else
    slash="/";
    OS =1;
#endif
    ui->setupUi(this);

    connect(ui->BT_PATH, SIGNAL(clicked(bool)), SLOT(choosePath()));
    connect(ui->BT_RUN, SIGNAL(clicked(bool)), SLOT(runOnDevice()));

}
void MainWindow::choosePath(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    (OS==1?"C://":"/home"),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    std::cout << dir.toStdString() << std::endl;
    PROJECT_PATH = dir.toStdString();
    ui->lblPath->setText(dir);
    std::string path = dir.toStdString()+slash+"platforms";

    DIR *parentdir = opendir(path.c_str());

    struct dirent *entry = readdir(parentdir);
    bool dot=false;
    while (entry != NULL)
    {dot=false;
        if (entry->d_type == DT_DIR) {
            //    std::cout << entry->d_name << std::endl;
            for(int c =0; entry->d_name[c] ; c++) {
                if(entry->d_name[c]=='.') {
                    dot = true;
                    break;
                }
            }
            if(!dot) {
                std::cout << entry->d_name << std::endl;
                ui->CB_PLATFORMS->addItem(QString::fromUtf8(entry->d_name));
            }
        }
        entry = readdir(parentdir);
    }

    closedir(parentdir);

}


void MainWindow::runOnDevice(){
    std::cout << "running" <<std::endl;

  std::string platform =  ui->CB_PLATFORMS->currentText().toStdString().c_str();
  std::string exportPath = "export ANDROID_HOME=/home/schirrel/Downloads/android-sdk-linux";
   std::string wholecmd = exportPath+" & ionic run "+platform+" --device";
   char* cmd = const_cast<char*> ( wholecmd.c_str());
   char buffer[128];



 //  system( "cd \Users & dir" )
 //   system(cmd);
    if(chdir(const_cast<char*> (PROJECT_PATH.c_str())) < 0 )
      {
         printf("Failed\n");

      } else {
 printf("mudo\n");
//  system(cmd);
//   system("");
   std::string result = "";
       std::shared_ptr<FILE> pipe(popen("echo $ANDROID_HOME", "r"), pclose);
       if (!pipe) throw std::runtime_error("popen() failed!");
       while (!feof(pipe.get())) {
           if (fgets(buffer, 128, pipe.get()) != NULL)
               result += buffer;
       }
  std::cout << result << std::endl;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
