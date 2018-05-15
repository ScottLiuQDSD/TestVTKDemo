#ifndef SOURCE3_HEADER_20180510
#define SOURCE3_HEADER_20180510

#include "windows.h"

class vtkRenderWindow;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkConeSource;
class vtkPolyDataMapper;
class vtkActor;


class MyVTKApp
{
public:
	MyVTKApp(HWND parent);
	~MyVTKApp();
private:
	vtkRenderWindow		*renWin;
	vtkRenderer			*renderer;
	vtkRenderWindowInteractor *iren;
	vtkConeSource		*cone;
	vtkPolyDataMapper	*coneMapper;
	vtkActor			*coneActor;

};

#endif // !SOURCE3_HEADER_20180510
