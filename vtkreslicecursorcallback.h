#ifndef VTKRESLICECURSORCALLBACK_H
#define VTKRESLICECURSORCALLBACK_H
//定义指针的回调函数
//按下鼠标左键拖动不在改变窗口的窗位窗宽，而是拾取坐标点的属性
//一个窗口中的事件其它的窗口也有相应的操作
#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkAssemblyPath.h>
#include <vtkPointData.h>
#include <vtkPropPicker.h>
#include <vtkInteractorStyleImage.h>
#include <vtkMath.h>
#include "vtktoqtsignal.h"

class vtkResliceImageViewer;
//get pixel intensity at position
template<typename T>
void vtkIntensityTemplate(vtkImageData* image, int* position, float *intensity){
   T* touple = ((T*) image->GetScalarPointer(position));
   int components = image->GetNumberOfScalarComponents();
   for(int i = 0; i < components; i++){
       intensity[i] = touple[i];
   }
//   assert(components == 1);
   //intensity[0] = touple[0];

}

class vtkResliceCursorCallback : public vtkCommand
{
public:
   static vtkResliceCursorCallback *New() {
       return new vtkResliceCursorCallback;
   }

   vtkResliceCursorCallback(){
       for(int i = 0; i < 3; i++){
           this->Viewers[i] = NULL;
       }
       this->picker = NULL;
   }
   ~vtkResliceCursorCallback(){
       for(int i = 0; i < 3; i++){
           this->Viewers[i] = NULL;
       }
       this->picker = NULL;
   }

   void SetPicker(vtkPropPicker * picker){
       this->picker = picker;
   }
   void SetViewer(vtkResliceImageViewer *Viewers[]){
       for(int i = 0; i < 3; i++){
           this->Viewers[i] = Viewers[i];
       }
   }

   void Execute( vtkObject *caller, unsigned long ev, void *callData ){
        std::cout << "do nothing" << std::endl;
        vtkResliceImageViewer * viewer = dynamic_cast< vtkResliceImageViewer* >(caller); //get caller
        vtkRenderWindowInteractor * interactor = viewer->GetRenderWindow()->GetInteractor();
        vtkRenderer* renderer = viewer->GetRenderer();
        vtkImageActor* actor = viewer->GetImageActor();
        vtkImageData* image = viewer->GetInput(); //use to get image data
        vtkInteractorStyle* style = vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
        this->picker->Pick(interactor->GetEventPosition()[0],
                           interactor->GetEventPosition()[1],
                            0.0, renderer);

       vtkAssemblyPath* path = this->picker->GetPath();
       bool validPick = false;
       if(path){
           vtkCollectionSimpleIterator sit;
           path->InitTraversal(sit);
           vtkAssemblyNode* node;
           for(int i = 0; i < path->GetNumberOfItems() && !validPick; i++){
               node = path->GetNextNode(sit);
               if(actor == vtkImageActor::SafeDownCast(node->GetViewProp())){
                   validPick = true;
               }

           }
       }
       // out of the image
       if(!validPick){
           return;
       }
       double pos[3];
       this->picker->GetPickPosition(pos);
       int image_coordiante[3];
       int axis = viewer->GetSliceOrientation();
       switch (axis) {
       case vtkResliceImageViewer::SLICE_ORIENTATION_XZ:
           image_coordiante[0] = vtkMath::Round(pos[0]);
           image_coordiante[1] = viewer->GetSlice();
           image_coordiante[2] = vtkMath::Round(pos[2]);
           break;
       case vtkResliceImageViewer::SLICE_ORIENTATION_YZ:
           image_coordiante[0] = viewer->GetSlice();
           image_coordiante[1] = vtkMath::Round(pos[0]);
           image_coordiante[2] = vtkMath::Round(pos[1]);
           break;
       default:
           image_coordiante[0] = vtkMath::Round(pos[0]);
           image_coordiante[1] = vtkMath::Round(pos[1]);
           image_coordiante[2] = viewer->GetSlice();
           break;

       }
       int components = image->GetNumberOfScalarComponents();
       assert(components == 1);//grayscale image
       switch (image->GetScalarType()) {
          vtkTemplateMacro((vtkIntensityTemplate<VTK_TT>(image, image_coordiante, this->intensity)));
       default:
          return;
       }
      VtkToQtSignal * vtktosignal = new VtkToQtSignal;
      vtktosignal->Execurte(image_coordiante[0], image_coordiante[1], image_coordiante[3], this->intensity); //sent signal;
   }
   float *intensity;
    friend class VtkToQtSignal;
   vtkResliceImageViewer * Viewers[3];
   vtkPropPicker * picker;
};

#endif // VTKRESLICECURSORCALLBACK_H
