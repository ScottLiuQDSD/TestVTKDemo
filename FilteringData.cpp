#include "stdafx.h"
#include "FilteringData.h"

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
#include "vtkShrinkPolyData.h"

struct CurVTKCommon: VTKCommonParam {
	CurVTKCommon()
		:part(nullptr),
		shrink(nullptr),
		partMapper(nullptr),
		partActor(nullptr)
	{

	}
	vtkSTLReader *part;
	vtkShrinkPolyData *shrink;
	vtkPolyDataMapper *partMapper;
	vtkLODActor *partActor;
};
CurVTKCommon *curParam = nullptr;

void FilterData43()
{
	if (nullptr == curParam) {
		curParam = new CurVTKCommon; 
	}

	curParam->part = vtkSTLReader::New();
	curParam->part->SetFileName("G:\\VTK-8.1.0\\build\\ExternalData\\Testing\\Data\\42400-IDGH.stl");
	curParam->shrink = vtkShrinkPolyData::New();
	curParam->shrink->SetInputConnection(curParam->part->GetOutputPort());
	curParam->shrink->SetShrinkFactor(0.97);
	curParam->partMapper = vtkPolyDataMapper::New();
	curParam->partMapper->SetInputConnection(curParam->shrink->GetOutputPort());
	curParam->partActor = vtkLODActor::New();
	curParam->partActor->SetMapper(curParam->partMapper);
	curParam->partActor->GetProperty()->SetColor(0.12745, 0.82745, 0.82745);
	curParam->partActor->RotateX(30.0);
	curParam->partActor->RotateY(-45.0);
	curParam->partActor->GetProperty()->SetOpacity(1.0);
	curParam->partActor->GetProperty()->SetAmbient(0.25);
	curParam->partActor->GetProperty()->SetDiffuse(0.6);
	curParam->partActor->GetProperty()->SetSpecular(1.0);
	curParam->partActor->GetProperty()->SetSpecularPower(10.0);
	curParam->partActor->GetProperty()->SetAmbientColor(0.25, .2, .2);
	curParam->partActor->GetProperty()->SetDiffuseColor(0.1, .2, .4);
	curParam->partActor->GetProperty()->SetSpecularColor(1.0, 1, 1);


	curParam->ren1 = vtkRenderer::New();
	curParam->renWin = vtkRenderWindow::New();
	curParam->renWin->AddRenderer(curParam->ren1);
	curParam->iren = vtkRenderWindowInteractor::New();
	curParam->iren->SetRenderWindow(curParam->renWin);
	
	curParam->ren1->AddActor(curParam->partActor);
	curParam->ren1->SetBackground(0.1, .2, .4);
	
	curParam->renWin->SetSize(300, 300);
	curParam->renWin->SetDesiredUpdateRate(5.0);

	curParam->iren->Initialize();

	curParam->ren1->ResetCamera();
	curParam->camera = curParam->ren1->GetActiveCamera(); //vtkCamera::New();
// 	curParam->ren1->SetActiveCamera(curParam->camera);
// 	curParam->camera->SetClippingRange(0.0475572, 23778.6);
// 	curParam->camera->SetFocalPoint(0.052665, -0.129454, -0.0573973);
// 	curParam->camera->SetPosition(0.327637, -0.116299, -0.256418);
// 	curParam->camera->ComputeViewPlaneNormal();
// 	curParam->camera->SetViewUp(-0.0225386, 0.999137, 0.034901);

// 	curParam->camera->Azimuth(150);
// 	curParam->camera->Elevation(60);
	curParam->camera->Zoom(1.8);
	curParam->renWin->Render();


	curParam->iren->Start();

}
void EndFilterData43()
{
	if (curParam) {
		delete curParam;
		curParam = nullptr;
	}
}