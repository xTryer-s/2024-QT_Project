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


void MainWindow::on_TurnToMode1_button_clicked()
{
    subwindow1 = new SubWindow1(this);
    // 设置子窗口的标题
    subwindow1->setWindowTitle("修图模式");

    subwindow1->show();

}

void MainWindow::on_TurnToMode2_button_clicked()
{
    subwindow2 = new SubWindow2(this);

    subwindow2->setWindowTitle("组图模式");
    subwindow2->show();

}

void MainWindow::on_ExitProcess_button_clicked()
{
    this->close();
}

void MainWindow::on_TurnToMode3_button_clicked()
{
    subwindow3 = new SubWindow3(this);

    subwindow3->setWindowTitle("GIF模式");
    subwindow3->show();
}

void MainWindow::on_pushButton_clicked()
{
    subwindow4 = new SubWindow4(this);

    subwindow4->setWindowTitle("视频模式");
    subwindow4->show();
}
