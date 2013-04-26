//
// This example reads ascii files where each line consists of points with its position (x,y,z) 
// and (optionally) one scalar or binary files in RAW 3d file format.
//
// some standard vtk headers

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkTransform.h>

// headers needed for this example
#include <vtkParticleReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataReader.h>

// Define interaction style
class MouseInteractorStyle4 : public vtkInteractorStyleTrackballCamera
{
  public:
    static MouseInteractorStyle4* New();
    vtkTypeMacro(MouseInteractorStyle4, vtkInteractorStyleTrackballCamera);
 
    virtual void OnLeftButtonDown() 
    {
      std::cout << "Pressed left mouse button." << std::endl;
      // Forward events
      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
 
    virtual void OnMiddleButtonDown() 
    {
      std::cout << "Pressed middle mouse button." << std::endl;
      // Forward events
      vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
    }
 
    virtual void OnRightButtonDown() 
    {
      std::cout << "Pressed right mouse button." << std::endl;
      // Forward events
      vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }
 
};
 
vtkStandardNewMacro(MouseInteractorStyle4);


// needed to easily convert int to std::string
int main(int argc, char* argv[])
{
   // Verify input arguments
   if ( argc != 3 )
   {
      std::cout << "Usage: " << argv[0]
      << " Filename(.tre) Filename(.vtk)" << std::endl;
      return EXIT_FAILURE;
   }

   std::string filePath = argv[1];
   std::string labelPath = argv[2];
   // Tortuosity provided by Ryan is big endian encoded
   //std::string filePath = "C:\\VTK\\vtkdata-5.8.0\\Data\\Particles.raw";
   // Read the file
   vtkSmartPointer<vtkParticleReader> reader =
      vtkSmartPointer<vtkParticleReader>::New();

   reader->SetFileName ( filePath.c_str() );
   // if nothing gets displayed or totally wrong, swap the endianness
   reader->SetDataByteOrderToBigEndian();
   reader->Update();

  vtkSmartPointer<vtkPolyDataReader> labelReader =
      vtkSmartPointer<vtkPolyDataReader>::New();
  labelReader->SetFileName ( labelPath.c_str() );
  labelReader->Update();

  vtkSmartPointer<vtkTransform> shrinkTransform = 
      vtkSmartPointer<vtkTransform>::New();
  shrinkTransform->Scale(0.05, 0.05, 0.05);
  vtkSmartPointer<vtkTransform> translateTransform = 
      vtkSmartPointer<vtkTransform>::New();
  // A +z translation of 7.5 is needed in Slicer RAS coords.
  translateTransform->Translate(14, 6, 0);
   // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
     vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
  std::cout << "number of pieces: " << mapper->GetNumberOfPieces() << std::endl;
  mapper->SetScalarRange(3, 9);

  vtkSmartPointer<vtkPolyDataMapper> labelMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  labelMapper->SetInputConnection(labelReader->GetOutputPort());
  std::cout << "number of labels: " << labelMapper->GetNumberOfPieces() << std::endl;
  labelMapper->SetScalarRange(0, 10);

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();

  actor->SetMapper(mapper);
  actor->GetProperty()->SetPointSize(2);
  actor->SetUserTransform(shrinkTransform);

  vtkSmartPointer<vtkActor> labelActor = 
    vtkSmartPointer<vtkActor>::New();
  labelActor->SetMapper(labelMapper);
  labelActor->SetUserTransform(translateTransform);
  labelActor->GetProperty()->SetOpacity(0.5);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  vtkSmartPointer<MouseInteractorStyle4> style =
    vtkSmartPointer<MouseInteractorStyle4>::New();
  renderWindowInteractor->SetInteractorStyle( style );

  renderer->AddActor(actor);
  renderer->AddActor(labelActor);
  renderer->SetBackground(0, 0, 0);

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
