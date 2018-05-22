// TestVTKDemo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestVTKDemo.h"
#include "Source3.h"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCommand.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "Source41.h"
#include "LoadCADPart.h"
#include "FilteringData.h"
#include "DataAssembly.h"
#include "Volumes3D.h"
#include "TestText4_11.h"
#include "DrawingClass4_12.h"
#include "DataManipulation4_13.h"
#include "DicomCTRender.h"
#include "ImplictPlaneWidgetDemo4_13.h"
#include "AntiAliasing4_14.h"
#include "PointDataAndCellDataConvert5_1.h"
#include "ElevationBandsWithGlyphs.h"
#include "PolyDataVisuallization.h"
#include "Contours2D6_1.h"

LRESULT				funcAssetCallback(HWND hwnd);

LRESULT runMyFunc(HWND hWnd)
{
	// 	if (nullptr == theVTKApp) {
	// 		theVTKApp = new MyVTKApp(hWnd);
	// 	}

	// 	funcAssetCallback(hWnd);
	//	CreateSimpleModel41(hWnd);
	// 	LoadCADParts();
	//FilterData43();
	//Assembly46();
	//Volumes3D_46();
	//Texture47();
	//Picking48();
	//TextAnnotation4_11();
	//TextAnnotationMultiLine4_11();
	//TextAnnotation3D4_11();
	//XYPlotActor();
	//CubeAxes();
	//LabeledMesh();
	//matching();
	//renderDicomCTDemo();	//this demo can show a volume model
	//ImplictPlaneWidgetDemo4_15();
	//AntiAliasing4_14();
	//deepthPeeling();
	//animationCueDemo();
	//PointDataAndCellDataConvert5_1();
	//RainbowRender5_1();
	//VisQuad5_1();
	//SpikeF5_1();
	//ElevationBandsWithGlyphs();
	//ProbeCutting5_1();
	//DataMerging5_1();
	//ProbeComb5_1();
	//ExtractGeometry5_1();
	//CreateCube5_2();
	//DecimationVtk5_2();
	//DecimationStl5_2();	//this show how to decimate a stl model (very fast);
	//SmoothMesh5_2();
	//SmoothMeshSTL5_2();
	//ClipData5_2(); 
	//ClipDataSTL5_2();
	Contours2D6_1();

	return S_OK;
}

#define MAX_LOADSTRING 100

class vtkMyCallback : public vtkCommand {
public:
	static vtkMyCallback *New()
	{ return new vtkMyCallback; }
	virtual void Execute(vtkObject* caller, unsigned long, void *)
	{
		vtkRenderer *renderer = reinterpret_cast<vtkRenderer*>(caller);
		cout << renderer->GetActiveCamera()->GetPosition()[0] << " "
			<< renderer->GetActiveCamera()->GetPosition()[0] << " "
			<< renderer->GetActiveCamera()->GetPosition()[0] << "\n";
		if (renderer) {
			renderer->SetBackground(0.2, 0.3, 0.4);
		}
	}
};

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


static MyVTKApp		*theVTKApp = nullptr;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTVTKDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTVTKDEMO));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTVTKDEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTVTKDEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_RUNFUNC:
			runMyFunc(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		switch (wmEvent) {
			case 2:
				PostQuitMessage(0);
				if (nullptr != theVTKApp) {
					delete theVTKApp;
					theVTKApp = nullptr;
				}
				return 0;
				break;
			default:
				break;
		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		if (nullptr != theVTKApp) {
			delete theVTKApp;
			theVTKApp = nullptr;
		}
		return 0;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


LRESULT funcAssetCallback(HWND)
{
	vtkConeSource *cone = vtkConeSource::New();
// 	cone->SetHeight(3.0);
	cone->SetRadius(1.0);
	cone->SetResolution(10);

	vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
	coneMapper->SetInputConnection(cone->GetOutputPort());
	vtkActor *coneActor = vtkActor::New();
	coneActor->SetMapper(coneMapper);

	vtkRenderer *ren1 = vtkRenderer::New();
	ren1->AddActor(coneActor);
	ren1->SetBackground(0.1, 0.2, 0.4);
	ren1->ResetCamera();

	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(300, 300);

	// Here is where we setup the observer, we do a new and ren1 will
	// eventually free the observer
	vtkMyCallback *mo1 = vtkMyCallback::New();
	ren1->AddObserver(vtkCommand::StartEvent, mo1);

	//
	// now we loop over 360 degrees and render the cone each time
	//
	int i;
	for (i = 0; i < 3600; ++i) {
		// render the image
		renWin->Render();
		// rotate the active camera by one degree
		ren1->GetActiveCamera()->Azimuth(1);
	}

	//
	// Free up any objects we created
	//
	mo1->Delete();
	cone->Delete();
	coneMapper->Delete();
	coneActor->Delete();
	ren1->Delete();
	renWin->Delete();
	return S_OK;
}