#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "contrastadujestdialog.h"

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
#include <vtkResliceCursorLineRepresentation.h>
#include <vtkResliceCursorThickLineRepresentation.h>
#include <vtkResliceCursorWidget.h>
#include <vtkResliceCursorActor.h>
#include <vtkResliceCursorPolyDataAlgorithm.h>
#include <vtkResliceCursor.h>

#include "itkimagetype.h"
#include "vtkreslicecursorcallback.h"
#include <QMessageBox>
#include <costuminteractorstyle.h>
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
//    m_pImageViewer[0]->SetSliceOrientationToXY();
//    m_pImageViewer[0]->SetInputData(image);
//    m_pImageViewer[0]->SetSlice(m_pImageViewer[0]->GetSliceMax()/2);
//    m_pImageViewer[0]->SliceScrollOnMouseWheelOff(); //关掉滑动鼠标滚轮切换slice的操作
//    ui->HorizontalWindow->update();

    ui->CoronalWindow->SetRenderWindow(m_pImageViewer[1]->GetRenderWindow());
    m_pImageViewer[1]->SetupInteractor(ui->CoronalWindow->GetRenderWindow()->GetInteractor());  //这一步是否有意义？
//    m_pImageViewer[1]->SetSliceOrientationToXZ();
//    m_pImageViewer[1]->SetInputData(image);
//    m_pImageViewer[1]->SetSlice(m_pImageViewer[1]->GetSliceMax()/2);
//    ui->CoronalWindow->update();

    ui->SagittalWindow->SetRenderWindow(m_pImageViewer[2]->GetRenderWindow());
    m_pImageViewer[2]->SetupInteractor(ui->SagittalWindow->GetRenderWindow()->GetInteractor());  //这一步是否有意义？
//    m_pImageViewer[2]->SetSliceOrientationToYZ();
//    m_pImageViewer[2]->SetInputData(image);
//    m_pImageViewer[2]->SetSlice(m_pImageViewer[2]->GetSliceMax()/2);
//    ui->SagittalWindow->update();

    for(int i = 0; i < 3; i++){
        vtkResliceCursorLineRepresentation *rep = vtkResliceCursorLineRepresentation::SafeDownCast(
                    m_pImageViewer[i]->GetResliceCursorWidget()->GetRepresentation());
        m_pImageViewer[i]->SetResliceCursor(m_pImageViewer[0]->GetResliceCursor());
        rep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);
        m_pImageViewer[i]->SetInputData(image);
        m_pImageViewer[i]->SetSliceOrientation((i+1)%3);
        m_pImageViewer[i]->SetResliceModeToAxisAligned();
    }
    vtkSmartPointer<vtkResliceCursorCallback> resliceCursorCallback  = vtkSmartPointer<vtkResliceCursorCallback>::New();
    for(int i = 0; i < 3; i++){
        resliceCursorCallback->Viewers[i] = m_pImageViewer[i];
    }
    vtkSmartPointer<CostumInteractorStyle> cosStyle[3];
//    = vtkSmartPointer<CostumInteractorStyle>::New();

    for(int i = 0; i < 3; i++){
        cosStyle[i] = vtkSmartPointer<CostumInteractorStyle>::New();
    }

    for(int i = 0; i < 3; i++){
        if(m_pImageViewer[i]->GetInteractor()){
            m_pImageViewer[i]->GetResliceCursorWidget()->SetInteractor(m_pImageViewer[i]->GetInteractor());
            m_pImageViewer[i]->GetInteractor()->SetInteractorStyle(cosStyle[i]);
//            m_pImageViewer[i]->AddObserver(vtkCommand::LeftButtonPressEvent, resliceCursorCallback, 1); //doesn't work
            m_pImageViewer[i]->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent, resliceCursorCallback);
//            m_pImageViewer[i]->GetInteractor()->AddObserver(vtkCommand::MouseMoveEvent, resliceCursorCallback);
//            m_pImageViewer[i]->GetInteractor()->Disable();
//            m_pImageViewer[i]->GetResliceCursorWidget()->AddObserver(vtkCommand::LeftButtonPressEvent, resliceCursorCallback, 0.6);
        }
    }
//    m_pImageViewer[0]->GetRenderer()->SetActiveCamera(m_pImageViewer[1]->GetRenderer()->GetActiveCamera());
}

//接收窗位对话框传回的值，并设置显示
void MainWindow::receiveWindow(float w, float l)
{
    for(int i = 0; i < 3; i++){
        m_pImageViewer[i]->SetColorLevel(l);
        m_pImageViewer[i]->SetColorWindow(w);
    }
}
void MainWindow::showCursorPosition(int x, int y, int z, float* intensity){
    ui->label->setText(QString::number(x));
    ui->label_2->setText(QString::number(y));
    ui->label_3->setText(QString::number(z));

}

//打开调整窗宽窗位对话框，连接槽函数
void MainWindow::on_actionContrast_Adjustment_triggered()
{

    ContrastAdujestDialog * cad = new ContrastAdujestDialog;
    cad->show();
    QObject::connect(cad, SIGNAL(sentWindowLevel(float, float)), this, SLOT(receiveWindow(float, float)));
}

void MainWindow::on_actionmessagebox_triggered()
{
     QMessageBox mb;
    mb.setText("debug");
    mb.exec();
}
