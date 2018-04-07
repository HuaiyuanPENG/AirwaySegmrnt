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
    void on_actionmessagebox_triggered();

private slots:
    //调整窗口窗位和窗宽
    void on_actionContrast_Adjustment_triggered();

    void on_actionOpen_File_triggered();

    void receiveWindow(float w, float l);

    void showCursorPosition(int x, int y, int z, float* intensity);
private:
    Ui::MainWindow *ui;

    vtkSmartPointer< vtkResliceImageViewer > m_pImageViewer[3];
};

#endif // MAINWINDOW_H
