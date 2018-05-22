#include "stdafx.h"
#include "PointDataAndCellDataConvert5_1.h"

#include "vtkUnstructuredGridReader.h"
#include "vtkPointDataToCellData.h"
#include "vtkWarpVector.h"
#include "vtkThreshold.h"
#include "vtkConnectivityFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkGeometryFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkLookupTable.h"
#include "vtkPolyDataMapper.h"
#include "vtkContourFilter.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkMultiBlockDataSet.h"

#include "vtkStructuredGridGeometryFilter.h"
#include "vtkStructuredGridOutlineFilter.h"

#include "vtkQuadric.h"
#include "vtkSampleFunction.h"
#include "vtkOutlineFilter.h"

#include "vtkPolyDataReader.h"
#include "vtkMaskPoints.h"
#include "vtkConeSource.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
#include "vtkGlyph3D.h"
#include "vtkSTLReader.h"
#include "vtkPlane.h"
#include "vtkImplicitPlaneWidget.h"
#include "vtkCutter.h"
#include "vtkProbeFilter.h"
#include "vtkDataSet.h"
#include "vtkDataSetAttributes.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
// #include "vtkmPolyDataNormals.h"
#include "vtkBMPReader.h"
#include "vtkImageLuminance.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkWarpScalar.h"
#include "vtkMergeFilter.h"
#include "vtkPlaneSource.h"
#include "vtkAppendPolyData.h"
#include "vtkSphere.h"
#include "vtkImplicitBoolean.h"
#include "vtkExtractGeometry.h"
#include "vtkShrinkFilter.h"
#include "vtkFloatArray.h"


void PointDataAndCellDataConvert5_1()
{
	vtkUnstructuredGridReader *reader = vtkUnstructuredGridReader::New();
	reader->SetFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/blow.vtk");
	reader->SetScalarsName("thickness9");
	reader->SetVectorsName("displacement9");

	vtkPointDataToCellData *p2c = vtkPointDataToCellData::New();
	p2c->SetInputConnection(reader->GetOutputPort());
	p2c->PassPointDataOn();

	vtkWarpVector *warp = vtkWarpVector::New();
	warp->SetInputConnection(p2c->GetOutputPort());
	vtkThreshold *thresh = vtkThreshold::New();
	thresh->SetInputConnection(warp->GetOutputPort());
	thresh->ThresholdBetween(0.25, 0.75);
	thresh->SetInputArrayToProcess(1, 0, 0, 0, "thickness9");
	//thresh->SetAttributeModeToUseCellData();

	auto connect = vtkConnectivityFilter::New();
	connect->SetInputConnection(thresh->GetOutputPort());
	connect->SetExtractionModeToSpecifiedRegions();
	connect->AddSpecifiedRegion(0);
	connect->AddSpecifiedRegion(1);

	auto moldMapper = vtkDataSetMapper::New();
	moldMapper->SetInputConnection(reader->GetOutputPort());
	moldMapper->ScalarVisibilityOff();
	auto moldActor = vtkActor::New();
	moldActor->SetMapper(moldMapper);
	moldActor->GetProperty()->SetColor(.2, .2, .2);
	moldActor->GetProperty()->SetRepresentationToWireframe();

	auto connect2 = vtkConnectivityFilter::New();
	connect2->SetInputConnection(thresh->GetOutputPort());
	auto parison = vtkGeometryFilter::New();
	parison->SetInputConnection(connect2->GetOutputPort());
	auto normals2 = vtkPolyDataNormals::New();
	normals2->SetInputConnection(parison->GetOutputPort());
	normals2->SetFeatureAngle(60);
	auto lut = vtkLookupTable::New();
	lut->SetHueRange(0.0, 0.66667);
	auto parisonMapper = vtkPolyDataMapper::New();
	parisonMapper->SetInputConnection(normals2->GetOutputPort());
	parisonMapper->SetLookupTable(lut);
	parisonMapper->SetScalarRange(0.12, 1.0);
	auto parisonActor = vtkActor::New();
	parisonActor->SetMapper(parisonMapper);

	auto cf = vtkContourFilter::New();
	cf->SetInputConnection(connect2->GetOutputPort());
	cf->SetValue(0, .5);
	auto contourMapper = vtkPolyDataMapper::New();
	contourMapper->SetInputConnection(cf->GetOutputPort());
	auto contours = vtkActor::New();
	contours->SetMapper(contourMapper);

	auto ren = vtkRenderer::New();
	auto renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren);
	auto iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	// Add the actors to the renderer, set the background and size
	ren->AddActor(moldActor);
	ren->AddActor(parisonActor);
	ren->AddActor(contours);

	ren->ResetCamera();
	ren->GetActiveCamera()->Azimuth(60);
	ren->GetActiveCamera()->Roll(-90);
	ren->GetActiveCamera()->Dolly(2);
	ren->ResetCameraClippingRange();

	ren->SetBackground(1, 1, 1);
	renWin->SetSize(750, 400);

	iren->Initialize();
	renWin->Render();
	iren->Start();

}
void RainbowRender5_1()
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
	vtkDataSet* pl3d_output = static_cast<vtkDataSet*>(pl3d->GetOutput()->GetBlock(0));

	auto plane = vtkStructuredGridGeometryFilter::New();
	plane->SetInputData(pl3d_output);
	plane->SetExtent(1, 100, 1, 100, 7, 7);
	auto lut = vtkLookupTable::New();
	auto planeMapper = vtkPolyDataMapper::New();
	planeMapper->SetLookupTable(lut);
	planeMapper->SetInputConnection(plane->GetOutputPort());
	planeMapper->SetScalarRange(pl3d_output->GetScalarRange());
	auto planeActor = vtkActor::New();
	planeActor->SetMapper(planeMapper);

	// This creates an outline around the data.
	auto outline = vtkStructuredGridOutlineFilter::New();
	outline->SetInputData(pl3d_output);
	auto outlineMapper = vtkPolyDataMapper::New();
	outlineMapper->SetInputConnection(outline->GetOutputPort());
	auto outlineActor = vtkActor::New();
	outlineActor->SetMapper(outlineMapper);

	// Much of the following is commented out.To try different lookup tables,
	// uncommented the appropriate portions.
	//
	// This creates a black to white lut.
	// //lut.SetHueRange(0, 0)
	// //lut.SetSaturationRange(0, 0)
	// //lut.SetValueRange(0.2, 1.0)
	//
	// This creates a red to blue lut.
	// //lut.SetHueRange(0.0, 0.667)
	//
	// This creates a blue to red lut.
	// //lut.SetHueRange(0.667, 0.0)
	//
	// This creates a weird effect.The Build() method causes the lookup
	// table to allocate memory and create a table based on the current
	// hue, saturation, value, and alpha(transparency) range.Here we then
	// manually overwrite the values generated by the Build() method.
	lut->SetNumberOfColors(256);
	lut->Build();
	for (int i = 0; i < 16; i++) {
		lut->SetTableValue(i * 16, 1, 0, 0, 1);
		lut->SetTableValue(i * 16 + 1, 0, 1, 0, 1);
		lut->SetTableValue(i * 16 + 2, 0, 0, 1, 1);
		lut->SetTableValue(i * 16 + 3, 0, 0, 0, 1);
	}

		// Create the RenderWindow, Renderer and both Actors
	auto ren = vtkRenderer::New();
	auto renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren);
	auto iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	// Add the actors to the renderer, set the background and size
	ren->AddActor(outlineActor);
	ren->AddActor(planeActor);

	ren->SetBackground(0.1, 0.2, 0.4);
	ren->TwoSidedLightingOff();

	renWin->SetSize(250, 250);

	auto cam1 = ren->GetActiveCamera();
	cam1->SetClippingRange(3.95297, 50);
	cam1->SetFocalPoint(8.88908, 0.595038, 29.3342);
	cam1->SetPosition(-12.3332, 31.7479, 41.2387);
	cam1->SetViewUp(0.060772, -0.319905, 0.945498);

	iren->Initialize();
	renWin->Render();
	iren->Start();


}
void VisQuad5_1()
{
	// VTK supports implicit functions of the form f(x, y, z) = constant.These
	// functions can represent things spheres, cones, etc.Here we use a
	// general form for a quadric to create an elliptical data field.
	auto quadric = vtkQuadric::New();
	quadric->SetCoefficients(.5, 1, .2, 0, .1, 0, 0, .2, 0, 0);

	// vtkSampleFunction samples an implicit function over the x - y - z range
	// specified(here it defaults to - 1, 1 in the x, y, z directions).
	auto sample = vtkSampleFunction::New();
	sample->SetSampleDimensions(30, 30, 30);
	sample->SetImplicitFunction(quadric);

	// Create five surfaces F(x, y, z) = constant between range specified.The
	// GenerateValues() method creates n isocontour values between the range
	// specified.
	auto contours = vtkContourFilter::New();
	contours->SetInputConnection(sample->GetOutputPort());
	contours->GenerateValues(6, 0.0, 1.2);

	auto contMapper = vtkPolyDataMapper::New();
	contMapper->SetInputConnection(contours->GetOutputPort());
	contMapper->SetScalarRange(0.0, 1.2);

	auto contActor = vtkActor::New();
	contActor->SetMapper(contMapper);

	// We'll put a simple outline around the data.
	auto outline = vtkOutlineFilter::New();
	outline->SetInputConnection(sample->GetOutputPort());

	auto outlineMapper = vtkPolyDataMapper::New();
	outlineMapper->SetInputConnection(outline->GetOutputPort());

	auto outlineActor = vtkActor::New();
	outlineActor->SetMapper(outlineMapper);
	outlineActor->GetProperty()->SetColor(0, 0, 0);

	// The usual rendering stuff.
	auto ren = vtkRenderer::New();
	auto renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren);
	auto iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	ren->SetBackground(1, 1, 1);
	ren->AddActor(contActor);
	ren->AddActor(outlineActor);

	iren->Initialize();
	renWin->Render();
	iren->Start();

}
void SpikeF5_1()
{
	// Read a data file.This originally was a Cyberware laser digitizer scan
	// of Fran J.'s face. Surface normals are generated based on local geometry
	// (i.e., the polygon normals surrounding eash point are averaged). We flip
	// the normals because we want them to point out from Fran's face.

	auto fran = vtkPolyDataReader::New();
	fran->SetFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/fran_cut.vtk");

// 	auto fran = vtkSTLReader::New();
// 	fran->SetFileName("F:/bin/цотф/maxillary_export.stl");

	auto normals = vtkPolyDataNormals::New();
	normals->SetInputConnection(fran->GetOutputPort());
	normals->FlipNormalsOn();
	auto franMapper = vtkPolyDataMapper::New();
	franMapper->SetInputConnection(normals->GetOutputPort());
	auto franActor = vtkActor::New();
	franActor->SetMapper(franMapper);
	franActor->GetProperty()->SetColor(1.0, 0.49, 0.25);

	// We subsample the dataset because we want to glyph just a subset of
	// the points.Otherwise the display is cluttered and cannot be easily
	// read.The RandonModeOn and SetOnRatio combine to random select one out
	// of every 10 points in the dataset.
	auto ptMask = vtkMaskPoints::New();
	ptMask->SetInputConnection(normals->GetOutputPort());
	ptMask->SetOnRatio(2000);
	ptMask->RandomModeOn();
	// In this case we are using a cone as a glyph.We transform the cone so
	// its base is at 0, 0, 0. This is the point where glyph rotation occurs.
	auto cone = vtkConeSource::New();
	cone->SetResolution(6);
	auto transform = vtkTransform::New();
	transform->Translate(0.5, 0.0, 0.0);
	auto transformF = vtkTransformPolyDataFilter::New();
	transformF->SetInputConnection(cone->GetOutputPort());
	transformF->SetTransform(transform);

	// vtkGlyph3D takes two inputs : the input point set(SetInput) which can be
	// any vtkDataSet; and the glyph(SetSource) which must be a vtkPolyData.
	// We are interested in orienting the glyphs by the surface normals that
	// we previosuly generated.
	auto glyph = vtkGlyph3D::New();
	glyph->SetInputConnection(ptMask->GetOutputPort());
	glyph->SetSourceConnection(transformF->GetOutputPort());
	glyph->SetVectorModeToUseNormal();
	glyph->SetScaleModeToScaleByVector();
	glyph->SetScaleFactor(0.04);
	auto spikeMapper = vtkPolyDataMapper::New();
	spikeMapper->SetInputConnection(glyph->GetOutputPort());
	auto spikeActor = vtkActor::New();
	spikeActor->SetMapper(spikeMapper);
	spikeActor->GetProperty()->SetColor(0.0, 0.79, 0.34);

	auto ren = vtkRenderer::New();
	auto renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren);
	auto iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	ren->AddActor(franActor);
	ren->AddActor(spikeActor);

	renWin->SetSize(500, 500);
	ren->SetBackground(0.1, 0.2, 0.4);

	iren->Initialize();
	renWin->Render();
	iren->Start();

}
void ProbeCutting5_1()
{
	VTK_SP(vtkRenderer, ren1);
	VTK_SP(vtkRenderWindow, renWin);
	VTK_SP(vtkRenderWindowInteractor, iren);
	renWin->SetMultiSamples(0);
	renWin->AddRenderer(ren1);
	iren->SetRenderWindow(renWin);

	VTK_SP(vtkMultiBlockPLOT3DReader, pl3d);
	pl3d->SetXYZFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/combxyz.bin");
	pl3d->SetQFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/combq.bin");
	pl3d->SetScalarFunctionNumber(100);
	pl3d->SetVectorFunctionNumber(202);
	pl3d->Update();
	vtkDataSet* output = static_cast<vtkDataSet*>(pl3d->GetOutput()->GetBlock(0));
	
	VTK_SP(vtkPlane, plane);
	plane->SetOrigin(output->GetCenter());
	plane->SetNormal(-0.287, 0, 0.9579);

	VTK_SP(vtkCutter, planeCut);
	planeCut->SetInputData(output);
	planeCut->SetCutFunction(plane);

	VTK_SP(vtkProbeFilter, probe);
	probe->SetInputConnection(planeCut->GetOutputPort());
	probe->SetSourceData(output);

	VTK_SP(vtkDataSetMapper, cutMapper);
	cutMapper->SetInputConnection(probe->GetOutputPort());
	double range[2];
	output->GetPointData()->GetScalars()->GetRange(range);
	cutMapper->SetScalarRange(range);

	VTK_SP(vtkActor, cutActor);
	cutActor->SetMapper(cutMapper);

	VTK_SP(vtkStructuredGridGeometryFilter, compPlane);
	compPlane->SetInputData(output);
	compPlane->SetExtent(0, 100, 0, 100, 9, 9);

	VTK_SP(vtkPolyDataMapper, planeMapper);
	planeMapper->SetInputConnection(compPlane->GetOutputPort());
	planeMapper->ScalarVisibilityOff();

	VTK_SP(vtkActor, planeActor);
	planeActor->SetMapper(planeMapper);
	planeActor->GetProperty()->SetRepresentationToWireframe();
	planeActor->GetProperty()->SetColor(0, 0, 0);

	VTK_SP(vtkStructuredGridOutlineFilter, outline);
	outline->SetInputData(output);

	VTK_SP(vtkPolyDataMapper, outlineMapper);
	outlineMapper->SetInputConnection(outline->GetOutputPort());

	VTK_SP(vtkActor, outlineActor);
	outlineActor->SetMapper(outlineMapper);

	auto outlineProp = outlineActor->GetProperty();
	outlineProp->SetColor(0, 0, 0);

	ren1->AddActor(outlineActor);
	ren1->AddActor(planeActor);
	ren1->AddActor(cutActor);
	ren1->SetBackground(1, 1, 1);
	renWin->SetSize(400, 300);

	auto cam1 = ren1->GetActiveCamera();
	cam1->SetClippingRange(11.1034, 59.5328);
	cam1->SetFocalPoint(9.71821, 0.458166, 29.3999);
	cam1->SetPosition(-2.95748, -26.7271, 44.5309);
	cam1->SetViewUp(0.0184785, 0.479657, 0.877262);

	iren->Initialize();
	renWin->Render();
	iren->Start();

}
void DataMerging5_1()
{
	// Read in an image and compute a luminance value.The image is
	// extracted as a set of polygons(vtkImageDataGeometryFilter).We then
	// will warp the plane using the scalar(luminance) values.
	VTK_SP(vtkBMPReader, reader);
	reader->SetFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/masonry.bmp");
	VTK_SP(vtkImageLuminance, luminance);
	luminance->SetInputConnection(reader->GetOutputPort());
	VTK_SP(vtkImageDataGeometryFilter, geometry);
	geometry->SetInputConnection(luminance->GetOutputPort());
	VTK_SP(vtkWarpScalar, warp);
	warp->SetInputConnection(geometry->GetOutputPort());
	warp->SetScaleFactor(-0.1);

	// Use vtkMergeFilter to combine the original image with the warped
	// geometry.
	VTK_SP(vtkMergeFilter, merge);
	merge->SetGeometryConnection(warp->GetOutputPort());
	merge->SetScalarsConnection(reader->GetOutputPort());
	VTK_SP(vtkDataSetMapper, mapper);
	mapper->SetInputConnection(merge->GetOutputPort());
	mapper->SetScalarRange(0, 255);
	VTK_SP(vtkActor, actor);
	actor->SetMapper(mapper);

		// Create renderer stuff
	VTK_SP(vtkRenderer, ren);
	VTK_SP(vtkRenderWindow, renWin);
	VTK_SP(vtkRenderWindowInteractor, iren);
	renWin->SetMultiSamples(0);
	renWin->AddRenderer(ren);
	iren->SetRenderWindow(renWin);

		// Add the actors to the renderer, set the background and size
	ren->AddActor(actor);
	ren->ResetCamera();
	ren->GetActiveCamera()->Azimuth(20);
	ren->GetActiveCamera()->Elevation(30);
	ren->SetBackground(0.1, 0.2, 0.4);
	ren->ResetCameraClippingRange();

	renWin->SetSize(250, 250);

	auto cam1 = ren->GetActiveCamera();
	cam1->Zoom(1.4);

	iren->Initialize();
	renWin->Render();
	iren->Start();
}
void ProbeComb5_1()
{
	VTK_SP(vtkRenderer, ren1);
	VTK_SP(vtkRenderWindow, renWin);
	VTK_SP(vtkRenderWindowInteractor, iren);
	renWin->SetMultiSamples(0);
	renWin->AddRenderer(ren1);
	iren->SetRenderWindow(renWin);

	VTK_SP(vtkMultiBlockPLOT3DReader, pl3d);
	pl3d->SetXYZFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/combxyz.bin");
	pl3d->SetQFileName("G:/VTK-8.1.0/build/ExternalData/Testing/Data/combq.bin");
	pl3d->SetScalarFunctionNumber(100);
	pl3d->SetVectorFunctionNumber(202);
	pl3d->Update();
	vtkDataSet* output = static_cast<vtkDataSet*>(pl3d->GetOutput()->GetBlock(0));

	vtkDataSet* pl3d_output = static_cast<vtkDataSet*>(pl3d->GetOutput()->GetBlock(0));

	auto planeF = vtkStructuredGridGeometryFilter::New();
	planeF->SetInputData(pl3d_output);
	planeF->SetExtent(1, 100, 1, 100, 7, 7);
	auto lut = vtkLookupTable::New();
	auto planeMapper = vtkPolyDataMapper::New();
	planeMapper->SetLookupTable(lut);
	planeMapper->SetInputConnection(planeF->GetOutputPort());
	planeMapper->SetScalarRange(pl3d_output->GetScalarRange());
	auto orgActor = vtkActor::New();
	orgActor->SetMapper(planeMapper);

	VTK_SP(vtkPlaneSource, plane);
	plane->SetResolution(50, 50);
	
	VTK_SP(vtkTransform, transP1);
	transP1->Translate(3.7, 0.0, 28.37);
	transP1->Scale(5, 5, 5);
	transP1->RotateY(90);

	VTK_SP(vtkTransformPolyDataFilter, tpd1);
	tpd1->SetInputConnection(plane->GetOutputPort());
	tpd1->SetTransform(transP1);

	VTK_SP(vtkOutlineFilter, outTpd1);
	outTpd1->SetInputConnection(tpd1->GetOutputPort());

	VTK_SP(vtkPolyDataMapper, mapTpd1);
	mapTpd1->SetInputConnection(outTpd1->GetOutputPort());

	VTK_SP(vtkActor, tpd1Actor);
	tpd1Actor->SetMapper(mapTpd1);
	tpd1Actor->GetProperty()->SetColor(0, 0, 0);

	VTK_SP(vtkTransform, transP2);
	transP2->Translate(9.2, 0.0, 31.20);
	transP2->Scale(5, 5, 5);
	transP2->RotateY(90);

	VTK_SP(vtkTransformPolyDataFilter, tpd2);
	tpd2->SetInputConnection(plane->GetOutputPort());
	tpd2->SetTransform(transP2);

	VTK_SP(vtkOutlineFilter, outTpd2);
	outTpd2->SetInputConnection(tpd2->GetOutputPort());

	VTK_SP(vtkPolyDataMapper, mapTpd2);
	mapTpd2->SetInputConnection(outTpd2->GetOutputPort());

	VTK_SP(vtkActor, tpd2Actor);
	tpd2Actor->SetMapper(mapTpd2);
	tpd2Actor->GetProperty()->SetColor(0, 0, 0);

	VTK_SP(vtkTransform, transP3);
	transP3->Translate(13.27, 0.0, 33.30);
	transP3->Scale(5, 5, 5);
	transP3->RotateY(90);

	VTK_SP(vtkTransformPolyDataFilter, tpd3);
	tpd3->SetInputConnection(plane->GetOutputPort());
	tpd3->SetTransform(transP3);

	VTK_SP(vtkOutlineFilter, outTpd3);
	outTpd3->SetInputConnection(tpd3->GetOutputPort());

	VTK_SP(vtkPolyDataMapper, mapTpd3);
	mapTpd3->SetInputConnection(outTpd3->GetOutputPort());

	VTK_SP(vtkActor, tpd3Actor);
	tpd3Actor->SetMapper(mapTpd3);
	tpd3Actor->GetProperty()->SetColor(0, 0, 0);

	VTK_SP(vtkAppendPolyData, appendF);
	appendF->AddInputConnection(tpd1->GetOutputPort());
	appendF->AddInputConnection(tpd2->GetOutputPort());
	appendF->AddInputConnection(tpd3->GetOutputPort());

	VTK_SP(vtkProbeFilter, probe);
	probe->SetInputConnection(appendF->GetOutputPort());
	probe->SetSourceData(output);

	VTK_SP(vtkContourFilter, contour);
	contour->SetInputConnection(probe->GetOutputPort());
	contour->GenerateValues(50, output->GetScalarRange());
	
	VTK_SP(vtkPolyDataMapper, contourMapper);
	contourMapper->SetInputConnection(contour->GetOutputPort());
	contourMapper->SetScalarRange(output->GetScalarRange());

	VTK_SP(vtkActor, planeActor);
	planeActor->SetMapper(contourMapper);
	
	VTK_SP(vtkStructuredGridOutlineFilter, outline);
	outline->SetInputData(output);
	VTK_SP(vtkPolyDataMapper, outlineMapper);
	outlineMapper->SetInputConnection(outline->GetOutputPort());
	
	VTK_SP(vtkActor, outlineActor);
	outlineActor->SetMapper(outlineMapper);
	outlineActor->GetProperty()->SetColor(0, 0, 0);

	ren1->AddActor(outlineActor);
	ren1->AddActor(planeActor);
	ren1->AddActor(orgActor);
	ren1->AddActor(tpd1Actor);
	ren1->AddActor(tpd2Actor);
	ren1->AddActor(tpd3Actor);
	ren1->SetBackground(1, 1, 1);
	renWin->SetSize(400, 300);

	auto cam1 = ren1->GetActiveCamera();
	cam1->SetClippingRange(11.1034, 59.5328);
	cam1->SetFocalPoint(9.71821, 0.458166, 29.3999);
	cam1->SetPosition(-2.95748, -26.7271, 44.5309);
	cam1->SetViewUp(0.0184785, 0.479657, 0.877262);

	iren->Initialize();
	renWin->Render();
	iren->Start();

}
void ExtractGeometry5_1()
{
	auto quadric = vtkQuadric::New();
	quadric->SetCoefficients(.5, 1, .2, 0, .1, 0, 0, .2, 0, 0);

	auto sample = vtkSampleFunction::New();
	sample->SetSampleDimensions(50, 50, 50);
	sample->SetImplicitFunction(quadric);

	VTK_SP(vtkTransform, transP1);
	transP1->Scale(1, .5, .333);
	VTK_SP(vtkSphere, sphere1);
	sphere1->SetRadius(0.25);
	sphere1->SetTransform(transP1);

	VTK_SP(vtkTransform, transP2);
	transP2->Scale(.333, .5, 1.0);
	VTK_SP(vtkSphere, sphere2);
	sphere2->SetRadius(0.25);
	sphere2->SetTransform(transP2);

	VTK_SP(vtkImplicitBoolean, union1);
	union1->AddFunction(sphere1);
	union1->AddFunction(sphere2);
	union1->SetOperationType(0);

	VTK_SP(vtkExtractGeometry, extract);
	extract->SetInputConnection(sample->GetOutputPort());
	extract->SetImplicitFunction(union1);
	VTK_SP(vtkShrinkFilter, shrink);
	shrink->SetInputConnection(extract->GetOutputPort());
	shrink->SetShrinkFactor(.5);
	VTK_SP(vtkDataSetMapper, dataMapper);
	dataMapper->SetInputConnection(shrink->GetOutputPort());
	VTK_SP(vtkActor, dataActor);
	dataActor->SetMapper(dataMapper);

	VTK_SP(vtkOutlineFilter, outline);
	outline->SetInputConnection(sample->GetOutputPort());
	VTK_SP(vtkPolyDataMapper, outlineMapper);
	outlineMapper->SetInputConnection(outline->GetOutputPort());
	VTK_SP(vtkActor, outlineActor);
	outlineActor->SetMapper(outlineMapper);
	outlineActor->GetProperty()->SetColor(0, 0, 0);
	
	VTK_SP(vtkRenderer, ren1);
	VTK_SP(vtkRenderWindow, renWin);
	VTK_SP(vtkRenderWindowInteractor, iren);
	renWin->SetMultiSamples(0);
	renWin->AddRenderer(ren1);
	iren->SetRenderWindow(renWin);

	ren1->AddActor(outlineActor);
	ren1->AddActor(dataActor);
	ren1->SetBackground(1, 1, 1);
	renWin->SetSize(500, 500);
	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.5);

	

	iren->Initialize();
	renWin->Render();
	iren->Start();

}

