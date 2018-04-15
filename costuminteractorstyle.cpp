#include "costuminteractorstyle.h"
#include "vtkAbstractPropPicker.h"
#include "vtkAssemblyPath.h"
#include "vtkPropCollection.h"

#include "vtkCallbackCommand.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageSlice.h"
#include "vtkImageMapper3D.h"
#include "vtkImageProperty.h"
#include <vtkAssemblyPath.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkSmartPointer.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(CostumInteractorStyle);
//调用父类的构造函数，该类不做操作
//CostumInteractorStyle::CostumInteractorStyle()
//{

//}

void CostumInteractorStyle::setImageViewer(vtkResliceImageViewer *viewer){
    this->resliceViewer = viewer;
}

void CostumInteractorStyle::Pick(){
    this->InvokeEvent(vtkCommand::PickEvent, this);
    vtkSmartPointer<vtkPropPicker> proppicker = vtkSmartPointer<vtkPropPicker>::New();
    proppicker->PickFromListOn();

    vtkImageActor * imageActor = this->resliceViewer->GetImageActor();
    proppicker->AddPickList(imageActor);
    imageActor->InterpolateOff();
    vtkRenderWindowInteractor * interactor = this->resliceViewer->GetRenderWindow()->GetInteractor();
    vtkRenderer *renderer = this->resliceViewer->GetRenderer();
    vtkImageData *image = this->resliceViewer->GetInput();

    proppicker->Pick(interactor->GetEventPosition()[0],
                     interactor->GetEventPosition()[1],
                     0.0, renderer);
    vtkAssemblyPath* path = proppicker->GetPath();
    bool validPick = false;
    if(path){
        vtkCollectionSimpleIterator sit;
        path->InitTraversal();
        vtkAssemblyNode* node;
        for(int i = 0; i < path->GetNumberOfItems() && !validPick; i++){
            node = path->GetNextNode(sit);
            if(imageActor == vtkImageActor::SafeDownCast(node->GetViewProp())){
                validPick = true;
            }
        }
    }
    if(!validPick){
        return;
    }
   double pos[3];
   proppicker->GetPickPosition(pos);
   std::cout << pos[0]<< " " << pos[1]<<" "<< pos[2]<<std::endl;


}
void CostumInteractorStyle::OnLeftButtonDown(){
    int x = this->Interactor->GetEventPosition()[0];
     int y = this->Interactor->GetEventPosition()[1];

     this->FindPokedRenderer(x, y);
     if (this->CurrentRenderer == NULL)
     {
       return;
     }

     // Redefine this button to handle window/level
     this->GrabFocus(this->EventCallbackCommand);
     if (!this->Interactor->GetShiftKey() && !this->Interactor->GetControlKey())
     {
       this->WindowLevelStartPosition[0] = x;
       this->WindowLevelStartPosition[1] = y;
//       this->StartWindowLevel();
         this->StartPick();
     }

     // If shift is held down, do a rotation
     else if (this->InteractionMode == VTKIS_IMAGE3D &&
              this->Interactor->GetShiftKey())
     {
       this->StartRotate();
     }

     // If ctrl is held down in slicing mode, slice the image
     else if (this->InteractionMode == VTKIS_IMAGE_SLICING &&
              this->Interactor->GetControlKey())
     {
       this->StartSlice();
     }


     // The rest of the button + key combinations remain the same

     else
     {
       this->Superclass::OnLeftButtonDown();
     }

}
