#ifndef _SMIP_VR_H
#define _SMIP_VR_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkOrientationMarkerWidget.h>

class smipVr
{
public:
	smipVr();
	~smipVr();

	void SetInput(vtkSmartPointer<vtkImageData> input);
	void SetValue(int value);
	void Render();
	void Start();
	void SetAxesOn(bool flag);
private:
	void CreateVolume();
	void UpdateVolume();
	void CreateAxesActor();

	vtkSmartPointer<vtkRenderer>					_renderer;
	vtkSmartPointer<vtkRenderWindow>				_window;
	vtkSmartPointer<vtkRenderWindowInteractor>		_interactor;
	vtkSmartPointer<vtkInteractorStyleSwitch>		_style;
	vtkSmartPointer<vtkImageData>					_input;
	vtkSmartPointer<vtkCamera>						_camera;
	vtkSmartPointer<vtkColorTransferFunction>		_colorFunc;
	vtkSmartPointer<vtkPiecewiseFunction>			_opacityFunc;
	vtkSmartPointer<vtkSmartVolumeMapper>			_mapper;
	vtkSmartPointer<vtkVolumeProperty>				_property;
	vtkSmartPointer<vtkVolume>						_volume;
	vtkSmartPointer<vtkAnnotatedCubeActor>			_axesActor;
	vtkSmartPointer<vtkOrientationMarkerWidget>		_axesWidget;

	int		_threshold{ 2000 };
};

#endif // !_SMIP_VR_H
