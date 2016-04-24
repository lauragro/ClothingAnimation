#include "window.h"
#include "foundation.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Create an instance of the widget and sim used for simulation
    myglwidget = new GLWidget(parent);

    // Create all required components of application
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    cout << "window deconstruction" << endl;
    delete myglwidget;
    delete ui;
}
