#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkEventQtSlotConnect.h>

class vtkImageViewer2;
class vtkResliceImageViewer;
class vtkRenderer;
class vtkRenderWindowInteractor;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_File_triggered();

private:
    Ui::MainWindow *ui;
    vtkSmartPointer< vtkResliceImageViewer > m_pImageViewer[3];
};

#endif // MAINWINDOW_H
