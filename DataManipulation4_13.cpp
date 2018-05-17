#include "stdafx.h"
#include "DataManipulation4_13.h"

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
#include "vtkSphereSource.h"
#include "vtkTextActor.h"
#include "vtkIdFilter.h"
#include "vtkSelectVisiblePoints.h"
#include "vtkLabeledDataMapper.h"
#include "vtkCellCenters.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkFloatArray.h"
#include "vtkUnstructuredGrid.h"
#include "vtkContourFilter.h"
#include "vtkPointData.h"
#include "vtkExtractEdges.h"
#include "vtkShrinkPolyData.h"
#include "vtkCubeSource.h"
#include "vtkThresholdPoints.h"
#include "vtkGlyph3D.h"
#include "vtkVectorText.h"

void case12(vtkFloatArray* scalars, vtkVectorText *caselabel, int _IN, int _OUT)
{
	scalars->InsertValue(0, _OUT);
	scalars->InsertValue(1, _IN);
	scalars->InsertValue(2, _OUT);
	scalars->InsertValue(3, _IN);
	scalars->InsertValue(4, _IN);
	scalars->InsertValue(5, _IN);
	scalars->InsertValue(6, _OUT);
	scalars->InsertValue(7, _OUT);
	if (_IN == 1)
		caselabel->SetText("Case 12 - 00111010");
	else
		caselabel->SetText("Case 12 - 11000101");
}
void matching()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	vtkFloatArray *Scalars = vtkFloatArray::New();
	Scalars->InsertNextValue(1.0);
	Scalars->InsertNextValue(0.0);
	Scalars->InsertNextValue(0.0);
	Scalars->InsertNextValue(1.0);
	Scalars->InsertNextValue(0.0);
	Scalars->InsertNextValue(0.0);
	Scalars->InsertNextValue(0.0);
	Scalars->InsertNextValue(0.0);

	vtkPoints *Points = vtkPoints::New();
	Points->InsertNextPoint(0, 0, 0);
	Points->InsertNextPoint(1, 0, 0);
	Points->InsertNextPoint(1, 1, 0);
	Points->InsertNextPoint(0, 1, 0);
	Points->InsertNextPoint(0, 0, 1);
	Points->InsertNextPoint(1, 0, 1);
	Points->InsertNextPoint(1, 1, 1);
	Points->InsertNextPoint(0, 1, 1);

	vtkIdList *Ids = vtkIdList::New();
	Ids->InsertNextId(0);
	Ids->InsertNextId(1);
	Ids->InsertNextId(2);
	Ids->InsertNextId(3);
	Ids->InsertNextId(4);
	Ids->InsertNextId(5);
	Ids->InsertNextId(6);
	Ids->InsertNextId(7);

	vtkUnstructuredGrid *Grid = vtkUnstructuredGrid::New();
	Grid->Allocate(10, 10);
	Grid->InsertNextCell(12, Ids);
	Grid->SetPoints(Points);
	Grid->GetPointData()->SetScalars(Scalars);

	vtkContourFilter *Marching = vtkContourFilter::New();
	Marching->SetInputData(Grid);
	Marching->SetValue(0, 0.5);
	Marching->Update();

	vtkExtractEdges *triangleEdges = vtkExtractEdges::New();
	triangleEdges->SetInputConnection(Marching->GetOutputPort());
	vtkTubeFilter *triangleEdgeTubes = vtkTubeFilter::New();
	triangleEdgeTubes->SetInputConnection(triangleEdges->GetOutputPort());
	triangleEdgeTubes->SetRadius(.005);
	triangleEdgeTubes->SetNumberOfSides(6);
	triangleEdgeTubes->UseDefaultNormalOn();
	triangleEdgeTubes->SetDefaultNormal(.577, .577, .577);

	vtkPolyDataMapper *triangleEdgeMapper = vtkPolyDataMapper::New();
	triangleEdgeMapper->SetInputConnection(triangleEdgeTubes->GetOutputPort());
	triangleEdgeMapper->ScalarVisibilityOff();

	vtkActor *triangleEdgeActor = vtkActor::New();
	triangleEdgeActor->SetMapper(triangleEdgeMapper);
	triangleEdgeActor->GetProperty()->SetDiffuseColor((double) 46 / 255, (double) 71 / 255, (double) 59/ 255);
	triangleEdgeActor->GetProperty()->SetSpecular(.4);
	triangleEdgeActor->GetProperty()->SetSpecularPower(10);

	vtkShrinkPolyData *aShrinker = vtkShrinkPolyData::New();
	aShrinker->SetShrinkFactor(1);
	aShrinker->SetInputConnection(Marching->GetOutputPort());
	vtkPolyDataMapper *aMapper = vtkPolyDataMapper::New();
	aMapper->ScalarVisibilityOff();
	aMapper->SetInputConnection(aShrinker->GetOutputPort());
	vtkActor *Triangles = vtkActor::New();
	Triangles->SetMapper(aMapper);
	Triangles->GetProperty()->SetDiffuseColor((double) 227 / 255, (double) 207 / 255, (double) 87/ 255);
	Triangles->GetProperty()->SetOpacity(.6);

	vtkCubeSource *CubeModel = vtkCubeSource::New();
	CubeModel->SetCenter(.5, .5, .5);
	vtkExtractEdges *Edges = vtkExtractEdges::New();
	Edges->SetInputConnection(CubeModel->GetOutputPort());
	vtkTubeFilter *Tubes = vtkTubeFilter::New();
	Tubes->SetInputConnection(Edges->GetOutputPort());
	Tubes->SetRadius(.01);
	Tubes->SetNumberOfSides(6);
	Tubes->UseDefaultNormalOn();
	Tubes->SetDefaultNormal(.577, .577, .577);

	vtkPolyDataMapper *TubeMapper = vtkPolyDataMapper::New();
	TubeMapper->SetInputConnection(Tubes->GetOutputPort());
	vtkActor *CubeEdges = vtkActor::New();
	CubeEdges->SetMapper(TubeMapper);
	CubeEdges->GetProperty()->SetDiffuseColor((double) 240 / 255, (double) 230 / 255, (double) 140 / 255);
	CubeEdges->GetProperty()->SetSpecular(.4);
	CubeEdges->GetProperty()->SetSpecularPower(10);

	vtkSphereSource *Sphere = vtkSphereSource::New();
	Sphere->SetRadius(0.04);
	Sphere->SetPhiResolution(20);
	Sphere->SetThetaResolution(20);

	vtkThresholdPoints *ThresholdIn = vtkThresholdPoints::New();
	ThresholdIn->SetInputData(Grid);
	ThresholdIn->ThresholdByUpper(.5);

	vtkGlyph3D *Vertices = vtkGlyph3D::New();
	Vertices->SetInputConnection(ThresholdIn->GetOutputPort());
	Vertices->SetSourceConnection(Sphere->GetOutputPort());

	vtkPolyDataMapper *SphereMapper = vtkPolyDataMapper::New();
	SphereMapper->SetInputConnection(Vertices->GetOutputPort());
	SphereMapper->ScalarVisibilityOff();
	vtkActor *CubeVertices = vtkActor::New();
	CubeVertices->SetMapper(SphereMapper);
	CubeVertices->GetProperty()->SetDiffuseColor(1, (double) 99 / 255, (double) 71 / 255);
	CubeVertices->GetProperty()->SetDiffuseColor(1, (double) 99 / 255, (double) 71 / 255);

	vtkVectorText *caseLabel = vtkVectorText::New();
	caseLabel->SetText("Case 1");

	vtkTransform *aLabelTransform = vtkTransform::New();
	aLabelTransform->Identity();
	aLabelTransform->Translate(-.2, 0, 1.25);
	aLabelTransform->Scale(.05, .05, .05);

	vtkTransformPolyDataFilter *labelTransform = vtkTransformPolyDataFilter::New();
	labelTransform->SetTransform(aLabelTransform);
	labelTransform->SetInputConnection(caseLabel->GetOutputPort());

	vtkPolyDataMapper *labelMapper = vtkPolyDataMapper::New();
	labelMapper->SetInputConnection(labelTransform->GetOutputPort());

	vtkActor *labelActor = vtkActor::New();
	labelActor->SetMapper(labelMapper);

	vtkCubeSource *baseModel = vtkCubeSource::New();
	baseModel->SetXLength(1.5);
	baseModel->SetYLength(.01);
	baseModel->SetZLength(1.5);
	vtkPolyDataMapper *baseMapper = vtkPolyDataMapper::New();
	baseMapper->SetInputConnection(baseModel->GetOutputPort());
	vtkActor *base = vtkActor::New();
	base->SetMapper(baseMapper);
	base->SetPosition(.5, -.09, .5);

	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	renWin->SetSize(640, 480);

 	ren1->AddActor(triangleEdgeActor);
	ren1->AddActor(base);
	ren1->AddActor(labelActor);
	ren1->AddActor(CubeEdges);
	ren1->AddActor(CubeVertices);
	ren1->AddActor(Triangles);

	ren1->SetBackground((double) 112 / 255, (double) 128 / 255, (double) 144 / 255);
	case12(Scalars, caseLabel, 0, 1);

	Grid->Modified();

	ren1->GetActiveCamera()->Dolly(1.2);
	ren1->GetActiveCamera()->Azimuth(30);
	ren1->GetActiveCamera()->Elevation(20);
	ren1->ResetCameraClippingRange();

	iren->Initialize();

	renWin->Render();

	iren->Start();
}