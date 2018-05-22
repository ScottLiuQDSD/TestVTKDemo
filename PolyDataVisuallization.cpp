#include "stdafx.h"
#include "PolyDataVisuallization.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDataSet.h"
#include "vtkPointData.h"

#include "vtkPolyDataReader.h"
#include "vtkDecimatePro.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"

#include "vtkSTLReader.h"
#include "vtkSTLWriter.h"

#include "vtkSmoothPolyDataFilter.h"

#include "vtkBYUReader.h"
#include "vtkPlane.h"
#include "vtkClipPolyData.h"
#include "vtkCutter.h"
#include "vtkStripper.h"
#include "vtkTriangleFilter.h"

void CreateCube5_2()
{
	int i;
	static double x[8][3] = { { 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, 0 }, { 0, 1, 0 },
	{ 0, 0, 1 }, { 1, 0, 1 }, { 1, 1, 1 }, { 0, 1, 1 } };
	static vtkIdType pts[6][4] = { { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 0, 1, 5, 4 },
	{ 1, 2, 6, 5 }, { 2, 3, 7, 6 }, { 3, 0, 4, 7 } };
	// Create the building blocks of polydata including data attributes.
	vtkPolyData *cube = vtkPolyData::New();
	vtkPoints *points = vtkPoints::New();
	vtkCellArray *polys = vtkCellArray::New();
	vtkFloatArray *scalars = vtkFloatArray::New();
	for (i = 0; i < 8; i++) {
		points->InsertPoint(i, x[i]);
	}
	for (i = 0; i < 6; i++) {
		polys->InsertNextCell(4, pts[i]);
	}
	for (i = 0; i < 8; i++) {
		scalars->InsertTuple1(i, i);
	}
	// We now assign the pieces to the vtkPolyData.
	cube->SetPoints(points);
	points->Delete();
	cube->SetPolys(polys);
	polys->Delete();
	cube->GetPointData()->SetScalars(scalars);
	scalars->Delete();

	vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper::New();
	cubeMapper->SetInputData(cube);
	cubeMapper->SetScalarRange(0, 7);
	vtkActor *cubeActor = vtkActor::New();
	cubeActor->SetMapper(cubeMapper);

	// The usual rendering stuff.
	vtkCamera *camera = vtkCamera::New();
	camera->SetPosition(1, 1, 1);
	camera->SetFocalPoint(0, 0, 0);

	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(cubeActor);
	renderer->SetActiveCamera(camera);
	renderer->ResetCamera();
	renderer->SetBackground(1, 1, 1);

	renWin->SetSize(300, 300);

	// interact with data
	renWin->Render();
	iren->Start();

	// Clean up

	cube->Delete();
	cubeMapper->Delete();
	cubeActor->Delete();
	camera->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
}
void DecimationVtk5_2()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	VTK_SP(vtkPolyDataReader, fran);
	fran->SetFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/fran_cut.vtk");
	
	VTK_SP(vtkDecimatePro, deci);
	deci->SetInputConnection(fran->GetOutputPort());
	deci->SetTargetReduction(0.9);
	deci->PreserveTopologyOn();

	VTK_SP(vtkPolyDataNormals, normals);
	normals->SetInputConnection(fran->GetOutputPort());
	normals->FlipNormalsOn();

	VTK_SP(vtkPolyDataMapper, franMapper);
	franMapper->SetInputConnection(normals->GetOutputPort());

	VTK_SP(vtkActor, franActor);
	franActor->SetMapper(franMapper);
	franActor->GetProperty()->SetColor(1.0, 0.49, 0.25);

	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(franActor);
	renderer->SetBackground(1, 1, 1);
	renWin->SetSize(300, 300);
	VTK_SP(vtkCamera, cam1);
	cam1->SetClippingRange(0.0475572, 2.37786);
	cam1->SetFocalPoint(0.052665, -0.129454, -0.0573973);
	cam1->SetPosition(0.327637, -0.116299, -0.256418);
	cam1->SetViewUp(-0.0225386, 0.999137, 0.034901);
	renderer->SetActiveCamera(cam1);

	// interact with data
	renWin->Render();
	iren->Start();


}
void DecimationStl5_2()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	VTK_SP(vtkSTLReader, fran);
	fran->SetFileName("F:/bin/export/370234001_0_L.stl");

	VTK_SP(vtkDecimatePro, deci);
	deci->SetInputConnection(fran->GetOutputPort());
	deci->SetTargetReduction(0.9);
	deci->PreserveTopologyOn();

	VTK_SP(vtkPolyDataNormals, normals);
	normals->SetInputConnection(fran->GetOutputPort());
	normals->FlipNormalsOn();

	VTK_SP(vtkPolyDataMapper, franMapper);
	franMapper->SetInputConnection(normals->GetOutputPort());

	VTK_SP(vtkActor, franActor);
	franActor->SetMapper(franMapper);
	franActor->GetProperty()->SetColor(1.0, 0.49, 0.25);

	deci->Update();

	VTK_SP(vtkSTLWriter, franWriter);
	franWriter->SetFileName("F:/bin/export/370234001_0_L_decimate.stl");
	franWriter->SetFileTypeToBinary();
	franWriter->SetInputConnection(deci->GetOutputPort());
	franWriter->Update();


	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(franActor);
	renderer->SetBackground(1, 1, 1);
	renWin->SetSize(300, 300);
	VTK_SP(vtkCamera, cam1);
	cam1->SetClippingRange(0.0475572, 2.37786);
	cam1->SetFocalPoint(0.052665, -0.129454, -0.0573973);
	cam1->SetPosition(0.327637, -0.116299, -0.256418);
	cam1->SetViewUp(-0.0225386, 0.999137, 0.034901);
	renderer->SetActiveCamera(cam1);

	// interact with data
	renWin->Render();
	iren->Start();
}
void SmoothMesh5_2()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	VTK_SP(vtkPolyDataReader, fran);
	fran->SetFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/fran_cut.vtk");

	VTK_SP(vtkDecimatePro, deci);
	deci->SetInputConnection(fran->GetOutputPort());
	deci->SetTargetReduction(0.9);
	deci->PreserveTopologyOn();

	VTK_SP(vtkSmoothPolyDataFilter, smoother);
	smoother->SetInputConnection(fran->GetOutputPort());
	smoother->SetNumberOfIterations(50);

	VTK_SP(vtkPolyDataNormals, normals);
	normals->SetInputConnection(smoother->GetOutputPort());
	normals->FlipNormalsOn();

	VTK_SP(vtkPolyDataMapper, franMapper);
	franMapper->SetInputConnection(normals->GetOutputPort());

	VTK_SP(vtkActor, franActor);
	franActor->SetMapper(franMapper);
	franActor->GetProperty()->SetColor(1.0, 0.49, 0.25);

	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(franActor);
	renderer->SetBackground(1, 1, 1);
	renWin->SetSize(300, 300);
	VTK_SP(vtkCamera, cam1);
	cam1->SetClippingRange(0.0475572, 2.37786);
	cam1->SetFocalPoint(0.052665, -0.129454, -0.0573973);
	cam1->SetPosition(0.327637, -0.116299, -0.256418);
	cam1->SetViewUp(-0.0225386, 0.999137, 0.034901);
	renderer->SetActiveCamera(cam1);

	// interact with data
	renWin->Render();
	iren->Start();
}
void SmoothMeshSTL5_2()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	VTK_SP(vtkSTLReader, fran);
	fran->SetFileName("F:/bin/export/370234001_0_L.stl");

	VTK_SP(vtkDecimatePro, deci);
	deci->SetInputConnection(fran->GetOutputPort());
	deci->SetTargetReduction(0.9);
	deci->PreserveTopologyOn();

	VTK_SP(vtkSmoothPolyDataFilter, smoother);
	smoother->SetInputConnection(fran->GetOutputPort());
	smoother->SetNumberOfIterations(50);

	VTK_SP(vtkPolyDataNormals, normals);
	normals->SetInputConnection(smoother->GetOutputPort());
	normals->FlipNormalsOn();

	VTK_SP(vtkPolyDataMapper, franMapper);
	franMapper->SetInputConnection(normals->GetOutputPort());

	VTK_SP(vtkActor, franActor);
	franActor->SetMapper(franMapper);
	franActor->GetProperty()->SetColor(1.0, 0.49, 0.25);

	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(franActor);
	renderer->SetBackground(1, 1, 1);
	renWin->SetSize(300, 300);
	VTK_SP(vtkCamera, cam1);
	cam1->SetClippingRange(0.0475572, 2.37786);
	cam1->SetFocalPoint(0.052665, -0.129454, -0.0573973);
	cam1->SetPosition(0.327637, -0.116299, -0.256418);
	cam1->SetViewUp(-0.0225386, 0.999137, 0.034901);
	renderer->SetActiveCamera(cam1);

	// interact with data
	renWin->Render();
	iren->Start();
}
void ClipData5_2()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	VTK_SP(vtkBYUReader, cow);
	cow->SetFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/Viewpoint/cow.g");

	VTK_SP(vtkPolyDataNormals, cowNormals);
	cowNormals->SetInputConnection(cow->GetOutputPort());

	VTK_SP(vtkPlane, plane);
	plane->SetOrigin(0.25, 0, 0);
	plane->SetNormal(-1, -1, 0);

	VTK_SP(vtkClipPolyData, clipper);
	clipper->SetInputConnection(cowNormals->GetOutputPort());
	clipper->SetClipFunction(plane);
	clipper->GenerateClipScalarsOn();
	clipper->GenerateClippedOutputOn();
	clipper->SetValue(0.5);

	VTK_SP(vtkPolyDataMapper, clipMapper);
	clipMapper->SetInputConnection(clipper->GetOutputPort());
	clipMapper->ScalarVisibilityOff();

	VTK_SP(vtkProperty, backProp);
	backProp->SetDiffuseColor(1, double(99)/255, double(71)/ 255);

	VTK_SP(vtkActor, clipActor);
	clipActor->SetMapper(clipMapper);
	clipActor->GetProperty()->SetColor(double(51) / 255, double(161) / 255, double(201) / 255);
	clipActor->SetBackfaceProperty(backProp);

	VTK_SP(vtkCutter, cutEdges);
	cutEdges->SetInputConnection(cowNormals->GetOutputPort());
	cutEdges->SetCutFunction(plane);
	cutEdges->GenerateCutScalarsOn();
	cutEdges->SetValue(0, 0.5);
	VTK_SP(vtkStripper, cutStrips);
	cutStrips->SetInputConnection(cutEdges->GetOutputPort());
	cutStrips->Update();

	VTK_SP(vtkPolyData, cutPoly);
	cutPoly->SetPoints(cutStrips->GetOutput()->GetPoints());
	cutPoly->SetPolys(cutStrips->GetOutput()->GetLines());

	VTK_SP(vtkTriangleFilter, cutTriangles);
	cutTriangles->SetInputData(cutPoly);
	VTK_SP(vtkPolyDataMapper, cutMapper);
	cutMapper->SetInputData(cutPoly);
	cutMapper->SetInputConnection(cutTriangles->GetOutputPort());

	VTK_SP(vtkActor, cutActor);
	cutActor->SetMapper(cutMapper);
	cutActor->GetProperty()->SetColor(double(51) / 255, double(161) / 255, double(201) / 255);
	
	VTK_SP(vtkPolyDataMapper, restMapper);
	restMapper->SetInputConnection(clipper->GetClippedOutputPort());
	restMapper->ScalarVisibilityOff();
	VTK_SP(vtkActor, restActor);
	restActor->SetMapper(restMapper);
	restActor->GetProperty()->SetRepresentationToWireframe();


	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(clipActor);
	renderer->AddActor(cutActor);
	renderer->AddActor(restActor);

	renderer->SetBackground(1, 1, 1);
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->GetActiveCamera()->Dolly(1.5);
	renderer->ResetCameraClippingRange();


	renWin->SetSize(300, 300);
	// interact with data
	renWin->Render();
	iren->Start();

}
void ClipDataSTL5_2()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	VTK_SP(vtkSTLReader, cow);
	cow->SetFileName("F:/bin/export/370234001_0_L.stl");

	VTK_SP(vtkPolyDataNormals, cowNormals);
	cowNormals->SetInputConnection(cow->GetOutputPort());

	VTK_SP(vtkPlane, plane);
	plane->SetOrigin(0.25, 0, 0);
	plane->SetNormal(-1, -1, 0);

	VTK_SP(vtkClipPolyData, clipper);
	clipper->SetInputConnection(cowNormals->GetOutputPort());
	clipper->SetClipFunction(plane);
	clipper->GenerateClipScalarsOn();
	clipper->GenerateClippedOutputOn();
	clipper->SetValue(0.5);

	VTK_SP(vtkPolyDataMapper, clipMapper);
	clipMapper->SetInputConnection(clipper->GetOutputPort());
	clipMapper->ScalarVisibilityOff();

	VTK_SP(vtkProperty, backProp);
	backProp->SetDiffuseColor(1, double(99) / 255, double(71) / 255);

	VTK_SP(vtkActor, clipActor);
	clipActor->SetMapper(clipMapper);
	clipActor->GetProperty()->SetColor(double(51) / 255, double(161) / 255, double(201) / 255);
	clipActor->SetBackfaceProperty(backProp);

	VTK_SP(vtkCutter, cutEdges);
	cutEdges->SetInputConnection(cowNormals->GetOutputPort());
	cutEdges->SetCutFunction(plane);
	cutEdges->GenerateCutScalarsOn();
	cutEdges->SetValue(0, 0.5);
	VTK_SP(vtkStripper, cutStrips);
	cutStrips->SetInputConnection(cutEdges->GetOutputPort());
	cutStrips->Update();

	VTK_SP(vtkPolyData, cutPoly);
	cutPoly->SetPoints(cutStrips->GetOutput()->GetPoints());
	cutPoly->SetPolys(cutStrips->GetOutput()->GetLines());

	VTK_SP(vtkTriangleFilter, cutTriangles);
	cutTriangles->SetInputData(cutPoly);
	VTK_SP(vtkPolyDataMapper, cutMapper);
	cutMapper->SetInputData(cutPoly);
	cutMapper->SetInputConnection(cutTriangles->GetOutputPort());

	VTK_SP(vtkActor, cutActor);
	cutActor->SetMapper(cutMapper);
	cutActor->GetProperty()->SetColor(double(51) / 255, double(161) / 255, double(201) / 255);

	VTK_SP(vtkPolyDataMapper, restMapper);
	restMapper->SetInputConnection(clipper->GetClippedOutputPort());
	restMapper->ScalarVisibilityOff();
	VTK_SP(vtkActor, restActor);
	restActor->SetMapper(restMapper);
	restActor->GetProperty()->SetRepresentationToWireframe();


	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(clipActor);
	renderer->AddActor(cutActor);
	renderer->AddActor(restActor);

	renderer->SetBackground(1, 1, 1);
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->GetActiveCamera()->Dolly(1.5);
	renderer->ResetCameraClippingRange();


	renWin->SetSize(300, 300);
	// interact with data
	renWin->Render();
	iren->Start();
}

