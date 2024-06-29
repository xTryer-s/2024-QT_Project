#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "subwindow1.h"
#include "subwindow2.h"
#include "subwindow3.h"
#include "subwindow4.h"

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
    void on_TurnToMode1_button_clicked();

    void on_TurnToMode2_button_clicked();

    void on_ExitProcess_button_clicked();

    void on_TurnToMode3_button_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    SubWindow1 *subwindow1;
    SubWindow2 *subwindow2;
    SubWindow3 *subwindow3;
    SubWindow4 *subwindow4;
};
#endif // MAINWINDOW_H
