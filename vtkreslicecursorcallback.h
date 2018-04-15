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
#include <vtkResliceImageViewer.h>

class VtkToQtSignal;
//class vtkResliceImageViewer;

template<typename T>
void vtkValueMessageTemplate(vtkImageData* image, int* position,
                             std::string& message)
{
    T* tuple = ((T*)image->GetScalarPointer(position));
    int components = image->GetNumberOfScalarComponents();
    for (int c = 0; c < components; ++c)
    {
        message += vtkVariant(tuple[c]).ToString();
        if (c != (components - 1))
        {
            message += ", ";
        }
    }
    message += " )";
}
//get pixel intensity at position
//template<typename T>
//void vtkIntensityTemplate(vtkImageData* image, int* position, float *intensity){
//   T* touple = ((T*) image->GetScalarPointer(position));
//   int components = image->GetNumberOfScalarComponents();
//   for(int i = 0; i < components; i++){
//       intensity[i] = touple[i];
//   }
//   assert(components == 1);
//   //intensity[0] = touple[0];

//}
class vtkImageInteractionCallback : public vtkCommand
{
public:
    static vtkImageInteractionCallback *New()
    {
        return new vtkImageInteractionCallback;
    }

    vtkImageInteractionCallback()
    {
        this->Viewer     = NULL;
        this->Picker     = NULL;
        this->Annotation = NULL;
    }

    ~vtkImageInteractionCallback()
    {
        this->Viewer     = NULL;
        this->Picker     = NULL;
        this->Annotation = NULL;
    }

    void SetPicker(vtkPropPicker *picker)
    {
        this->Picker = picker;
    }

    void SetAnnotation(vtkCornerAnnotation *annotation)
    {
        this->Annotation = annotation;
    }

    void SetViewer(vtkResliceImageViewer *viewer)
    {
        this->Viewer = viewer;
    }

    virtual void Execute(vtkObject *, unsigned long vtkNotUsed(event), void *)
    {
        vtkRenderWindowInteractor *interactor =
        this->Viewer->GetRenderWindow()->GetInteractor();
        vtkRenderer* renderer = this->Viewer->GetRenderer();
        vtkImageActor* actor = this->Viewer->GetImageActor();
        vtkImageData* image = this->Viewer->GetInput();
        vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(
                                                                     interactor->GetInteractorStyle());

#if VTK_MAJOR_VERSION <= 5
        image->Update();
#endif
    std::cout << interactor->GetEventPosition()[0] << std::endl;
        // Pick at the mouse location provided by the interactor
        this->Picker->Pick(interactor->GetEventPosition()[0],
                           interactor->GetEventPosition()[1],
                           0.0, renderer);

        // There could be other props assigned to this picker, so
        // make sure we picked the image actor
    std::cout<< "debug" << std::endl;
        vtkAssemblyPath* path = this->Picker->GetPath();
        bool validPick = false;

        if (path)
        {
            vtkCollectionSimpleIterator sit;
            path->InitTraversal(sit);
            vtkAssemblyNode *node;
            for (int i = 0; i < path->GetNumberOfItems() && !validPick; ++i)
            {
                node = path->GetNextNode(sit);
                if (actor == vtkImageActor::SafeDownCast(node->GetViewProp()))
                {
                    validPick = true;
                }
            }
        }

        if (!validPick)
        {
            this->Annotation->SetText(0, "Off Image");
            interactor->Render();
            // Pass the event further on
            style->OnMouseMove();
            return;
        }

        // Get the world coordinates of the pick
        double pos[3];
        this->Picker->GetPickPosition(pos);

        int image_coordinate[3];

        int axis = this->Viewer->GetSliceOrientation();
        switch (axis)
        {
            case vtkImageViewer2::SLICE_ORIENTATION_XZ:
                image_coordinate[0] = vtkMath::Round(pos[0]);
                image_coordinate[1] = this->Viewer->GetSlice();
                image_coordinate[2] = vtkMath::Round(pos[2]);
                break;
            case vtkImageViewer2::SLICE_ORIENTATION_YZ:
                image_coordinate[0] = this->Viewer->GetSlice();
                image_coordinate[1] = vtkMath::Round(pos[0]);
                image_coordinate[2] = vtkMath::Round(pos[1]);
                break;
            default:  // vtkImageViewer2::SLICE_ORIENTATION_XY
                image_coordinate[0] = vtkMath::Round(pos[0]);
                image_coordinate[1] = vtkMath::Round(pos[1]);
                image_coordinate[2] = this->Viewer->GetSlice();
                break;
        }

        std::string message = "Location: ( ";
        message += vtkVariant(image_coordinate[0]).ToString();
        message += ", ";
        message += vtkVariant(image_coordinate[1]).ToString();
        message += ", ";
        message += vtkVariant(image_coordinate[2]).ToString();
        message += " )\nValue: ( ";

        switch (image->GetScalarType())
        {
                vtkTemplateMacro((vtkValueMessageTemplate<VTK_TT>(image,
                                                                  image_coordinate,
                                                                  message)));

            default:
                return;
        }

        this->Annotation->SetText( 0, message.c_str() );
        interactor->Render();
        style->OnMouseMove();
    }

private:
    vtkResliceImageViewer*      Viewer;      // Pointer to the viewer
    vtkPropPicker*        Picker;      // Pointer to the picker
    vtkCornerAnnotation*  Annotation;  // Pointer to the annotation
};

/*
class vtkResliceCursorCallback : public vtkCommand
{
public:
   static vtkResliceCursorCallback *New() {
       return new vtkResliceCursorCallback;
   }

   vtkResliceCursorCallback(){
       for(int i = 0; i < 3; i++){
           this->Viewers = NULL;
       }
       this->picker = NULL;
   }
   ~vtkResliceCursorCallback(){
       for(int i = 0; i < 3; i++){
           this->Viewers = NULL;
       }
       this->picker = NULL;
   }

   void SetPicker(vtkPropPicker * picker){
       this->picker = picker;
   }
   void SetViewer(vtkResliceImageViewer *Viewers){
       for(int i = 0; i < 3; i++){
           this->Viewers = Viewers;
       }
   }

   void Execute( vtkObject *caller, unsigned long ev, void *callData ){

//        vtkResliceImageViewer * viewer = dynamic_cast< vtkResliceImageViewer* >(caller); //get caller 调用者不是viewer，而是interactor

        assert(Viewers != NULL);

        vtkRenderWindowInteractor * interactor = this->Viewers->GetRenderWindow()->GetInteractor();
        assert(interactor!=NULL);
        vtkRenderer* renderer = Viewers->GetRenderer();
        assert(renderer!=NULL);
        vtkImageActor* actor = Viewers->GetImageActor();
        assert(actor!=NULL);
        vtkImageData* image = Viewers->GetInput(); //use to get image data
        assert(image != NULL);
        assert(picker != NULL);
    std::cout << "do nothing" << std::endl;
        vtkInteractorStyle* style = vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());

        this->picker->Pick(interactor->GetEventPosition()[0],
                           interactor->GetEventPosition()[1],
                            0.0, renderer);
    std::cout << "debug" << std::endl;
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
       int axis = Viewers->GetSliceOrientation();
       switch (axis) {
       case vtkResliceImageViewer::SLICE_ORIENTATION_XZ:
           image_coordiante[0] = vtkMath::Round(pos[0]);
           image_coordiante[1] = Viewers->GetSlice();
           image_coordiante[2] = vtkMath::Round(pos[2]);
           break;
       case vtkResliceImageViewer::SLICE_ORIENTATION_YZ:
           image_coordiante[0] = Viewers->GetSlice();
           image_coordiante[1] = vtkMath::Round(pos[0]);
           image_coordiante[2] = vtkMath::Round(pos[1]);
           break;
       default:
           image_coordiante[0] = vtkMath::Round(pos[0]);
           image_coordiante[1] = vtkMath::Round(pos[1]);
           image_coordiante[2] = Viewers->GetSlice();
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
   vtkResliceImageViewer * Viewers;
   vtkPropPicker * picker;
};
*/
#endif // VTKRESLICECURSORCALLBACK_H
