#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenu>
#include <QFileDialog>
#include <QDir>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <QVTKWidget.h>
#include <vtkImageActor.h>
#include <string.h>
#include <vtkResliceImageViewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>

#include "itkimagetype.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i = 0; i < 3; i++){
           m_pImageViewer[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_File_triggered()
{

}
