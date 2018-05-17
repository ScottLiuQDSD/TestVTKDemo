#include "stdafx.h"
#include "AntiAliasing4_14.h"

#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"

#include"vtkActor.h"
#include"vtkImageSinusoidSource.h"
#include"vtkImageData.h"
#include"vtkImageDataGeometryFilter.h"
#include"vtkDataSetSurfaceFilter.h"
#include"vtkLookupTable.h"
#include"vtkCamera.h"

#include "vtkAnimationCue.h"
#include "vtkAnimationScene.h"
#include "vtkFloatArray.h"

void AntiAliasing4_14()
{
	vtkRenderWindowInteractor *i = vtkRenderWindowInteractor::New();
	vtkRenderWindow *w = vtkRenderWindow::New();
	i->SetRenderWindow(w);
	w->SetMultiSamples(0); // no multisampling
	w->SetPointSmoothing(1); // point antialiasing
	vtkRenderer *r = vtkRenderer::New();
	w->AddRenderer(r);
	vtkSphereSource *s = vtkSphereSource::New();
	vtkPolyDataMapper *m = vtkPolyDataMapper::New();
	m->SetInputConnection(s->GetOutputPort());
	vtkActor *a = vtkActor::New();
	a->SetMapper(m);
	vtkProperty *p = a->GetProperty();
	if (0) {
		p->SetRepresentationToPoints(); // we want to see points
		p->SetPointSize(2.0); // big enough to notice antialiasing
		p->SetLighting(0); // don't be disturb by shading
		w->SetPointSmoothing(1);
		p->SetRepresentationToPoints();
		p->SetPointSize(2.0);

		w->SetLineSmoothing(1);
		p->SetRepresentationToWireframe();
		p->SetLineWidth(2.0);

		w->SetPolygonSmoothing(1);
		p->SetRepresentationToSurface();
	}
	
	r->AddActor(a);

	i->Start();

	s->Delete();
	m->Delete();
	a->Delete();
	r->Delete();
	w->Delete();
	i->Delete();

}
void deepthPeeling()
{
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->SetMultiSamples(0);
	renWin->SetAlphaBitPlanes(1);
	iren->SetRenderWindow(renWin);
	renWin->Delete();
	vtkRenderer *renderer = vtkRenderer::New();
	renWin->AddRenderer(renderer);
	renderer->Delete();
	renderer->SetUseDepthPeeling(1);
	renderer->SetMaximumNumberOfPeels(200);
	renderer->SetOcclusionRatio(0.1);
	vtkImageSinusoidSource *imageSource = vtkImageSinusoidSource::New();
	imageSource->SetWholeExtent(0, 9, 0, 9, 0, 9);
	imageSource->SetPeriod(5);
	imageSource->Update();
	vtkImageData *image = imageSource->GetOutput();
	double range[2];
	image->GetScalarRange(range);
	vtkDataSetSurfaceFilter *surface = vtkDataSetSurfaceFilter::New();
	surface->SetInputConnection(imageSource->GetOutputPort());
	imageSource->Delete();
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(surface->GetOutputPort());
	surface->Delete();
	vtkLookupTable *lut = vtkLookupTable::New();
	lut->SetTableRange(range);
	lut->SetAlphaRange(0.5, 0.5);
	lut->SetHueRange(0.2, 0.7);
	lut->SetNumberOfTableValues(256);
	lut->Build();
	mapper->SetScalarVisibility(1);
	mapper->SetLookupTable(lut);
	lut->Delete();
	vtkActor *actor = vtkActor::New();
	renderer->AddActor(actor);
	actor->Delete();
	actor->SetMapper(mapper);
	mapper->Delete();
	renderer->SetBackground(0.1, 0.3, 0.0);
	renWin->SetSize(400, 400);
	renWin->Render();
	if (renderer->GetLastRenderingUsedDepthPeeling()) {
		cout << "depth peeling was used" << endl;
	} else {
		cout << "depth peeling was not used (alpha blending instead)" << endl;
	}
	vtkCamera *camera = renderer->GetActiveCamera();
	camera->Azimuth(-40.0);
	camera->Elevation(20.0);
	renWin->Render();
	iren->Start();
}

class vtkCustomAnimationCue : public vtkAnimationCue {
public:
	static vtkCustomAnimationCue* New();
// 	vtkTypeRevisionMacro(vtkCustomAnimationCue, vtkAnimationCue);
	vtkRenderWindow *RenWin;
	vtkSphereSource* Sphere;
protected:
	vtkCustomAnimationCue()
	{
		this->RenWin = 0;
		this->Sphere = 0;
	}
	// Overridden to adjust the sphere's radius depending on the frame we
	// are rendering. In this animation we want to change the StartTheta
	// of the sphere from 0 to 180 over the length of the cue.
	virtual void TickInternal(double currenttime, double deltatime,
							  double clocktime)
	{
		double new_st = currenttime * 180;
		// since the cue is in normalized mode, the currentime will be in the
		// range [0,1], where 0 is start of the cue and 1 is end of the cue.
		this->Sphere->SetStartTheta(new_st);
		this->RenWin->Render();
	}
};
vtkStandardNewMacro(vtkCustomAnimationCue);
// vtkCxxRevisionMacro(vtkCustomAnimationCue, "$Revision$");

void animationCueDemo()
{
	vtkFloatArray *scalars = vtkFloatArray::New();
	scalars->InsertTuple1(0, 1.0);
	scalars->InsertTuple1(1, 1.2);

	// Create the graphics structure. The renderer renders into the
	// render window.
	vtkRenderer *ren1 = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->SetMultiSamples(0);
	renWin->AddRenderer(ren1);
	vtkSphereSource* sphere = vtkSphereSource::New();
	vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(sphere->GetOutputPort());
	vtkActor* actor = vtkActor::New();
	actor->SetMapper(mapper);
	ren1->AddActor(actor);
	ren1->ResetCamera();
	renWin->Render();
	// Create an Animation Scene
	vtkAnimationScene *scene = vtkAnimationScene::New();
	scene->SetModeToSequence();
	scene->SetFrameRate(30);
	scene->SetStartTime(0);
	scene->SetEndTime(60);
	// Create an Animation Cue to animate the camera.
	vtkCustomAnimationCue *cue1 = vtkCustomAnimationCue::New();
	cue1->Sphere = sphere;
	cue1->RenWin = renWin;
	cue1->SetTimeModeToNormalized();
	cue1->SetStartTime(0);
	cue1->SetEndTime(2.0);
	scene->AddCue(cue1);
	scene->Play();
	scene->Stop();
	ren1->Delete();
	renWin->Delete();
	scene->Delete();
	cue1->Delete();
}