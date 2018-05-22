#include "stdafx.h"
#include "Contours2D6_1.h"
#include "vtkQuadric.h"
#include "vtkSampleFunction.h"
#include "vtkExtractVOI.h"
#include "vtkContourFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkOutlineFilter.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"

void Contours2D6_1()
{
	// Quadric definition.This is a type of implicit function.Here the
	// coefficients to the equations are set.
	VTK_SP(vtkQuadric, quadric);
	quadric->SetCoefficients(.5, 1, .2, 0, .1, 0, 0, .2, 0, 0);

		// The vtkSampleFunction uses the quadric function and evaluates function
		// value over a regular lattice(i.e., a volume).
	VTK_SP(vtkSampleFunction, sample);
	sample->SetSampleDimensions(30, 30, 30);
	sample->SetImplicitFunction(quadric);
	sample->ComputeNormalsOff();
	sample->Update();

		// Here a single slice(i.e., image) is extracted from the volume. (Note: in
		// actuality the VOI request causes the sample function to operate on just the
		// slice.)
	VTK_SP(vtkExtractVOI, extract);
	extract->SetInputConnection(sample->GetOutputPort());
	extract->SetVOI(0, 29, 0, 29, 15, 15);
	extract->SetSampleRate(1, 2, 3);

		// The image is contoured to produce contour lines.Thirteen contour values
		// ranging from(0, 1.2) inclusive are produced.
	VTK_SP(vtkContourFilter, contours);
	contours->SetInputConnection(extract->GetOutputPort());
	contours->GenerateValues(13, 0.0, 1.2);

		// The contour lines are mapped to the graphics library.
	VTK_SP(vtkPolyDataMapper, contMapper);
	contMapper->SetInputConnection(contours->GetOutputPort());
	contMapper->SetScalarRange(0.0, 1.2);

	VTK_SP(vtkActor, contActor);
	contActor->SetMapper(contMapper);

		// Create outline an outline of the sampled data.
	VTK_SP(vtkOutlineFilter, outline);
	outline->SetInputConnection(sample->GetOutputPort());

	VTK_SP(vtkPolyDataMapper, outlineMapper);
	outlineMapper->SetInputConnection(outline->GetOutputPort());

	VTK_SP(vtkActor, outlineActor);
	outlineActor->SetMapper(outlineMapper);
	outlineActor->GetProperty()->SetColor(0, 0, 0);

	VTK_SP(vtkRenderer, renderer);
	VTK_SP(vtkRenderWindow, renWin);
	renWin->AddRenderer(renderer);

	VTK_SP(vtkRenderWindowInteractor, iren);
	iren->SetRenderWindow(renWin);

	renderer->AddActor(contActor);
	renderer->AddActor(outlineActor);

	renderer->SetBackground(1, 1, 1);
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Zoom(1.5);


	renWin->SetSize(300, 300);
	// interact with data
	renWin->Render();
	iren->Start();



}




