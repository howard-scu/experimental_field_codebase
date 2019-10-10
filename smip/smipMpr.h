#ifndef _SMIP_MPR_H
#define _SMIP_MPR_H

#include "smipMprManager.h"
#include <vtkSmartPointer.h>
#include "vtkResliceImageViewer2.h"
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPointPicker.h>
#include <vtkPointPicker.h>
#include <vtkImageData.h>
#include <vtkActor.h>


class smipMpr
{
public:
	smipMpr();
	~smipMpr();

	inline void SetInput(vtkSmartPointer<vtkImageData> input)
	{
		_input = input;
	}
	void	UpdatePosition(double* position);
	double* GetPosition();

	void	Render();
	void	Start();

private:
	void Initialize();
	void UpdateImageInfos();

private:
	vtkSmartPointer<vtkImageData>					_input{ nullptr };
	vtkSmartPointer<vtkResliceImageViewer2>			_viewer[3];
	vtkSmartPointer<vtkRenderWindow>				_window[3];
	vtkSmartPointer<vtkRenderWindowInteractor>		_interactor[3];
	vtkSmartPointer<vtkInteractorStyleImage>		_style[3];
	vtkSmartPointer<vtkPointPicker>					_imagePicker[3];
	vtkSmartPointer<vtkPointPicker>					_pointPicker[3];
	vtkSmartPointer<vtkActor>						_pointActor[3];
	vtkSmartPointer<smipMprManager>					_mprManager;

	int*		_dims;
	int*		_extents;
	double*		_origins;
	double*		_spacings;
	double		_position[3];
	int			_width{600};
	int			_height{400};
};

#endif // _SMIP_MPR_H
