#ifndef COSTUMINTERACTORSTYLE_H
#define COSTUMINTERACTORSTYLE_H

/*
 * 自定义交互模式
 * 可选的继承基类是vtkInteractorStyle 和vtkInteractorStyleTrackball
 */

//#include <vtkInteractorStyle.h>
//#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>

class CostumInteractorStyle : public vtkInteractorStyleImage
{
public:
//        static CostumInteractorStyle * New(){
//            return new CostumInteractorStyle;
//        }
    static CostumInteractorStyle *New();
    vtkTypeMacro(CostumInteractorStyle, vtkInteractorStyleImage);
    //控制按下鼠标左键后移动的操作
    virtual void OnMouseMove();
    virtual void OnLeftButtonDown();
    virtual void OnLeftButtonUp();

    // 修改原有的交互控制窗位、窗宽的操作，战时不做操作，计划完成拾取图像像素属性，坐标与灰度值
    virtual void WindowLevel();
    virtual void StartWindowLevel();



};

#endif // COSTUMINTERACTORSTYLE_H
