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
std::string SDK_PATH = "";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
setWindowIcon(QIcon(":/icons/logo.ico"));
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
    connect(ui->BT_SDK, SIGNAL(clicked(bool)), SLOT(chooseSDK()));

}
void MainWindow::choosePath(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Project Directory"),
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

void MainWindow::chooseSDK(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open SDK Directory"),
                                                    (OS==1?"C://":"/home"),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    std::cout << dir.toStdString() << std::endl;
    SDK_PATH = dir.toStdString();
    ui->lblSDK->setText(dir);
}


bool MainWindow::systemMethod(){
    std::string platform =  ui->CB_PLATFORMS->currentText().toStdString().c_str();
    std::string SDK_PATH = "export ANDROID_HOME=/home/schirrel/Downloads/android-sdk-linux";
    std::string wholecmd = SDK_PATH+" && ionic run "+platform+" --device";
    char* cmd = const_cast<char*> ( wholecmd.c_str());
    char buffer[128];



    //  system( "cd \Users & dir" )

    if(chdir(const_cast<char*> (PROJECT_PATH.c_str())) < 0 )
    {
        printf("Failed\n");
        return false;
    } else {
       // system(cmd);

        FILE *fp;
        char path[1035];

        /* Open the command for reading. */
        fp = popen(cmd, "r");
        if (fp == NULL) {
          printf("Failed to run command\n" );
          exit(1);
        }
       /* Read the output a line at a time - output it. */
        while (fgets(path, sizeof(path)-1, fp) != NULL) {
        //  printf("%s", path);
          ui->lblOutput->setText(ui->lblOutput->text()+" "+ path);

        }

        /* close */
        pclose(fp);




        return true;

    }
}

void MainWindow::runOnDevice(){
    ui->BT_RUN->setEnabled(false);
   // ui->lblOutput->setText("Running...");
    if(systemMethod()){
//        ui->lblOutput->setText("Finished...");
    }
    ui->BT_RUN->setEnabled(true);


}

MainWindow::~MainWindow()
{
    delete ui;
}
