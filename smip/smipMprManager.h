#ifndef _SMIP_MPR_MANAGER_H
#define _SMIP_MPR_MANAGER_H

#include <vtkCommand.h>
#include <vtkPointPicker.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include "vtkResliceImageViewer2.h"
#include <vtkImageData.h>

class smipMprManager : public vtkCommand
{
public:
	static smipMprManager* New();
	vtkTypeMacro(smipMprManager, vtkCommand);

	inline void SetViewers(vtkSmartPointer<vtkResliceImageViewer2> viewers[])
	{
		for (int i = 0; i < 3; i++)
			_viewers[i] = viewers[i];
	}

	inline void SetImagePicker(vtkSmartPointer<vtkPointPicker> pickers[])
	{
		for (int i = 0; i < 3; i++)
			_imagePicker[i] = pickers[i];
	}

	inline void SetPointPicker(vtkSmartPointer<vtkPointPicker> pickers[])
	{
		for (int i = 0; i < 3; i++)
			_pointPicker[i] = pickers[i];
	}

	inline void SetPointActor(vtkSmartPointer<vtkActor> actors[])
	{
		for (int i = 0; i < 3; i++)
			_pointActor[i] = actors[i];
	}

	inline void SetPosition(double* position)
	{
		for (int i = 0; i < 3; i++)
			_position[i] = position[i];
	}

	inline void SetImage(vtkSmartPointer<vtkImageData> image)
	{
		_image = image;
		_dims = _image->GetDimensions();
		_extents = _image->GetExtent();
		_spacings = _image->GetSpacing();
		_origins = _image->GetOrigin();
	}

	inline double* GetPosition()
	{
		return _position;
	}

protected:
	smipMprManager();
	~smipMprManager();

	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData);

	//void Pan();
	//void Zoom();
private:
	smipMprManager(const smipMprManager&) = delete;
	void operator=(const smipMprManager&) = delete;
	vtkSmartPointer<vtkResliceImageViewer2>			_viewers[3];
	vtkSmartPointer<vtkPointPicker>					_imagePicker[3];
	vtkSmartPointer<vtkPointPicker>					_pointPicker[3];
	vtkSmartPointer<vtkActor>						_pointActor[3];
	vtkSmartPointer<vtkImageData>					_image;

	bool				_positionFlag{ false };
	bool				_wlFlag{ false };
	double				_position[3];
	int*				_dims;
	int*				_extents;
	double*				_origins;
	double*				_spacings;
	int					_xy[2];

	int					_level;
	int					_window;
};

#endif // _SMIP_MPR_MANAGER_H
