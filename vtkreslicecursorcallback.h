#ifndef VTKRESLICECURSORCALLBACK_H
#define VTKRESLICECURSORCALLBACK_H
//定义指针的回调函数
#include <vtkCommand.h>
class vtkResliceCursorCallback : public vtkCommand
{
public:
   static vtkResliceCursorCallback *New() { return new vtkResliceCursorCallback; }
   //implement precedure
   //  执行放缩图像的操作，实现方式是调整相机到slice 的距离
   void Execute( vtkObject *caller, unsigned long ev, viod *callData ){

   }


};

#endif // VTKRESLICECURSORCALLBACK_H
