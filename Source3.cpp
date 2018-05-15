#include "Source3.h"

#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkInteractorStyleFlight.h"

static HANDLE hist;



MyVTKApp::MyVTKApp(HWND parent)
{
	renderer = vtkRenderer::New();
	renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	renWin->SetParentId(parent);
	iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	vtkInteractorStyleFlight *flightStyle = vtkInteractorStyleFlight::New();
	iren->SetInteractorStyle(flightStyle);

	cone = vtkConeSource::New();
	cone->SetHeight(2.0);
	cone->SetRadius(2.4);
	cone->SetResolution(100);
	coneMapper = vtkPolyDataMapper::New();
	coneMapper->SetInputConnection(cone->GetOutputPort());
	coneActor = vtkActor::New();
	coneActor->SetMapper(coneMapper);


	renderer->AddActor(coneActor);
	renderer->SetBackground(0.2, 0.4, 0.3);
	renderer->ResetCamera();
	renWin->SetSize(800, 600);
	renWin->Render();
}
MyVTKApp::~MyVTKApp()
{
	renWin->Delete();
	renderer->Delete();
	iren->Delete();
	cone->Delete();
	coneMapper->Delete();
	coneActor->Delete();

}