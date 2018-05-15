#include "stdafx.h"
#include "LoadCADPart.h"

#include "vtkSTLReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkLODActor.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkInteractorStyleFlight.h"

LRESULT LoadCADParts()
{

	vtkSTLReader *part = vtkSTLReader::New();
	part->SetFileName("G:\\VTK-8.1.0\\build\\ExternalData\\Testing\\Data\\42400-IDGH.stl");

	vtkPolyDataMapper *partMapper = vtkPolyDataMapper::New();
	partMapper->SetInputConnection(part->GetOutputPort());

	vtkLODActor *partActor = vtkLODActor::New();
	partActor->SetMapper(partMapper);
	partActor->GetProperty()->SetColor(0.82745, 0.82745, 0.82745);

	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	vtkInteractorStyleFlight *flightStyle = vtkInteractorStyleFlight::New();
	iren->SetInteractorStyle(flightStyle);

	ren1->AddActor(partActor);
	ren1->SetBackground(0.1, 0.2, 0.4);
	renWin->SetSize(300, 300);

	iren->Initialize();

	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.6);
	renWin->Render();

	partActor->Delete();
	partMapper->Delete();
	part->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();

	return S_OK;
}
