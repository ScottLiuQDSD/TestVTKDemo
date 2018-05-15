#include "stdafx.h"
#include "Source41.h"

#include "vtkCylinderSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"

LRESULT CreateSimpleModel41(HWND hWnd)
{
	vtkCylinderSource *cylinder = vtkCylinderSource::New();
	cylinder->SetResolution(80);

	vtkPolyDataMapper *cylinderMapper = vtkPolyDataMapper::New();
	cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

	vtkActor *cylinderActor = vtkActor::New();
	cylinderActor->SetMapper(cylinderMapper);
	cylinderActor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
	cylinderActor->RotateX(30.0);
	cylinderActor->RotateY(-45.0);

	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	ren1->AddActor(cylinderActor);
	ren1->SetBackground(0.1, 0.2, 0.4);
	renWin->SetSize(200, 200);

	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.5);
	renWin->Render();

	iren->Start();


	cylinder->Delete();
	cylinderMapper->Delete();
	cylinderActor->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();


	return S_OK;
}

