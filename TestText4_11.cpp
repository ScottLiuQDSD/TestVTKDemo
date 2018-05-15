#include "stdafx.h"
#include "TestText4_11.h"

#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkLODActor.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkTextMapper.h"
#include "vtkActor2D.h"
#include "vtkCellArray.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkProperty2D.h"
#include "vtkAxes.h"
#include "vtkVectorText.h"
#include "vtkFollower.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkScalarBarActor.h"

void TextAnnotationMultiLine4_11()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	const int font_size = 14;

	vtkTextProperty *singleLineTextProp = vtkTextProperty::New();
	singleLineTextProp->SetFontSize(font_size);
	singleLineTextProp->SetFontFamilyToArial();
	singleLineTextProp->BoldOff();
	singleLineTextProp->ItalicOff();
	singleLineTextProp->ShadowOff();

	vtkTextProperty *multiLineTextProp = vtkTextProperty::New();
	multiLineTextProp->ShallowCopy(singleLineTextProp);
	multiLineTextProp->BoldOn();
	multiLineTextProp->ItalicOn();
	multiLineTextProp->ShadowOn();
	multiLineTextProp->SetLineSpacing(0.8);

	vtkTextMapper *singleLineTextB = vtkTextMapper::New();
	singleLineTextB->SetInput("Single Line(Bottom)");
	{
		auto tprop = singleLineTextB->GetTextProperty();
		tprop->ShallowCopy(singleLineTextProp);
		tprop->SetVerticalJustificationToBottom();
		tprop->SetColor(1, 0, 0);
	}
	vtkActor2D *singleLineTextActorB = vtkActor2D::New();
	singleLineTextActorB->SetMapper(singleLineTextB);
	singleLineTextActorB->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	singleLineTextActorB->GetPositionCoordinate()->SetValue(0.05, 0.85);

	vtkTextMapper *singleLineTextC = vtkTextMapper::New();
	singleLineTextC->SetInput("Single Line(centered)");
	{
		auto tprop = singleLineTextC->GetTextProperty();
		tprop->ShallowCopy(singleLineTextProp);
		tprop->SetVerticalJustificationToCentered();
		tprop->SetColor(0, 1, 0);
	}
	vtkActor2D *singleLineTextActorC = vtkActor2D::New();
	singleLineTextActorC->SetMapper(singleLineTextC);
	singleLineTextActorC->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	singleLineTextActorC->GetPositionCoordinate()->SetValue(0.05, 0.75);

	vtkTextMapper *singleLineTextT = vtkTextMapper::New();
	singleLineTextT->SetInput("Single Line(top)");
	{
		auto tprop = singleLineTextT->GetTextProperty();
		tprop->ShallowCopy(singleLineTextProp);
		tprop->SetVerticalJustificationToTop();
		tprop->SetColor(0, 1, 0);
	}
	vtkActor2D *singleLineTextActorT = vtkActor2D::New();
	singleLineTextActorT->SetMapper(singleLineTextT);
	singleLineTextActorT->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	singleLineTextActorT->GetPositionCoordinate()->SetValue(0.05, 0.65);

	vtkTextMapper *textMapperL = vtkTextMapper::New();
	textMapperL->SetInput("This is\nmulti-line\ntext output\n(left-top)");
	{
		auto tprop = textMapperL->GetTextProperty();
		tprop->ShallowCopy(multiLineTextProp);
		tprop->SetJustificationToLeft();
		tprop->SetVerticalJustificationToTop();
		tprop->SetColor(1, 0, 0);
	}
	vtkActor2D *textActorL = vtkActor2D::New();
	textActorL->SetMapper(textMapperL);
	textActorL->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	textActorL->GetPositionCoordinate()->SetValue(0.05, 0.5);

	vtkTextMapper *textMapperC = vtkTextMapper::New();
	textMapperC->SetInput("This is\nmulti-line\ntext output\n(centered)");
	{
		auto tprop = textMapperC->GetTextProperty();
		tprop->ShallowCopy(multiLineTextProp);
		tprop->SetJustificationToLeft();
		tprop->SetVerticalJustificationToCentered();
		tprop->SetColor(0, 1, 0);
	}
	vtkActor2D *textActorC = vtkActor2D::New();
	textActorC->SetMapper(textMapperC);
	textActorC->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	textActorC->GetPositionCoordinate()->SetValue(0.5, 0.5);

	vtkTextMapper *textMapperR = vtkTextMapper::New();
	textMapperR->SetInput("This is\nmulti-line\ntext output\n(right-bottom)");
	{
		auto tprop = textMapperR->GetTextProperty();
		tprop->ShallowCopy(multiLineTextProp);
		tprop->SetJustificationToRight();
		tprop->SetVerticalJustificationToBottom();
		tprop->SetColor(0, 0, 1);
	}
	vtkActor2D *textActorR = vtkActor2D::New();
	textActorR->SetMapper(textMapperR);
	textActorR->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	textActorR->GetPositionCoordinate()->SetValue(0.95, 0.5);

	vtkPoints *pts = vtkPoints::New();
	pts->InsertNextPoint(0.05, 0.0, 0.0);
	pts->InsertNextPoint(0.05, 1.0, 0.0);
	pts->InsertNextPoint(0.5, 0.0, 0.0);
	pts->InsertNextPoint(0.5, 1.0, 0.0);
	pts->InsertNextPoint(0.95, 0.0, 0.0);
	pts->InsertNextPoint(0.95, 1.0, 0.0);
	pts->InsertNextPoint(0.0, 0.5, 0.0);
	pts->InsertNextPoint(1.0, 0.5, 0.0);
	pts->InsertNextPoint(0.00, 0.85, 0.0);
	pts->InsertNextPoint(0.50, 0.85, 0.0);
	pts->InsertNextPoint(0.00, 0.75, 0.0);
	pts->InsertNextPoint(0.50, 0.75, 0.0);
	pts->InsertNextPoint(0.00, 0.65, 0.0);
	pts->InsertNextPoint(0.50, 0.65, 0.0);

	vtkCellArray *Lines = vtkCellArray::New();
	Lines->InsertNextCell(2);
	Lines->InsertCellPoint(0);
	Lines->InsertCellPoint(1);
	Lines->InsertNextCell(2);
	Lines->InsertCellPoint(2);
	Lines->InsertCellPoint(3);
	Lines->InsertNextCell(2);
	Lines->InsertCellPoint(4);
	Lines->InsertCellPoint(5);
	Lines->InsertNextCell(2);
	Lines->InsertCellPoint(6);
	Lines->InsertCellPoint(7);
	Lines->InsertNextCell(2);
	Lines->InsertCellPoint(8);
	Lines->InsertCellPoint(9);
	Lines->InsertNextCell(2);
	Lines->InsertCellPoint(10);
	Lines->InsertCellPoint(11);
	Lines->InsertNextCell(2);
	Lines->InsertCellPoint(12);
	Lines->InsertCellPoint(13);

	vtkPolyData *grid = vtkPolyData::New();
	grid->SetPoints(pts);
	grid->SetLines(Lines);

	vtkCoordinate *normCoords = vtkCoordinate::New();
	normCoords->SetCoordinateSystemToNormalizedViewport();

	vtkPolyDataMapper2D *mapper = vtkPolyDataMapper2D::New();
	mapper->SetInputData(grid);
	mapper->SetTransformCoordinate(normCoords);
	vtkActor2D *gridActor = vtkActor2D::New();
	gridActor->SetMapper(mapper);
	gridActor->GetProperty()->SetColor(0.1, .1, .1);

	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	ren1->AddActor2D(textActorL);
	ren1->AddActor2D(textActorC);
	ren1->AddActor2D(textActorR);
	ren1->AddActor2D(singleLineTextActorB);
	ren1->AddActor2D(singleLineTextActorC);
	ren1->AddActor2D(singleLineTextActorT);
	ren1->AddActor2D(gridActor);


	ren1->SetBackground(1, 1, 1);
	renWin->SetSize(500, 300);
	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.5);
	renWin->Render();
	iren->Start();
}

void TextAnnotation3D4_11()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	vtkAxes *axes = vtkAxes::New();
	axes->SetOrigin(0, 0, 0);
	vtkPolyDataMapper *axesMapper = vtkPolyDataMapper::New();
	axesMapper->SetInputConnection(axes->GetOutputPort());
	vtkActor *axesActor = vtkActor::New();
	axesActor->SetMapper(axesMapper);

	vtkVectorText *atext = vtkVectorText::New();
	atext->SetText("Origin");
	vtkPolyDataMapper *textMapper = vtkPolyDataMapper::New();
	textMapper->SetInputConnection(atext->GetOutputPort());
	vtkFollower *textActor = vtkFollower::New();
	textActor->SetMapper(textMapper);
	textActor->SetScale(0.2, 0.2, 0.2);
	textActor->AddPosition(0, -0.1, 0);

	// 4.12 
	vtkScalarBarActor* scalarBar = vtkScalarBarActor::New();
	scalarBar->SetLookupTable(textMapper->GetLookupTable());
	scalarBar->SetTitle("Temperature");
	scalarBar->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
	scalarBar->GetPositionCoordinate()->SetValue(0.1, 0.1);
	scalarBar->SetOrientationToHorizontal();
	scalarBar->SetWidth(0.8);
	scalarBar->SetHeight(0.17);

	vtkRenderer *ren1 = vtkRenderer::New();
	ren1->SetBackground(0.5, 0.5, 0.5);
	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	ren1->AddActor(axesActor);
	ren1->AddActor(textActor);
	ren1->AddActor(scalarBar);

	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.6);

	ren1->ResetCameraClippingRange();
	textActor->SetCamera(ren1->GetActiveCamera());
	renWin->Render();
	iren->Start();


}