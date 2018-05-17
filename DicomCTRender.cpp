#include "stdafx.h"
#include "DicomCTRender.h"
#include "vtkCellPicker.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolume.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkAlgorithm.h"
#include "vtkImageResample.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDICOMImageReader.h"
#include "vtkXMLImageDataReader.h"
#include "vtkMetaImageReader.h"
#include "vtkVolumeMapper.h"
#include "vtkPiecewiseFunction.h"
#include "vtkInteractorObserver.h"
#include "vtkVolumeProperty.h"
#include "vtkImageAlgorithm.h"
#include "vtkImageData.h"
#include "vtkAutoInit.h"

vtkPiecewiseFunction *opacityFun = nullptr;
vtkColorTransferFunction *colorFun = nullptr;
vtkVolumeProperty *property_ = nullptr;
vtkVolume *volume = nullptr;
vtkFixedPointVolumeRayCastMapper *mapper = nullptr;
vtkAlgorithm *reader = nullptr;
vtkImageResample *resample = nullptr;
vtkRenderer *renderer = nullptr;
vtkRenderWindow *renWin4 = nullptr;
vtkRenderWindowInteractor *iren = nullptr;
#define VTI_FILETYPE 1
#define MHA_FILETYPE 2

void renderDicomCTDemo()
{
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkRenderingFreeType);/// need to init this 
	VTK_MODULE_INIT(vtkInteractionStyle);
	VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);//vtkRayCastImageDisplayHelper

	int count = 0;
	char *dirname = NULL;
	double opacityWindow = 4096;
	double opacityLevel = 2048;
	int blendType = 0;
	int clip = 0;
	double reductionFactor = 1.0;
	double frameRate = 10.0;
	char *fileName = 0;
	int fileType = 0;

	bool independentComponents = true;
	int argc = 6;
	char* argv[7] = 
	{
		{"-DICOM"},
		{"G:/CBCT/Newfolder/¡ı”Ò¡˙__1980_12_30"},
		{"-CT_Bone"},
		{"-Clip"},
		{"-FrameRate"},
		{"15"},
		{""}
	};

	while (count < argc) {
		if (!strcmp(argv[count], "-DICOM")) {
			size_t size = strlen(argv[count + 1]) + 1;
			dirname = new char[size];
			snprintf(dirname, size, "%s", argv[count + 1]);
			count += 2;
		} else if (!strcmp(argv[count], "-VTI")) {
			size_t size = strlen(argv[count + 1]) + 1;
			fileName = new char[size];
			fileType = VTI_FILETYPE;
			snprintf(fileName, size, "%s", argv[count + 1]);
			count += 2;
		} else if (!strcmp(argv[count], "-MHA")) {
			size_t size = strlen(argv[count + 1]) + 1;
			fileName = new char[size];
			fileType = MHA_FILETYPE;
			snprintf(fileName, size, "%s", argv[count + 1]);
			count += 2;
		} else if (!strcmp(argv[count], "-Clip")) {
			clip = 1;
			count++;
		} else if (!strcmp(argv[count], "-MIP")) {
			opacityWindow = atof(argv[count + 1]);
			opacityLevel = atof(argv[count + 2]);
			blendType = 0;
			count += 3;
		} else if (!strcmp(argv[count], "-CompositeRamp")) {
			opacityWindow = atof(argv[count + 1]);
			opacityLevel = atof(argv[count + 2]);
			blendType = 1;
			count += 3;
		} else if (!strcmp(argv[count], "-CompositeShadeRamp")) {
			opacityWindow = atof(argv[count + 1]);
			opacityLevel = atof(argv[count + 2]);
			blendType = 2;
			count += 3;
		} else if (!strcmp(argv[count], "-CT_Skin")) {
			blendType = 3;
			count += 1;
		} else if (!strcmp(argv[count], "-CT_Bone")) {
			blendType = 4;
			count += 1;
		} else if (!strcmp(argv[count], "-CT_Muscle")) {
			blendType = 5;
			count += 1;
		} else if (!strcmp(argv[count], "-RGB_Composite")) {
			blendType = 6;
			count += 1;
		} else if (!strcmp(argv[count], "-FrameRate")) {
			frameRate = atof(argv[count + 1]);
			if (frameRate < 0.01 || frameRate > 60.0) {
				cout << "Invalid frame rate - use a number between 0.01 and 60.0" << endl;
				cout << "Using default frame rate of 10 frames per second." << endl;
				frameRate = 10.0;
			}
			count += 2;
		} else if (!strcmp(argv[count], "-ReductionFactor")) {
			reductionFactor = atof(argv[count + 1]);
			if (reductionFactor <= 0.0 || reductionFactor >= 1.0) {
				cout << "Invalid reduction factor - use a number between 0 and 1 (exclusive)" << endl;
				cout << "Using the default of no reduction." << endl;
				reductionFactor = 1.0;
			}
			count += 2;
		} else if (!strcmp(argv[count], "-DependentComponents")) {
			independentComponents = false;
			count += 1;
		} else {
			cout << "Unrecognized option: " << argv[count] << endl;
			cout << endl;
// 			PrintUsage();
			exit(EXIT_FAILURE);
		}
	}

	if (!dirname && !fileName) {
		cout << "Error: you must specify a directory of DICOM data or a .vti file or a .mha!" << endl;
		cout << endl;
// 		PrintUsage();
		exit(EXIT_FAILURE);
	}

	// Create the renderer, render window and interactor
	renderer = vtkRenderer::New();
	renWin4 = vtkRenderWindow::New();
	renWin4->AddRenderer(renderer);

	// Connect it all. Note that funny arithematic on the
	// SetDesiredUpdateRate - the vtkRenderWindow divides it
	// allocated time across all renderers, and the renderer
	// divides it time across all props. If clip is
	// true then there are two props
	iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin4);
	iren->SetDesiredUpdateRate(frameRate / (1 + clip));

	iren->GetInteractorStyle()->SetDefaultRenderer(renderer);

	// Read the data
	reader = 0;
	vtkImageData *input = 0;
	if (dirname) {
		vtkDICOMImageReader *dicomReader = vtkDICOMImageReader::New();
		dicomReader->SetDirectoryName(dirname);
		dicomReader->Update();
		input = dicomReader->GetOutput();
		reader = dicomReader;
	} else if (fileType == VTI_FILETYPE) {
		vtkXMLImageDataReader *xmlReader = vtkXMLImageDataReader::New();
		xmlReader->SetFileName(fileName);
		xmlReader->Update();
		input = xmlReader->GetOutput();
		reader = xmlReader;
	} else if (fileType == MHA_FILETYPE) {
		vtkMetaImageReader *metaReader = vtkMetaImageReader::New();
		metaReader->SetFileName(fileName);
		metaReader->Update();
		input = metaReader->GetOutput();
		reader = metaReader;
	} else {
		cout << "Error! Not VTI or MHA!" << endl;
		exit(EXIT_FAILURE);
	}

	// Verify that we actually have a volume
	int dim[3];
	input->GetDimensions(dim);
	if (dim[0] < 2 ||
		dim[1] < 2 ||
		dim[2] < 2) {
		cout << "Error loading data!" << endl;
		exit(EXIT_FAILURE);
	}

	resample = vtkImageResample::New();
	if (reductionFactor < 1.0) {
		resample->SetInputConnection(reader->GetOutputPort());
		resample->SetAxisMagnificationFactor(0, reductionFactor);
		resample->SetAxisMagnificationFactor(1, reductionFactor);
		resample->SetAxisMagnificationFactor(2, reductionFactor);
	}

	// Create our volume and mapper
	volume = vtkVolume::New();
	mapper = vtkFixedPointVolumeRayCastMapper::New();

	if (reductionFactor < 1.0) {
		mapper->SetInputConnection(resample->GetOutputPort());
	} else {
		mapper->SetInputConnection(reader->GetOutputPort());
	}


	// Set the sample distance on the ray to be 1/2 the average spacing
	double spacing[3];
	if (reductionFactor < 1.0) {
		resample->GetOutput()->GetSpacing(spacing);
	} else {
		input->GetSpacing(spacing);
	}

	//  mapper->SetSampleDistance( (spacing[0]+spacing[1]+spacing[2])/6.0 );
	//  mapper->SetMaximumImageSampleDistance(10.0);


	// Create our transfer function
	colorFun = vtkColorTransferFunction::New();
	opacityFun = vtkPiecewiseFunction::New();

	// Create the property_ and attach the transfer functions
	property_ = vtkVolumeProperty::New();
	property_->SetIndependentComponents(independentComponents);
	property_->SetColor(colorFun);
	property_->SetScalarOpacity(opacityFun);
	property_->SetInterpolationTypeToLinear();

	// connect up the volume to the property_ and the mapper
	volume->SetProperty(property_);
	volume->SetMapper(mapper);

	// Depending on the blend type selected as a command line option,
	// adjust the transfer function
	switch (blendType) {
		// MIP
		// Create an opacity ramp from the window and level values.
		// Color is white. Blending is MIP.
		case 0:
			colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0);
			opacityFun->AddSegment(opacityLevel - 0.5*opacityWindow, 0.0,
								   opacityLevel + 0.5*opacityWindow, 1.0);
			mapper->SetBlendModeToMaximumIntensity();
			break;

			// CompositeRamp
			// Create a ramp from the window and level values. Use compositing
			// without shading. Color is a ramp from black to white.
		case 1:
			colorFun->AddRGBSegment(opacityLevel - 0.5*opacityWindow, 0.0, 0.0, 0.0,
									opacityLevel + 0.5*opacityWindow, 1.0, 1.0, 1.0);
			opacityFun->AddSegment(opacityLevel - 0.5*opacityWindow, 0.0,
								   opacityLevel + 0.5*opacityWindow, 1.0);
			mapper->SetBlendModeToComposite();
			property_->ShadeOff();
			break;

			// CompositeShadeRamp
			// Create a ramp from the window and level values. Use compositing
			// with shading. Color is white.
		case 2:
			colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0);
			opacityFun->AddSegment(opacityLevel - 0.5*opacityWindow, 0.0,
								   opacityLevel + 0.5*opacityWindow, 1.0);
			mapper->SetBlendModeToComposite();
			property_->ShadeOn();
			break;

			// CT_Skin
			// Use compositing and functions set to highlight skin in CT data
			// Not for use on RGB data
		case 3:
			colorFun->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);
			colorFun->AddRGBPoint(-1000, .62, .36, .18, 0.5, 0.0);
			colorFun->AddRGBPoint(-500, .88, .60, .29, 0.33, 0.45);
			colorFun->AddRGBPoint(3071, .83, .66, 1, 0.5, 0.0);

			opacityFun->AddPoint(-3024, 0, 0.5, 0.0);
			opacityFun->AddPoint(-1000, 0, 0.5, 0.0);
			opacityFun->AddPoint(-500, 1.0, 0.33, 0.45);
			opacityFun->AddPoint(3071, 1.0, 0.5, 0.0);

			mapper->SetBlendModeToComposite();
			property_->ShadeOn();
			property_->SetAmbient(0.1);
			property_->SetDiffuse(0.9);
			property_->SetSpecular(0.2);
			property_->SetSpecularPower(10.0);
			property_->SetScalarOpacityUnitDistance(0.8919);
			break;

			// CT_Bone
			// Use compositing and functions set to highlight bone in CT data
			// Not for use on RGB data
		case 4:
			colorFun->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);
			colorFun->AddRGBPoint(-16, 0.73, 0.25, 0.30, 0.49, .61);
			colorFun->AddRGBPoint(641, .90, .82, .56, .5, 0.0);
			colorFun->AddRGBPoint(3071, 1, 1, 1, .5, 0.0);

			opacityFun->AddPoint(-3024, 0, 0.5, 0.0);
			opacityFun->AddPoint(-16, 0, .49, .61);
			opacityFun->AddPoint(641, .72, .5, 0.0);
			opacityFun->AddPoint(3071, .71, 0.5, 0.0);

			mapper->SetBlendModeToComposite();
			property_->ShadeOn();
			property_->SetAmbient(0.1);
			property_->SetDiffuse(0.9);
			property_->SetSpecular(0.2);
			property_->SetSpecularPower(10.0);
			property_->SetScalarOpacityUnitDistance(0.8919);
			break;

			// CT_Muscle
			// Use compositing and functions set to highlight muscle in CT data
			// Not for use on RGB data
		case 5:
			colorFun->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);
			colorFun->AddRGBPoint(-155, .55, .25, .15, 0.5, .92);
			colorFun->AddRGBPoint(217, .88, .60, .29, 0.33, 0.45);
			colorFun->AddRGBPoint(420, 1, .94, .95, 0.5, 0.0);
			colorFun->AddRGBPoint(3071, .83, .66, 1, 0.5, 0.0);

			opacityFun->AddPoint(-3024, 0, 0.5, 0.0);
			opacityFun->AddPoint(-155, 0, 0.5, 0.92);
			opacityFun->AddPoint(217, .68, 0.33, 0.45);
			opacityFun->AddPoint(420, .83, 0.5, 0.0);
			opacityFun->AddPoint(3071, .80, 0.5, 0.0);

			mapper->SetBlendModeToComposite();
			property_->ShadeOn();
			property_->SetAmbient(0.1);
			property_->SetDiffuse(0.9);
			property_->SetSpecular(0.2);
			property_->SetSpecularPower(10.0);
			property_->SetScalarOpacityUnitDistance(0.8919);
			break;

			// RGB_Composite
			// Use compositing and functions set to highlight red/green/blue regions
			// in RGB data. Not for use on single component data
		case 6:
			opacityFun->AddPoint(0, 0.0);
			opacityFun->AddPoint(5.0, 0.0);
			opacityFun->AddPoint(30.0, 0.05);
			opacityFun->AddPoint(31.0, 0.0);
			opacityFun->AddPoint(90.0, 0.0);
			opacityFun->AddPoint(100.0, 0.3);
			opacityFun->AddPoint(110.0, 0.0);
			opacityFun->AddPoint(190.0, 0.0);
			opacityFun->AddPoint(200.0, 0.4);
			opacityFun->AddPoint(210.0, 0.0);
			opacityFun->AddPoint(245.0, 0.0);
			opacityFun->AddPoint(255.0, 0.5);

			mapper->SetBlendModeToComposite();
			property_->ShadeOff();
			property_->SetScalarOpacityUnitDistance(1.0);
			break;
		default:
			vtkGenericWarningMacro("Unknown blend type.");
			break;
	}

	// Set the default window size
	renWin4->SetSize(600, 600);
	renWin4->Render();

	// Add the volume to the scene
	renderer->AddVolume(volume);

	renderer->ResetCamera();

	// interact with data
	renWin4->Render();

	iren->Start();

}
void releaseRenderDicomCTDemo()
{
	opacityFun->Delete();
	colorFun->Delete();
	property_->Delete();

	volume->Delete();
	mapper->Delete();
	reader->Delete();
	resample->Delete();
	renderer->Delete();
	renWin4->Delete();
	iren->Delete();
}
