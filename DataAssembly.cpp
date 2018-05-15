#include "stdafx.h"
#include "DataAssembly.h"

#include "vtkPolyDataMapper.h"
#include "vtkLODActor.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkInteractorStyleFlight.h"
#include "vtkSphereSource.h"
#include "vtkCubeSource.h"
#include "vtkConeSource.h"
#include "vtkCylinderSource.h"
#include "vtkAssembly.h"


void Assembly46()
{
	vtkSphereSource* sphere = vtkSphereSource::New();
	vtkPolyDataMapper*spheremapper = vtkPolyDataMapper::New();
	spheremapper->SetInputConnection(sphere->GetOutputPort());
	vtkActor*sphereactor = vtkActor::New();
	sphereactor->SetMapper(spheremapper);
	sphereactor->SetOrigin(2, 1, 3);
	sphereactor->RotateY(6);
	sphereactor->SetPosition(2.25, 0, 0);
	sphereactor->GetProperty()->SetColor(1, 0, 1);

	vtkCubeSource*cube = vtkCubeSource::New();
	vtkPolyDataMapper*cubemapper = vtkPolyDataMapper::New();
	cubemapper->SetInputConnection(cube->GetOutputPort());
	vtkActor*cubeactor = vtkActor::New();
	cubeactor->SetMapper(cubemapper);
	cubeactor->SetPosition(0.0, 0.25, 0);
	cubeactor->GetProperty ()->SetColor(0.0, 0.0, 1.0);

	vtkConeSource*cone = vtkConeSource::New();
	vtkPolyDataMapper*conemapper = vtkPolyDataMapper::New();
	conemapper->SetInputConnection(cone->GetOutputPort());
	vtkActor*coneactor = vtkActor::New();
	coneactor->SetMapper(conemapper);
	coneactor->SetPosition(0.0, 0.0, 0.25);
	coneactor->GetProperty()->SetColor(0.0, 1.0, 0.0);

	vtkCylinderSource*cylinder = vtkCylinderSource::New();
	vtkPolyDataMapper*cylindermapper = vtkPolyDataMapper::New();
	cylindermapper->SetInputConnection(cylinder->GetOutputPort());
	vtkActor*cylinderactor = vtkActor::New();
	cylinderactor->SetMapper(cylindermapper);
	cylinderactor->GetProperty()->SetColor(1, 0, 0);

	vtkAssembly *assembly = vtkAssembly::New();
	assembly->AddPart(cylinderactor);
	assembly->AddPart(sphereactor);
	assembly->AddPart(cubeactor);
	assembly->AddPart(coneactor);
	assembly->SetOrigin(5, 10, 15);
	assembly->AddPosition(5, 0, 0);
	assembly->RotateX(15);

	vtkRenderer*ren1 = vtkRenderer::New();
	vtkRenderWindow*renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	ren1->AddActor(assembly);
	ren1->AddActor(coneactor);
	ren1->SetBackground(0.3, 0.0, 0.0);
	renWin->SetSize(300, 300);

	iren->Initialize();

	vtkCamera*camera = vtkCamera::New();
	camera->SetClippingRange(21.9464, 30.0179);
	camera->SetFocalPoint(3.49221, 2.28844, - 0.970866);
	camera->SetPosition(3.49221, 2.28844, 24.5216);
	camera->SetViewAngle(30);
	camera->SetViewUp(0, 1, 0);
	ren1->SetActiveCamera(camera);


	renWin->Render();

	iren->Start();
}
void EndAssembly46()
{

}
