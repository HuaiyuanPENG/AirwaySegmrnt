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
vtkStandardNewMacro(CostumInteractorStyle);
//调用父类的构造函数，该类不做操作
//CostumInteractorStyle::CostumInteractorStyle()
//{

//}

void CostumInteractorStyle::OnMouseMove(){

    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    switch (this->State) {
    case VTKIS_WINDOW_LEVEL:
        this->FindPokedRenderer(x, y);
        this->WindowLevel();
        this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
        break;
    case VTKIS_PICK:
        this->FindPokedRenderer(x, y);
        this->Pick();
        this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
        break;
    case VTKIS_SLICE:
        this->FindPokedRenderer(x, y);
        this->Slice();
        this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
        break;
    }
    this->Superclass::OnMouseMove();
}

void CostumInteractorStyle::OnLeftButtonDown(){
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->FindPokedRenderer(x, y);
    if(this->CurrentRenderer == NULL){
        return;
    }
    this->GrabFocus(this->EventCallbackCommand);
      if (!this->Interactor->GetShiftKey() && !this->Interactor->GetControlKey())
      {
        this->WindowLevelStartPosition[0] = x;
        this->WindowLevelStartPosition[1] = y;
        this->StartWindowLevel();
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

void CostumInteractorStyle::OnLeftButtonUp(){
    switch (this->State)
    {
      case VTKIS_WINDOW_LEVEL:
        this->EndWindowLevel();
        if ( this->Interactor )
        {
          this->ReleaseFocus();
        }
        break;

      case VTKIS_SLICE:
        this->EndSlice();
        if ( this->Interactor )
        {
          this->ReleaseFocus();
        }
        break;
    }

    // Call parent to handle all other states and perform additional work

    this->Superclass::OnLeftButtonUp();
}

void CostumInteractorStyle::WindowLevel(){

}

void CostumInteractorStyle::StartWindowLevel(){
    if (this->State != VTKIS_NONE)
     {
       return;
     }
     this->StartState(VTKIS_WINDOW_LEVEL);//设置状态

     // Get the last (the topmost) image
     this->SetCurrentImageNumber(this->CurrentImageNumber);

     if (this->HandleObservers &&
         this->HasObserver(vtkCommand::StartWindowLevelEvent))
     {
       this->InvokeEvent(vtkCommand::StartWindowLevelEvent, this);
     }
     else
     {
       if (this->CurrentImageProperty)
       {
         vtkImageProperty *property = this->CurrentImageProperty;
         this->WindowLevelInitial[0] = property->GetColorWindow();
         this->WindowLevelInitial[1] = property->GetColorLevel();
       }
     }
}
