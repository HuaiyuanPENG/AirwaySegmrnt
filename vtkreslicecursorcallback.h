#ifndef VTKRESLICECURSORCALLBACK_H
#define VTKRESLICECURSORCALLBACK_H
//定义指针的回调函数
//按下鼠标左键拖动不在改变窗口的窗位窗宽，而是拾取坐标点的属性
#include <vtkCommand.h>
class vtkResliceCursorCallback : public vtkCommand
{
public:
   static vtkResliceCursorCallback *New() { return new vtkResliceCursorCallback; }
   void Execute( vtkObject *caller, unsigned long ev, viod *callData ){
        //do nothing
   }
//public:
//   vtkResliceImageViewer * Viewer[3];
};

#endif // VTKRESLICECURSORCALLBACK_H
