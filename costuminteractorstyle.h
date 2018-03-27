#ifndef COSTUMINTERACTORSTYLE_H
#define COSTUMINTERACTORSTYLE_H

/*
 * 自定义交互模式
 * 可选的继承基类是vtkInteractorStyle 和vtkInteractorStyleTrackball
 */

#include <vtkInteractorStyle.h>


class CostumInteractorStyle : public vtkInteractorStyle
{
public:
    CostumInteractorStyle();
};

#endif // COSTUMINTERACTORSTYLE_H
