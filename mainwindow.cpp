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
    QString fileNameOpen = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Image Files (*.nii)") );
    DicomReaderType::Pointer reader = DicomReaderType::New();
    ConnectorType::Pointer connector = ConnectorType::New();
    reader->SetFileName(fileNameOpen.toStdString());

    try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject & err)
    {
        std::cerr << "ExceptionObject caght!" << std::endl;
        std::cerr << err << std::endl;
    }

    connector->SetInput(reader->GetOutput()); //数据类型转换 此时可讲connector作为数据源 connector 输出是vtkimagedata

    try{
        connector->Update();
    }
    catch(itk::ExceptionObject &err){
        std::cerr << "ExceptionObject caght!" << std::endl;
        std::cerr << err << std::endl;
    }
    //深拷贝，不这样做的话操作图像程序会直接退出。
    vtkImageData * image = vtkImageData::New();
    image->DeepCopy(connector->GetOutput());

    ui->HorizontalWindow->SetRenderWindow(m_pImageViewer[0]->GetRenderWindow());
    m_pImageViewer[0]->SetupInteractor(ui->HorizontalWindow->GetRenderWindow()->GetInteractor());  //这一步是否有意义？
    m_pImageViewer[0]->SetSliceOrientationToXY();
    m_pImageViewer[0]->SetInputData(image);
    m_pImageViewer[0]->SetSlice(m_pImageViewer[0]->GetSliceMax()/2);
    ui->HorizontalWindow->update();

    ui->CoronalWindow->SetRenderWindow(m_pImageViewer[1]->GetRenderWindow());
    m_pImageViewer[1]->SetupInteractor(ui->CoronalWindow->GetRenderWindow()->GetInteractor());  //这一步是否有意义？
    m_pImageViewer[1]->SetSliceOrientationToXZ();
    m_pImageViewer[1]->SetInputData(image);
    m_pImageViewer[1]->SetSlice(m_pImageViewer[1]->GetSliceMax()/2);
    ui->CoronalWindow->update();

    ui->SagittalWindow->SetRenderWindow(m_pImageViewer[2]->GetRenderWindow());
    m_pImageViewer[2]->SetupInteractor(ui->SagittalWindow->GetRenderWindow()->GetInteractor());  //这一步是否有意义？
    m_pImageViewer[2]->SetSliceOrientationToYZ();
    m_pImageViewer[2]->SetInputData(image);
    m_pImageViewer[2]->SetSlice(m_pImageViewer[2]->GetSliceMax()/2);
    ui->SagittalWindow->update();

}
