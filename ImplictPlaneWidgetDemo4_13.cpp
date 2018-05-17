#include "stdafx.h"
#include "ImplictPlaneWidgetDemo4_13.h"

#include "vtkAutoInit.h"
#include "vtkSphereSource.h"
#include "vtkConeSource.h"
#include "vtkGlyph3D.h"
#include "vtkAppendPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkLODActor.h"
#include "vtkPlane.h"
#include "vtkClipPolyData.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImplicitPlaneWidget.h"

vtkLODActor *selectActor = nullptr;
vtkPlane *plane = nullptr;

class BuildVTKWidgetCall : public vtkCommand {
public:

	static BuildVTKWidgetCall *New()
	{
		return new BuildVTKWidgetCall;
	}
public:
	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData)
	{
		vtkImplicitPlaneWidget *pWidget = reinterpret_cast<vtkImplicitPlaneWidget*>(caller);
		if (pWidget) {
			vtkSmartPointer<vtkPlane> planeNew = vtkPlane::New();
			pWidget->GetPlane(planeNew);
			actor->VisibilityOn();
		}
	}
	void setPlane(vtkSmartPointer<vtkPlane> other) { pPlane = other; }
	void setActor(vtkSmartPointer<vtkActor> other) { actor = other; }
private:
	vtkSmartPointer<vtkPlane> pPlane;
	vtkSmartPointer<vtkActor> actor;
};
void ImplictPlaneWidgetDemo4_15()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	auto sphere = vtkSphereSource::New();
	auto cone = vtkConeSource::New();
	auto glyph = vtkGlyph3D::New();
	glyph->SetInputConnection(sphere->GetOutputPort());
	glyph->SetSourceConnection(cone->GetOutputPort());
	glyph->SetVectorModeToUseNormal();
	glyph->SetScaleModeToScaleByVector();
	glyph->SetScaleFactor(0.25);

	auto apd = vtkAppendPolyData::New();
	apd->AddInputConnection(glyph->GetOutputPort());
	apd->AddInputConnection(sphere->GetOutputPort());

	auto maceMapper = vtkPolyDataMapper::New();
	maceMapper->SetInputConnection(apd->GetOutputPort());

	auto maceActor = vtkLODActor::New();
	maceActor->SetMapper(maceMapper);
	maceActor->VisibilityOn();

	plane = vtkPlane::New();
	auto clipper = vtkClipPolyData::New();
	clipper->SetInputConnection(apd->GetOutputPort());
	clipper->SetClipFunction(plane);
	clipper->InsideOutOn();

	auto selectMapper = vtkPolyDataMapper::New();
	selectMapper->SetInputConnection(clipper->GetOutputPort());

	selectActor = vtkLODActor::New();
	selectActor->SetMapper(selectMapper);
	selectActor->GetProperty()->SetColor(0, 1, 0);
	selectActor->VisibilityOff();
	selectActor->SetScale(1.01, 1.01, 1.01);

	auto ren = vtkRenderer::New();
	auto renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren);
	auto iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	auto planeWidget = vtkImplicitPlaneWidget::New();
	planeWidget->SetInteractor(iren);
	planeWidget->SetPlaceFactor(1.25);
	planeWidget->SetInputConnection(glyph->GetOutputPort());
	planeWidget->PlaceWidget();

	vtkSmartPointer<BuildVTKWidgetCall> pCall = BuildVTKWidgetCall::New();
	pCall->setActor(selectActor);
	planeWidget->AddObserver(vtkCommand::InteractionEvent, pCall);

	ren->AddActor(maceActor);
	ren->AddActor(selectActor);

	ren->SetBackground(1, 1, 1);
	renWin->SetSize(300, 300);
	ren->SetBackground(0.1, 0.2, 0.4);

	iren->Initialize();
	renWin->Render();

	iren->Start();




}