#ifndef VTKRESLICECURSORCALLBACK_H
#define VTKRESLICECURSORCALLBACK_H
//定义指针的回调函数
//按下鼠标左键拖动不在改变窗口的窗位窗宽，而是拾取坐标点的属性
#include <vtkCommand.h>
//#include <QMessageBox>
class vtkResliceImageViewer;
class vtkResliceCursorCallback : public vtkCommand
{
public:
   static vtkResliceCursorCallback *New() {
       return new vtkResliceCursorCallback;
//       QMessageBox::warning(NULL, "warning", "debug", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
   }
   void Execute( vtkObject *caller, unsigned long ev, void *callData ){
        std::cout << "do nothing" << std::endl;
//        QMessageBox::warning(NULL, "warning", "debug", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

   }
//public:
   vtkResliceImageViewer * Viewer[3];

};

#endif // VTKRESLICECURSORCALLBACK_H
