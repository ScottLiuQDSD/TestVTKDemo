#include "stdafx.h"
#include "DrawingClass4_12.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkDataObject.h"
#include "vtkLineSource.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkProbeFilter.h"
#include "vtkAppendPolyData.h"
#include "vtkTubeFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkXYPlotActor.h"
#include "vtkProperty2D.h"
#include "vtkTextProperty.h"
#include "vtkStructuredGridOutlineFilter.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkBYUReader.h"
#include "vtkPolyDataNormals.h"
#include "vtkLODActor.h"
#include "vtkOutlineFilter.h"
#include "vtkLight.h"
#include "vtkCubeAxesActor2D.h"

void XYPlotActor()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	vtkMultiBlockPLOT3DReader *pl3d = vtkMultiBlockPLOT3DReader::New();
	pl3d->SetXYZFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/combxyz.bin");
	pl3d->SetQFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/combq.bin");
	pl3d->SetScalarFunctionNumber(100);
	pl3d->SetVectorFunctionNumber(202);
	pl3d->Update();

	auto pl3dOutput = pl3d->GetOutput()->GetBlock(0);
	vtkLineSource *line = vtkLineSource::New();
	line->SetResolution(40);

	vtkTransform *transL1 = vtkTransform::New();
	transL1->Translate(3.7, 0.0, 28.37);
	transL1->Scale(5, 5, 5);
	transL1->RotateY(90);

	vtkTransformPolyDataFilter *tf = vtkTransformPolyDataFilter::New();
	tf->SetInputConnection(line->GetOutputPort());
	tf->SetTransform(transL1);

	vtkProbeFilter *probe = vtkProbeFilter::New();
	probe->SetInputConnection(tf->GetOutputPort());
	probe->SetSourceData(pl3dOutput);

	vtkTransform *transL2 = vtkTransform::New();
	transL2->Translate(9.2, 0.0, 31.2);
	transL2->Scale(5, 5, 5);
	transL2->RotateY(90);

	vtkTransformPolyDataFilter *tf2 = vtkTransformPolyDataFilter::New();
	tf2->SetInputConnection(line->GetOutputPort());
	tf2->SetTransform(transL2);

	vtkProbeFilter *probe2 = vtkProbeFilter::New();
	probe2->SetInputConnection(tf2->GetOutputPort());
	probe2->SetSourceData(pl3dOutput);

	vtkTransform *transL3 = vtkTransform::New();
	transL3->Translate(13.27, 0.0, 33.40);
	transL3->Scale(5, 5, 5);
	transL3->RotateY(90);

	vtkTransformPolyDataFilter *tf3 = vtkTransformPolyDataFilter::New();
	tf3->SetInputConnection(line->GetOutputPort());
	tf3->SetTransform(transL3);

	vtkProbeFilter *probe3 = vtkProbeFilter::New();
	probe3->SetInputConnection(tf3->GetOutputPort());
	probe3->SetSourceData(pl3dOutput);

	vtkAppendPolyData *appendF = vtkAppendPolyData::New();
	appendF->AddInputConnection(probe->GetOutputPort());
	appendF->AddInputConnection(probe2->GetOutputPort());
	appendF->AddInputConnection(probe3->GetOutputPort());

	vtkTubeFilter *tuber = vtkTubeFilter::New();
	tuber->SetInputConnection(appendF->GetOutputPort());
	tuber->SetRadius(0.1);
	vtkPolyDataMapper *lineMapper = vtkPolyDataMapper::New();
	lineMapper->SetInputConnection(tuber->GetOutputPort());
	vtkActor *lineActor = vtkActor::New();
	lineActor->SetMapper(lineMapper);

	//Create an xy - plot using the output of the 3 probe filters as input.
	//The x - values we are plotting are arc length.
	vtkXYPlotActor *xyplot = vtkXYPlotActor::New();
	xyplot->AddDataSetInputConnection(probe->GetOutputPort());
	xyplot->AddDataSetInputConnection(probe2->GetOutputPort());
	xyplot->AddDataSetInputConnection(probe3->GetOutputPort());
	xyplot->GetPositionCoordinate()->SetValue(0.0, 0.67, 0);
	xyplot->GetPosition2Coordinate()->SetValue(1.0, 0.33, 0); //#relative to Position
	xyplot->SetXValuesToArcLength();
	xyplot->SetNumberOfXLabels(6);
	xyplot->SetTitle("Pressure vs. Arc Length (Zoomed View)");
	xyplot->SetXTitle("");
	xyplot->SetYTitle("P");
	xyplot->SetXRange(.1, .35);
	xyplot->SetYRange(.2, .4);
	xyplot->GetProperty()->SetColor(0, 0, 0);
	xyplot->GetProperty()->SetLineWidth(2);
	//Set text prop color(same color for backward compat with test)
	//Assign same object to all text props
	{
		auto tprop = xyplot->GetTitleTextProperty();
		tprop->SetColor(xyplot->GetProperty()->GetColor());
		xyplot->SetAxisTitleTextProperty(tprop);
		xyplot->SetAxisLabelTextProperty(tprop);
	}

	//Create an xy - plot using the output of the 3 probe filters as input.
	//The x - values we are plotting are normalized arc length.
	vtkXYPlotActor *xyplot2 = vtkXYPlotActor::New();
	xyplot2->AddDataSetInputConnection(probe->GetOutputPort());
	xyplot2->AddDataSetInputConnection(probe2->GetOutputPort());
	xyplot2->AddDataSetInputConnection(probe3->GetOutputPort());
	xyplot2->GetPositionCoordinate()->SetValue(0.0, 0.33, 0);
	xyplot2->GetPosition2Coordinate()->SetValue(1.0, 0.33, 0); //#relative to Position
	xyplot2->SetXValuesToArcLength();
	xyplot2->SetNumberOfXLabels(6);
	xyplot2->SetTitle("Pressure vs. Normalized Arc Length");
	xyplot2->SetXTitle("");
	xyplot2->SetYTitle("P");
	xyplot2->PlotPointsOn();
	xyplot2->PlotLinesOff();
	xyplot2->GetProperty()->SetColor(1, 0, 0);
	xyplot2->GetProperty()->SetLineWidth(2);
	//Set text prop color(same color for backward compat with test)
	//Assign same object to all text props
	{
		auto tprop = xyplot2->GetTitleTextProperty();
		tprop->SetColor(xyplot2->GetProperty()->GetColor());
		xyplot2->SetAxisTitleTextProperty(tprop);
		xyplot2->SetAxisLabelTextProperty(tprop);
	}

	//Create an xy - plot using the output of the 3 probe filters as input.
	//The x - values we are plotting are the underlying point data values.
	vtkXYPlotActor *xyplot3 = vtkXYPlotActor::New();
	xyplot3->AddDataSetInputConnection(probe->GetOutputPort());
	xyplot3->AddDataSetInputConnection(probe2->GetOutputPort());
	xyplot3->AddDataSetInputConnection(probe3->GetOutputPort());
	xyplot3->GetPositionCoordinate()->SetValue(0.0, 0.0, 0);
	xyplot3->GetPosition2Coordinate()->SetValue(1.0, 0.33, 0); //#relative to Position
	xyplot3->SetXValuesToArcLength();
	xyplot3->SetNumberOfXLabels(6);
	xyplot3->SetTitle("Pressure vs. Point Id");
	xyplot3->SetXTitle("Probe Length");
	xyplot3->SetYTitle("P");
	xyplot3->PlotPointsOn();
	xyplot3->PlotLinesOff();
	xyplot3->GetProperty()->SetColor(0, 0, 1);
	xyplot3->GetProperty()->SetLineWidth(3);
	//Set text prop color(same color for backward compat with test)
	//Assign same object to all text props
	{
		auto tprop = xyplot3->GetTitleTextProperty();
		tprop->SetColor(xyplot3->GetProperty()->GetColor());
		xyplot3->SetAxisTitleTextProperty(tprop);
		xyplot3->SetAxisLabelTextProperty(tprop);
	}

	vtkStructuredGridOutlineFilter *outline = vtkStructuredGridOutlineFilter::New();
	outline->SetInputData(pl3dOutput);
	vtkPolyDataMapper *outlineMapper = vtkPolyDataMapper::New();
	outlineMapper->SetInputConnection(outline->GetOutputPort());
	vtkActor *outlineActor = vtkActor::New();
	outlineActor->SetMapper(outlineMapper);
	outlineActor->GetProperty()->SetColor(0, 0, 0);

	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderer *ren2 = vtkRenderer::New();
	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->AddRenderer(ren2);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	ren1->SetBackground(0.6784, 0.8471, 0.9020);
	ren1->SetViewport(0, 0, .5, 1);
	ren1->AddActor(outlineActor);
	ren1->AddActor(lineActor);

	ren2->SetBackground(1, 1, 1);
	ren2->SetViewport(0.5, 0, 1, 1);
	ren2->AddActor2D(xyplot);
	ren2->AddActor2D(xyplot2);
	ren2->AddActor2D(xyplot3);
	renWin->SetSize(500, 250);

	auto cam1 = ren1->GetActiveCamera();
	cam1->SetClippingRange(3.95297, 100);
	cam1->SetFocalPoint(8.88908, 0.595038, 29.3342);
	cam1->SetPosition(-12.3332, 31.7479, 41.2387);
	cam1->SetViewUp(0.060772, - 0.319905, 0.945498);
	iren->Initialize();

	iren->Start();
}
void CubeAxes()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);


	vtkBYUReader *fohe = vtkBYUReader::New();
	fohe->SetGeometryFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/teapot.g");

	/// we give the model four different transform matrix but same camera
	/// so the model will act synchronous, and show different direction.
	vtkPolyDataNormals *normals = vtkPolyDataNormals::New();
	normals->SetInputConnection(fohe->GetOutputPort());
	vtkPolyDataMapper *foheMapper = vtkPolyDataMapper::New();
	foheMapper->SetInputConnection(normals->GetOutputPort());

	vtkLODActor *foheActor = vtkLODActor::New();
	foheActor->SetMapper(foheMapper);

	vtkTransform *trans2 = vtkTransform::New();
	trans2->RotateZ(-90);
	vtkTransformPolyDataFilter * transFilter2 = vtkTransformPolyDataFilter::New();
	transFilter2->SetInputConnection(normals->GetOutputPort());
	transFilter2->SetTransform(trans2);

	vtkPolyDataMapper *foheMapper2 = vtkPolyDataMapper::New();
	foheMapper2->SetInputConnection(transFilter2->GetOutputPort());
	
	vtkLODActor *foheActor2 = vtkLODActor::New();
	foheActor2->SetMapper(foheMapper2);

	vtkTransform *trans3 = vtkTransform::New();
	trans3->RotateZ(90);
	vtkTransformPolyDataFilter * transFilter3 = vtkTransformPolyDataFilter::New();
	transFilter3->SetInputConnection(normals->GetOutputPort());
	transFilter3->SetTransform(trans3);

	vtkPolyDataMapper *foheMapper3 = vtkPolyDataMapper::New();
	foheMapper3->SetInputConnection(transFilter3->GetOutputPort());

	vtkLODActor *foheActor3 = vtkLODActor::New();
	foheActor3->SetMapper(foheMapper3);

	vtkTransform *trans4 = vtkTransform::New();
	trans4->RotateX(90);
	vtkTransformPolyDataFilter * transFilter4 = vtkTransformPolyDataFilter::New();
	transFilter4->SetInputConnection(normals->GetOutputPort());
	transFilter4->SetTransform(trans4);

	vtkPolyDataMapper *foheMapper4 = vtkPolyDataMapper::New();
	foheMapper4->SetInputConnection(transFilter4->GetOutputPort());

	vtkLODActor *foheActor4 = vtkLODActor::New();
	foheActor4->SetMapper(foheMapper4);


	vtkOutlineFilter *outline = vtkOutlineFilter::New();
	outline->SetInputConnection(normals->GetOutputPort());
	vtkPolyDataMapper *mapOutline = vtkPolyDataMapper::New();
	mapOutline->SetInputConnection(outline->GetOutputPort());
	vtkActor *outlineActor = vtkActor::New();
	outlineActor->SetMapper(mapOutline);
	outlineActor->GetProperty()->SetColor(0, 0, 0);

	vtkCamera *camera = vtkCamera::New();
	camera->SetClippingRange(1.60187, 20.0842);
	camera->SetFocalPoint(0.21406, 1.5, 0);
	camera->SetPosition(8.3761, 4.94858, 4.12505);
	camera->SetViewUp(0.180325, 0.549245, -0.815974);

//	// if we set four different camera then the view will roll separatelly
// 	vtkCamera *camera1 = vtkCamera::New();
// 	camera1->SetClippingRange(1.60187, 20.0842);
// 	camera1->SetFocalPoint(0.21406, 1.5, 0);
// 	camera1->SetPosition(8.3761, 4.94858, 4.12505);
// 	camera1->SetViewUp(0.180325, 0.549245, -0.815974);
// 	vtkCamera *camera2 = vtkCamera::New();
// 	camera2->SetClippingRange(1.60187, 20.0842);
// 	camera2->SetFocalPoint(0.21406, 1.5, 0);
// 	camera2->SetPosition(8.3761, 4.94858, 4.12505);
// 	camera2->SetViewUp(0.180325, 0.549245, -0.815974);
// 	camera2->Roll(-90);
// 	vtkCamera *camera3 = vtkCamera::New();
// 	camera3->SetClippingRange(1.60187, 20.0842);
// 	camera3->SetFocalPoint(0.21406, 1.5, 0);
// 	camera3->SetPosition(8.3761, 4.94858, 4.12505);
// 	camera3->SetViewUp(0.180325, 0.549245, -0.815974);
// 	camera3->Roll(90);
// 	vtkCamera *camera4 = vtkCamera::New();
// 	camera4->SetClippingRange(1.60187, 20.0842);
// 	camera4->SetFocalPoint(0.21406, 1.5, 0);
// 	camera4->SetPosition(8.3761, 4.94858, 4.12505);
// 	camera4->SetViewUp(0.180325, 0.549245, -0.815974);
// 	camera4->Elevation(90);

	vtkLight *light = vtkLight::New();
	light->SetFocalPoint(0.21406, 1.5, 0);
	light->SetPosition(8.3761, 4.94858, 4.12505);

	vtkRenderer *ren1 = vtkRenderer::New();
	ren1->SetViewport(0, 0, 0.5, 0.5);
	ren1->SetActiveCamera(camera);
	ren1->AddLight(light);
	vtkRenderer *ren2 = vtkRenderer::New();
	ren2->SetViewport(0.5, 0, 1.0, 0.5);
	ren2->SetActiveCamera(camera);
	ren2->AddLight(light);
	vtkRenderer *ren3 = vtkRenderer::New();
	ren3->SetViewport(0, 0.5, 0.5, 1.0);
	ren3->SetActiveCamera(camera);
	ren3->AddLight(light);
	vtkRenderer *ren4 = vtkRenderer::New();
	ren4->SetViewport(0.5, 0.5, 1.0, 1.0);
	ren4->SetActiveCamera(camera);
	ren4->AddLight(light);

	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->AddRenderer(ren2);
	renWin->AddRenderer(ren3);
	renWin->AddRenderer(ren4);
	renWin->SetWindowName("VTK - Cube Axes");
	renWin->SetSize(800, 800);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);


	ren1->AddViewProp(foheActor);
	ren1->AddViewProp(outlineActor);
	ren2->AddViewProp(foheActor2);
	ren2->AddViewProp(outlineActor);
	ren3->AddViewProp(foheActor3);
	ren3->AddViewProp(outlineActor);
	ren4->AddViewProp(foheActor4);
	ren4->AddViewProp(outlineActor);
	ren1->SetBackground(0.1, 0.2, 0.4);
	ren2->SetBackground(0.1, 0.2, 0.4);
	ren3->SetBackground(0.1, 0.2, 0.4);
	ren4->SetBackground(0.1, 0.2, 0.4);

	vtkTextProperty *tprop = vtkTextProperty::New();
	tprop->SetColor(1, 1, 1);
	tprop->ShadowOn();

	vtkCubeAxesActor2D *axes = vtkCubeAxesActor2D::New();
	axes->SetInputConnection(normals->GetOutputPort());
	axes->SetCamera(ren1->GetActiveCamera());
	axes->SetLabelFormat("%6.4g");
	axes->SetFlyModeToClosestTriad();
	axes->SetFontFactor(0.8);
	axes->SetAxisTitleTextProperty(tprop);
	axes->SetAxisLabelTextProperty(tprop);
	ren1->AddViewProp(axes);

	vtkCubeAxesActor2D *axes2 = vtkCubeAxesActor2D::New();
	axes2->SetViewProp(foheActor);
	axes2->SetCamera(ren2->GetActiveCamera());
	axes2->SetLabelFormat("%6.4g");
	axes2->SetFlyModeToClosestTriad();
	axes2->SetFontFactor(0.8);
	axes2->ScalingOff();
	axes2->SetAxisTitleTextProperty(tprop);
	axes2->SetAxisLabelTextProperty(tprop);
	ren2->AddViewProp(axes2);

	vtkCubeAxesActor2D *axes3 = vtkCubeAxesActor2D::New();
	axes3->SetViewProp(foheActor);
	axes3->SetCamera(ren3->GetActiveCamera());
	axes3->SetLabelFormat("%6.4g");
	axes3->SetFlyModeToClosestTriad();
	axes3->SetFontFactor(0.8);
	axes3->ScalingOff();
	axes3->SetAxisTitleTextProperty(tprop);
	axes3->SetAxisLabelTextProperty(tprop);
	ren3->AddViewProp(axes3);

	vtkCubeAxesActor2D *axes4 = vtkCubeAxesActor2D::New();
	axes4->SetViewProp(foheActor);
	axes4->SetCamera(ren4->GetActiveCamera());
	axes4->SetLabelFormat("%6.4g");
	axes4->SetFlyModeToClosestTriad();
	axes4->SetFontFactor(0.8);
	axes4->ScalingOff();
	axes4->SetAxisTitleTextProperty(tprop);
	axes4->SetAxisLabelTextProperty(tprop);
	ren4->AddViewProp(axes4);
	
	renWin->Render();

	iren->Initialize();
	iren->Start();




}
