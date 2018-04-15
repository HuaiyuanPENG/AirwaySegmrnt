#ifndef COSTUMINTERACTORSTYLE_H
#define COSTUMINTERACTORSTYLE_H

/*
 * 自定义交互模式
 * 可选的继承基类是vtkInteractorStyle 和vtkInteractorStyleTrackball
 * 原有的Pick 操作只是将事件抛出，重写该函数
 */

//#include <vtkInteractorStyle.h>
//#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkResliceImageViewer.h>
#include <vtkSmartPointer.h>

class CostumInteractorStyle : public vtkInteractorStyleImage
{
public:
//        static CostumInteractorStyle * New(){
//            return new CostumInteractorStyle;
//        }
    static CostumInteractorStyle *New();
    vtkTypeMacro(CostumInteractorStyle, vtkInteractorStyleImage);
    void setImageViewer(vtkResliceImageViewer * viewer);
    void Pick();
    void OnLeftButtonDown();
private:
    vtkSmartPointer<vtkResliceImageViewer> resliceViewer;


};

#endif // COSTUMINTERACTORSTYLE_H
