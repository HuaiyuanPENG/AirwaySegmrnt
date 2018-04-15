#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "contrastadujestdialog.h"
#include <iostream>
#include <vtkIndent.h>
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
#include <vtkImageFlip.h>

#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>
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
//    vtkSmartPointer<vtkImageFlip> flipXaxis = vtkSmartPointer<vtkImageFlip>::New();
//    flipXaxis->SetFilteredAxes(1);
//    flipXaxis->SetInputData(connector->GetOutput());
//    flipXaxis->Update();
    image->DeepCopy(connector->GetOutput());


    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    ui->HorizontalWindow->SetRenderWindow(m_pImageViewer[0]->GetRenderWindow());
    m_pImageViewer[0]->SetupInteractor(interactor);
//    m_pImageViewer[0]->SetupInteractor(ui->HorizontalWindow->GetRenderWindow()->GetInteractor());
    m_pImageViewer[0]->SetSliceOrientationToXY();
    ui->CoronalWindow->SetRenderWindow(m_pImageViewer[1]->GetRenderWindow());
    m_pImageViewer[1]->SetupInteractor(ui->CoronalWindow->GetRenderWindow()->GetInteractor());
    m_pImageViewer[1]->SetSliceOrientationToXZ();
    ui->SagittalWindow->SetRenderWindow(m_pImageViewer[2]->GetRenderWindow());
    m_pImageViewer[2]->SetupInteractor(ui->SagittalWindow->GetRenderWindow()->GetInteractor());
    m_pImageViewer[2]->SetSliceOrientationToYZ();
    for(int i = 0; i < 3; i++){
        vtkResliceCursorLineRepresentation *rep = vtkResliceCursorLineRepresentation::SafeDownCast(
                    m_pImageViewer[i]->GetResliceCursorWidget()->GetRepresentation());
        m_pImageViewer[i]->SetResliceCursor(m_pImageViewer[0]->GetResliceCursor());
        rep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);
        m_pImageViewer[i]->SetInputData(image);
//        m_pImageViewer[i]->SetSliceOrientation((i+1)%3);
        m_pImageViewer[i]->SetResliceModeToAxisAligned();
    }
    ui->HSliceNum->setText(QString::number(m_pImageViewer[0]->GetSlice()) + " of " + QString::number(m_pImageViewer[0]->GetSliceMax()));
    ui->CSliceNum->setText(QString::number(m_pImageViewer[1]->GetSlice()) + " of " + QString::number(m_pImageViewer[1]->GetSliceMax()));
    ui->SSliceNum->setText(QString::number(m_pImageViewer[2]->GetSlice()) + " of " + QString::number(m_pImageViewer[2]->GetSliceMax()));

 connection = vtkSmartPointer<vtkEventQtSlotConnect>::New();
 connection->Connect(m_pImageViewer[0], vtkResliceImageViewer::SliceChangedEvent,
                    this, SLOT(sliceMouseWheelChanged(vtkObject*)));
 connection->Connect(m_pImageViewer[1], vtkResliceImageViewer::SliceChangedEvent,
                    this, SLOT(sliceMouseWheelChanged(vtkObject*)));
 connection->Connect(m_pImageViewer[2], vtkResliceImageViewer::SliceChangedEvent,
                    this, SLOT(sliceMouseWheelChanged(vtkObject*)));

//connection->Connect(m_pImageViewer[0]->GetInteractor(), vtkCommand::LeftButtonPressEvent,
//                    this, SLOT(showCursorPosition(vtkObject*)));
//    vtkSmartPointer<vtkPropPicker> picker[3];
    vtkSmartPointer<CostumInteractorStyle> cosStyle[3];
//    vtkImageActor* imageactor[3];
//    = vtkSmartPointer<CostumInteractorStyle>::New();

//    for(int i = 0; i < 3; i++){
//        cosStyle[i] = vtkSmartPointer<CostumInteractorStyle>::New();
//        cosStyle[i] = vtkSmartPointer<vtkInteractorStyleImage>::New();
//        resliceCursorCallback[i] = vtkSmartPointer<vtkResliceCursorCallback>::New();
//        resliceCursorCallback[i]->SetViewer(m_pImageViewer[i]);
//        picker[i] = vtkSmartPointer<vtkPropPicker>::New();
//        picker[i]->PickFromListOn();
//        imageactor[i] = m_pImageViewer[i]->GetImageActor();
//        picker[i]->AddPickList(imageactor[i]);
//        imageactor[i]->InterpolateOff();
//        resliceCursorCallback[i]->SetPicker(picker[i]);
//    }
//    for(int i = 0; i < 3; i++){
//        if(m_pImageViewer[i]->GetInteractor()){
//            m_pImageViewer[i]->GetResliceCursorWidget()->SetInteractor(m_pImageViewer[i]->GetInteractor());
//            m_pImageViewer[i]->GetInteractor()->SetInteractorStyle(cosStyle[i]);
//            m_pImageViewer[i]->AddObserver(vtkCommand::LeftButtonPressEvent, resliceCursorCallback); //doesn't work
//            m_pImageViewer[i]->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent, resliceCursorCallback[i]);
//            m_pImageViewer[i]->GetInteractorStyle()->AddObserver(vtkCommand::LeftButtonPressEvent, resliceCursorCallback[i]);
//            m_pImageViewer[i]->GetInteractor()->AddObserver(vtkCommand::MouseMoveEvent, resliceCursorCallback);
//            m_pImageViewer[i]->GetInteractor()->Disable();
//            m_pImageViewer[i]->GetResliceCursorWidget()->AddObserver(vtkCommand::LeftButtonPressEvent, resliceCursorCallback, 0.6);
//        }
//    }
    /*
    vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
    picker->PickFromListOn();
    vtkImageActor * imageactor = m_pImageViewer[0]->GetImageActor();

    picker->AddPickList(imageactor);
    imageactor->InterpolateOff();
    vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation->SetLinearFontScaleFactor(2);
        cornerAnnotation->SetNonlinearFontScaleFactor(1);
        cornerAnnotation->SetMaximumFontSize(20);
        cornerAnnotation->SetText(0, "Off Image");
        cornerAnnotation->SetText(3, "<window>\n<level>");
        cornerAnnotation->GetTextProperty()->SetColor(1, 0, 0);
        m_pImageViewer[0]->GetRenderer()->AddViewProp(cornerAnnotation);

        vtkSmartPointer<vtkImageInteractionCallback> callback = vtkSmartPointer<vtkImageInteractionCallback>::New();
       callback->SetViewer(m_pImageViewer[0]);
       callback->SetAnnotation(cornerAnnotation);
       callback->SetPicker(picker);
       */
//       ui->HorizontalWindow->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
//       vtkInteractorStyleImage *style = vtkInteractorStyleImage::New();
//       ui->HorizontalWindow->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
//    m_pImageViewer[0]->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent,callback);
//   vtkInteractorStyleImage * imageStyle = m_pImageViewer[0]->GetInteractorStyle();
//   imageStyle->AddObserver(vtkCommand::LeftButtonPressEvent,callback);
//    m_pImageViewer[0]->GetInteractor()->Initialize();
//    m_pImageViewer[0]->GetInteractor()->Start();
//    m_pImageViewer[0]->GetRenderer()->SetActiveCamera(m_pImageViewer[1]->GetRenderer()->GetActiveCamera());
connect(ui->HorizonScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sliceChanged1(int)));
connect(ui->CoronalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sliceChanged2(int)));
connect(ui->SagittalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sliceChanged3(int)));

}

//接收窗位对话框传回的值，并设置显示
void MainWindow::receiveWindow(float w, float l)
{
    for(int i = 0; i < 3; i++){
        m_pImageViewer[i]->SetColorLevel(l);
        m_pImageViewer[i]->SetColorWindow(w);
    }
}/*
void MainWindow::showCursorPosition(int x, int y, int z, float* intensity){
    ui->label->setText(QString::number(x));
    ui->label_2->setText(QString::number(y));
    ui->label_3->setText(QString::number(z));

}*/

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
void MainWindow::showCursorPosition(vtkObject *obj){
   vtkRenderWindowInteractor * iren = vtkRenderWindowInteractor::SafeDownCast(obj);
   double x = iren->GetEventPosition()[0];
   double y = iren->GetEventPosition()[1];
   ui->label->setText(QString::number(x));
   ui->label_2->setText(QString::number(y));

}

void MainWindow::sliceChanged1(int value){
    double p = static_cast<double>(value)/static_cast<double>(ui->HorizonScrollBar->maximum());
    m_pImageViewer[0]->SetSlice( vtkMath::Round(m_pImageViewer[0]->GetSliceMax() * p));

    ui->HSliceNum->setText(QString::number(m_pImageViewer[0]->GetSlice()) + " of " + QString::number(m_pImageViewer[0]->GetSliceMax()));
}

void MainWindow::sliceChanged2(int value){
    double p = static_cast<double>(value)/static_cast<double>(ui->CoronalScrollBar->maximum());
    m_pImageViewer[1]->SetSlice( vtkMath::Round(m_pImageViewer[1]->GetSliceMax() * p));

    ui->CSliceNum->setText(QString::number(m_pImageViewer[1]->GetSlice()) + " of " + QString::number(m_pImageViewer[1]->GetSliceMax()));
}

void MainWindow::sliceChanged3(int value){

    double p = static_cast<double>(value)/static_cast<double>(ui->SagittalScrollBar->maximum());
    m_pImageViewer[2]->SetSlice( vtkMath::Round(m_pImageViewer[2]->GetSliceMax() * p));

    ui->SSliceNum->setText(QString::number(m_pImageViewer[2]->GetSlice()) + " of " + QString::number(m_pImageViewer[2]->GetSliceMax()));
}


void MainWindow::sliceMouseWheelChanged(vtkObject * obj){
//  std::cout << "connect" << std::endl;
    vtkResliceImageViewer* viewer = vtkResliceImageViewer::SafeDownCast(obj);
    int axis = viewer->GetSliceOrientation();
    switch (axis) {
    case vtkResliceImageViewer::SLICE_ORIENTATION_XY:
        ui->HSliceNum->setText(QString::number(m_pImageViewer[0]->GetSlice()) + " of " + QString::number(m_pImageViewer[0]->GetSliceMax()));

        break;
    case vtkResliceImageViewer::SLICE_ORIENTATION_XZ:
    ui->CSliceNum->setText(QString::number(m_pImageViewer[1]->GetSlice()) + " of " + QString::number(m_pImageViewer[1]->GetSliceMax()));

    default:
    ui->SSliceNum->setText(QString::number(m_pImageViewer[2]->GetSlice()) + " of " + QString::number(m_pImageViewer[2]->GetSliceMax()));
        break;
    }

//    ui->HSliceNum->setText(QString::number(m_pImageViewer[0]->GetSlice()) + " of " + QString::number(m_pImageViewer[0]->GetSliceMax()));
}








