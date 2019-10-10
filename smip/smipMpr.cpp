#include "smipMpr.h"

#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkCamera.h>


smipMpr::smipMpr()
{
}


smipMpr::~smipMpr()
{
}

void smipMpr::UpdatePosition(double * position)
{
	int x = (position[0] - _origins[0]) / _spacings[0];
	int y = (position[1] - _origins[1]) / _spacings[1];
	int z = (position[2] - _origins[2]) / _spacings[2];
	_viewer[0]->SetSlice(x);
	_viewer[1]->SetSlice(y);
	_viewer[2]->SetSlice(z);
	for (int i = 0; i < 3; i++)
	{
		_pointActor[i]->SetPosition(_position);
		_viewer[i]->GetInteractor()->Render();
	}
}

double * smipMpr::GetPosition()
{
	return _mprManager->GetPosition();
}

void smipMpr::Render()
{
	Initialize();
	for (int i = 0; i < 3; i++)
	{
		//_viewer[i]->SetInputData(_input);
		_window[i]->Render();;
	}
}

void smipMpr::Start()
{
	_interactor[2]->Start();
}

void smipMpr::Initialize()
{
	UpdateImageInfos();

	for (int i = 0; i < 3; i++)
	{
		_window[i] = vtkSmartPointer<vtkRenderWindow>::New();
		_window[i]->SetSize(_width, _height);
		
		// WARNING: interactor顺序非常关键
		_style[i] = vtkSmartPointer<vtkInteractorStyleImage>::New();
		_interactor[i] = vtkSmartPointer<vtkRenderWindowInteractor>::New();

		_viewer[i] = vtkSmartPointer<vtkResliceImageViewer2>::New();
		_viewer[i]->SetRenderWindow(_window[i]);
		_viewer[i]->SetupInteractor(_interactor[i]);
		_viewer[i]->SetInputData(_input);
		_viewer[i]->SetSliceOrientation(i);
		_viewer[i]->SetSlice(_dims[i] / 2);
		_viewer[i]->SetResliceModeToAxisAligned();
		_viewer[i]->SetSliceScrollOnMouseWheel(false);
		_viewer[i]->SetColorLevel(1500);
		_viewer[i]->SetColorWindow(3000);
		_viewer[i]->GetRenderer()->AddActor(_pointActor[i]);

		_imagePicker[i] = vtkSmartPointer<vtkPointPicker>::New();
		_imagePicker[i]->PickFromListOn();
		_imagePicker[i]->AddPickList(_viewer[i]->GetImageActor());

		_pointPicker[i] = vtkSmartPointer<vtkPointPicker>::New();
		_pointPicker[i]->PickFromListOn();
		_pointPicker[i]->AddPickList(_pointActor[i]);

		_viewer[i]->GetRenderer()->ResetCamera();
		_viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelProjection(true);
		if (i == 1)
		{
			auto scale1 = (_dims[0] * _spacings[0]) / _width;		// x轴比例
			auto scale2 = (_dims[2] * _spacings[2]) / _height;		// y轴比例
			auto max_scale = scale1 > scale2 ? scale1 : scale2;
			_viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelScale(max_scale * _height / 2.0);
		}
		else if (i == 0)
		{
			auto scale1 = (_dims[1] * _spacings[1]) / _width;
			auto scale2 = (_dims[2] * _spacings[2]) / _height;
			auto max_scale = scale1 > scale2 ? scale1 : scale2;
			_viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelScale(max_scale * _height / 2.0);
		}
		else if (i == 2)
		{
			auto scale1 = (_dims[0] * _spacings[0]) / _width;
			auto scale2 = (_dims[1] * _spacings[1]) / _height;
			auto max_scale = scale1 > scale2 ? scale1 : scale2;
			_viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelScale(max_scale * _height / 2.0);
		}
	}

	_mprManager = vtkSmartPointer<smipMprManager>::New();
	_mprManager->SetViewers(_viewer);
	_mprManager->SetImagePicker(_imagePicker);
	_mprManager->SetPointPicker(_pointPicker);
	_mprManager->SetPointActor(_pointActor);
	_mprManager->SetImage(_input);
	for (int i = 0; i < 3; i++)
	{
		_position[i] = _origins[i] + _dims[i] / 2 * _spacings[i];
	}
	for (int i = 0; i < 3; i++)
	{
		_pointActor[i]->SetPosition(_position);
	}
	_mprManager->SetPosition(_position);
	for (int i = 0; i < 3; i++)
	{
		_interactor[i]->SetInteractorStyle(_style[i]);

		_style[i]->AddObserver(vtkCommand::MouseWheelBackwardEvent, _mprManager);
		_style[i]->AddObserver(vtkCommand::MouseWheelForwardEvent, _mprManager);
		_style[i]->AddObserver(vtkCommand::LeftButtonPressEvent, _mprManager);
		_style[i]->AddObserver(vtkCommand::MouseMoveEvent, _mprManager);
		_style[i]->AddObserver(vtkCommand::LeftButtonReleaseEvent, _mprManager);
	}
}

void smipMpr::UpdateImageInfos()
{
	if (_input == nullptr) return;
	_dims = _input->GetDimensions();
	_extents = _input->GetExtent();
	_spacings = _input->GetSpacing();
	_origins = _input->GetOrigin();

	for (int i = 0; i < 3; i++)
	{
		//_position[i] = _origins[i] + (_extents[2 * i] + _extents[2 * i + 1]) / 2.0 *_spacings[i];
		_position[i] = 0.0;
	}

	for (int i = 0; i < 3; i++)
	{
		auto point = vtkSmartPointer<vtkPoints>::New();
		auto pid = point->InsertNextPoint(_position);

		auto vertice = vtkSmartPointer<vtkCellArray>::New();
		vertice->InsertNextCell(1, &pid);

		auto pointData = vtkSmartPointer<vtkPolyData>::New();
		pointData->SetPoints(point);
		pointData->SetVerts(vertice);

		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(pointData);

		_pointActor[i] = vtkSmartPointer<vtkActor>::New();
		_pointActor[i]->SetMapper(mapper);
		_pointActor[i]->GetProperty()->SetPointSize(10.0);
		_pointActor[i]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		_pointActor[i]->GetProperty()->SetRenderPointsAsSpheres(true);
		_pointActor[i]->GetProperty()->SetInterpolationToFlat();
	}
}
