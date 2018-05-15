// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkCamera;
class vtkLight;

struct VTKCommonParam {
	VTKCommonParam() :
		ren1(nullptr),
		renWin(nullptr),
		iren(nullptr),
		camera(nullptr),
		light(nullptr)
	{
	}
	vtkRenderer *ren1;
	vtkRenderWindow *renWin;
	vtkRenderWindowInteractor *iren;
	vtkCamera	*camera;
	vtkLight	*light;
};

// TODO: reference additional headers your program requires here
