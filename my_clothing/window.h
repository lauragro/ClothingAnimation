#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "glwidget.h"
#include "spring.h"

namespace Ui {
    class sim;
    class GLWidget;
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    GLWidget *myglwidget;
    Sim *mySim;
    void displaySpringDetails();

public slots:
    void box_dampingConstant(float value);
    void box_restLength(float value);
    void box_mass(float value);
    void box_springConstant(float value);
    void dampingSlider(int a);


};

#endif // MAINWINDOW_H
