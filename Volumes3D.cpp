#include "stdafx.h"
#include "Volumes3D.h"

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
#include "vtkLODProp3D.h"
#include "vtkBMPReader.h"
#include "vtkTexture.h"
#include "vtkPlaneSource.h"

#include "vtkCellPicker.h"
#include "vtkTextMapper.h"
#include "vtkActor2D.h"
#include "vtkSphereSource.h"
#include "vtkCubeSource.h"
#include "vtkConeSource.h"
#include "vtkCylinderSource.h"
#include "vtkGlyph3D.h"
#include "vtkTextProperty.h"
#include "vtkCallbackCommand.h"


void Volumes3D_46()
{
	vtkBMPReader *bmpReader = vtkBMPReader::New();
	bmpReader->SetFileName("G:\\VTK-8.1.0\\build\\ExternalData\\Testing\\Data\\masonry.bmp");
	vtkTexture *atext = vtkTexture::New();
	atext->SetInputConnection(bmpReader->GetOutputPort());
	atext->InterpolateOn();

	vtkSTLReader *part = vtkSTLReader::New();
	part->SetFileName("G:\\VTK-8.1.0\\build\\ExternalData\\Testing\\Data\\42400-IDGH.stl");

	vtkPolyDataMapper *partMapper = vtkPolyDataMapper::New();
	partMapper->SetInputConnection(part->GetOutputPort());

	vtkActor *partActor = vtkActor::New();
	partActor->SetMapper(partMapper);
//  	partActor->GetProperty()->SetColor(0.82745, 0.82745, 0.82745);
	partActor->SetTexture(atext);

	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
// 	vtkInteractorStyleFlight *flightStyle = vtkInteractorStyleFlight::New();
// 	iren->SetInteractorStyle(flightStyle);

	ren1->AddActor(partActor);
	ren1->SetBackground(0.1, 0.2, 0.4);
	renWin->SetSize(300, 300);

	iren->Initialize();

	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.6);
	renWin->Render();

// 	partActor->Delete();
// 	partMapper->Delete();
// 	part->Delete();
// 	ren1->Delete();
// 	renWin->Delete();
// 	iren->Delete();
}

void Texture47()
{
	vtkBMPReader *bmpReader = vtkBMPReader::New();
	bmpReader->SetFileName("G:\\VTK-8.1.0\\build\\ExternalData\\Testing\\Data\\masonry.bmp");
	vtkTexture *atext = vtkTexture::New();
	atext->SetInputConnection(bmpReader->GetOutputPort());
	atext->InterpolateOn();

	vtkPlaneSource *plane = vtkPlaneSource::New();
	vtkPolyDataMapper *planeMapper = vtkPolyDataMapper::New();
	planeMapper->SetInputConnection(plane->GetOutputPort());
	vtkActor *planeActor = vtkActor::New();
	planeActor->SetMapper(planeMapper);
	planeActor->SetTexture(atext);

	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	ren1->AddActor(planeActor);
	ren1->SetBackground(0.1, .2, .4);
	renWin->SetSize(500, 500);

	renWin->Render();

	ren1->ResetCamera();
	vtkCamera *cam1 = ren1->GetActiveCamera();
	cam1->Elevation(-30);
	cam1->Roll(-20);
	ren1->ResetCameraClippingRange();

	renWin->Render();
	iren->Start();





}


/*
static double *selPt;
static double x;
static double y;
static double xp;
static double yp;
static double zp;
vtkCellPicker *picker;
vtkActor2D *textActor;
vtkTextMapper *textMapper;
vtkRenderWindow *renWin;
vtkPoints *pickPos;

void myCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData)
{
	if (picker->GetCellId() < 0) {
		textActor->VisibilityOff();
	} else {
		selPt = picker->GetSelectionPoint();
		x = *selPt;
		y = *(selPt + 1);
		pickPos = picker->GetPickedPositions();
		xp = *(pickPos->GetPoint(0));
		yp = *(pickPos->GetPoint(0) + 1);
		zp = *(pickPos->GetPoint(0) + 2);
		char str[50];
		sprintf_s(str, "%f,%f,%f", xp, yp, zp);
		textMapper->SetInput(str);
		textActor->SetPosition(x, y);
		textActor->VisibilityOn();
	}

	renWin->Render();
}
void Picking48()
{
	vtkSphereSource *sphere = vtkSphereSource::New();
	vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
	sphereMapper->SetInputConnection(sphere->GetOutputPort());
	vtkLODActor *sphereActor = vtkLODActor::New();
	sphereActor->SetMapper(sphereMapper);

	vtkConeSource *cone = vtkConeSource::New();
	vtkGlyph3D *glyph = vtkGlyph3D::New();
	glyph->SetInputConnection(sphere->GetOutputPort());
	glyph->SetSourceConnection(cone->GetOutputPort());
	glyph->SetVectorModeToUseNormal();
	glyph->SetScaleModeToScaleByVector();
	glyph->SetScaleFactor(0.25);
	vtkPolyDataMapper *spikeMapper = vtkPolyDataMapper::New();
	spikeMapper->SetInputConnection(glyph->GetOutputPort());
	vtkLODActor *spikeActor = vtkLODActor::New();
	spikeActor->SetMapper(spikeMapper);

	picker = vtkCellPicker::New();
	vtkSmartPointer<vtkCallbackCommand> annotatePick = vtkSmartPointer<vtkCallbackCommand>::New();
	annotatePick->SetCallback(myCallbackFunction);
	annotatePick->SetClientData(picker);
	picker->AddObserver(vtkCommand::EndPickEvent, annotatePick);

	textMapper = vtkTextMapper::New();
	vtkTextProperty *tprop = textMapper->GetTextProperty();
	tprop->SetFontFamilyToArial();
	tprop->SetFontSize(10);
	tprop->BoldOn();
	tprop->ShadowOn();
	tprop->SetColor(1, 0, 0);
	textActor = vtkActor2D::New();
	textActor->VisibilityOff();
	textActor->SetMapper(textMapper);

	vtkRenderer *ren1 = vtkRenderer::New();
	renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	iren->SetPicker(picker);

	ren1->AddActor2D(textActor);
	ren1->AddActor(sphereActor);
	ren1->AddActor(spikeActor);
	ren1->SetBackground(1, 1, 1);
	renWin->SetSize(500, 300);

	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.5);
	
	iren->Initialize();

	iren->Start();

	picker->Pick(85, 126, 0, ren1);


}
*/

#include "vtkAutoInit.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkLODActor.h"
#include "vtkConeSource.h"
#include "vtkGlyph3D.h"
#include "vtkCallbackCommand.h"
#include "vtkCommand.h"
#include "vtkCellPicker.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
#include "vtkActor2D.h"
#include "vtkCamera.h"
#include "vtkAbstractPicker.h"
#include "vtkPoints.h"
#include "vtkObject.h"

/*
static vtkSmartPointer<vtkCellPicker> picker1;
static vtkSmartPointer<vtkTextMapper> textMapper;
static vtkSmartPointer<vtkActor2D> textActor;
static vtkSmartPointer<vtkPoints> pickPos;
static vtkSmartPointer<vtkRenderWindow> renWin;
static double *selPt;
static double x;
static double y;
static double xp;
static double yp;
static double zp;

void myCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData)
{
	picker1 = (vtkCellPicker*) clientData;
	if (picker1->GetCellId() < 0) {
		textActor->VisibilityOff();
	} else {
		selPt = picker1->GetSelectionPoint();
		x = *selPt;
		y = *(selPt + 1);
		pickPos = picker1->GetPickedPositions();
		xp = *(pickPos->GetPoint(0));
		yp = *(pickPos->GetPoint(0) + 1);
		zp = *(pickPos->GetPoint(0) + 2);
		char str[50];
		sprintf_s(str, "%f,%f,%f", xp, yp, zp);
		textMapper->SetInput(str);
		textActor->SetPosition(x, y);
		textActor->VisibilityOn();
	}

	renWin->Render();
}

void Picking48()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);

	//# create a sphere source, mapper, and actor
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection(sphere->GetOutputPort());
	sphereMapper->GlobalImmediateModeRenderingOn();
	vtkSmartPointer<vtkLODActor> sphereActor = vtkSmartPointer<vtkLODActor>::New();
	sphereActor->SetMapper(sphereMapper);

	//# create the spikes by glyphing the sphere with a cone.Create the mapper
	//# and actor for the glyphs.
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
	glyph->SetInputConnection(sphere->GetOutputPort());
	glyph->SetSourceConnection(cone->GetOutputPort());
	glyph->SetVectorModeToUseNormal();
	glyph->SetScaleModeToScaleByVector();
	glyph->SetScaleFactor(0.25);
	vtkSmartPointer<vtkPolyDataMapper> spikeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	spikeMapper->SetInputConnection(glyph->GetOutputPort());
	vtkSmartPointer<vtkLODActor> spikeActor = vtkSmartPointer<vtkLODActor>::New();
	spikeActor->SetMapper(spikeMapper);

	//# Create a cell picker.
	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	vtkSmartPointer<vtkCallbackCommand> annotatePick = vtkSmartPointer<vtkCallbackCommand>::New();
	annotatePick->SetCallback(myCallbackFunction);
	annotatePick->SetClientData(picker);
	picker->AddObserver(vtkCommand::EndPickEvent, annotatePick);

	//# Create a text mapper and actor to display the results of picking.
	textMapper = vtkSmartPointer<vtkTextMapper>::New();
	textMapper->GetTextProperty()->SetFontFamilyToArial();
	textMapper->GetTextProperty()->SetFontSize(10);
	textMapper->GetTextProperty()->BoldOn();
	textMapper->GetTextProperty()->ShadowOn();
	textMapper->GetTextProperty()->SetColor(1, 0, 0);
	textActor = vtkSmartPointer<vtkActor2D>::New();
	textActor->VisibilityOff();
	textActor->SetMapper(textMapper);

	//Create the Renderer, RenderWindow, RenderWindowInteractor
	vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
	renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(ren1);
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	iren->SetPicker(picker);

	//Add the actors to the renderer, set the background and size
	ren1->AddActor2D(textActor);
	ren1->AddActor(sphereActor);
	ren1->AddActor(spikeActor);
	ren1->SetBackground(1, 1, 1);
	renWin->SetSize(300, 300);
	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.4);

	//# Pick the cell at this location.
	picker->Pick(85.0, 126.0, 0, ren1);

	iren->Start();

}
*/

#include "vtkTextActor.h"
#include "vtkTextRenderer.h"

void TextAnnotation4_11()
{
	VTK_MODULE_INIT(vtkRenderingFreeType);

	vtkSphereSource *sphere = vtkSphereSource::New();
	vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
	sphereMapper->SetInputConnection(sphere->GetOutputPort());

	vtkLODActor *sphereActor = vtkLODActor::New();
	sphereActor->SetMapper(sphereMapper);

	vtkTextActor *textActor = vtkTextActor::New();
	textActor->SetTextScaleModeToProp();
	textActor->SetDisplayPosition(100, 50);
	textActor->SetInput("I am a sphere!");

	textActor->GetPosition2Coordinate()
		->SetCoordinateSystemToNormalizedViewport();
	textActor->GetPosition2Coordinate()
		->SetValue(0.6, 0.1);

	auto tprop = textActor->GetTextProperty();
	tprop->SetFontSize(18);
	tprop->SetFontFamilyToArial();
	tprop->SetJustificationToCentered();
	tprop->BoldOn();
	tprop->ShadowOn();
	tprop->SetColor(0, 0, 1);

	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	ren1->AddActor(sphereActor);

	ren1->AddViewProp(textActor);
 	ren1->AddViewProp(sphereActor);

	ren1->SetBackground(1, 1, 1);
	renWin->SetSize(250, 125);
	ren1->ResetCamera();
	ren1->GetActiveCamera()->Zoom(1.5);
	renWin->Render();

	iren->Start();



}