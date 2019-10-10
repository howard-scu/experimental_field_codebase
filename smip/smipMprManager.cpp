#include "smipMprManager.h"
#include <vtkObjectFactory.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(smipMprManager);

smipMprManager::smipMprManager()
{
}


smipMprManager::~smipMprManager()
{
}

void smipMprManager::Execute(vtkObject * caller, unsigned long eventId, void * callData)
{
	auto style = static_cast<vtkInteractorStyleImage*>(caller);
	auto interactor = style->GetInteractor();
	auto id = [=]()->int {
		for (int i = 0; i < 3; i++)
		{
			if (interactor == _viewers[i]->GetInteractor())
				return i;
		}
		return 0;
	}();

	switch (eventId)
	{
	case vtkCommand::LeftButtonPressEvent:
	{
		auto res = _pointPicker[id]->Pick(interactor->GetEventPosition()[0],
			interactor->GetEventPosition()[1], 0.0, _viewers[id]->GetRenderer());

		_xy[0] = interactor->GetEventPosition()[0];
		_xy[1] = interactor->GetEventPosition()[1];

		_level = _viewers[id]->GetColorLevel();
		_window = _viewers[id]->GetColorWindow();

		if (res)
		{
			_positionFlag = true;
		}
		else
		{
			_wlFlag = true;
		}
		break;
	}
	case vtkCommand::LeftButtonReleaseEvent:
	{
		_positionFlag = false;
		_wlFlag = false;
		break;
	}
	case vtkCommand::MouseMoveEvent:
	{
		if (_positionFlag)
		{
			auto res = _imagePicker[id]->Pick(interactor->GetEventPosition()[0],
				interactor->GetEventPosition()[1], 0.0, _viewers[id]->GetRenderer());
			if (res)
			{
				_imagePicker[id]->GetPickPosition(_position);

				int x = (_position[0] - _origins[0]) / _spacings[0];
				int y = (_position[1] - _origins[1]) / _spacings[1];
				int z = (_position[2] - _origins[2]) / _spacings[2];
				_viewers[0]->SetSlice(x);
				_viewers[1]->SetSlice(y);
				_viewers[2]->SetSlice(z);
				for (int i = 0; i < 3; i++)
				{
					_pointActor[i]->SetPosition(_position);
					_viewers[i]->GetInteractor()->Render();
				}
			}
		}
		if (_wlFlag)
		{
			auto dx = interactor->GetEventPosition()[0] - _xy[0];
			auto dy = interactor->GetEventPosition()[1] - _xy[1];
			for (int i = 0; i < 3; i++)
			{
				_viewers[i]->SetColorLevel(_level + dx);
				_viewers[i]->SetColorWindow(_window + 4 * dy);
				_viewers[i]->GetInteractor()->Render();
			}
		}
		break;
	}
	case vtkCommand::MouseWheelBackwardEvent:
	{
		if (_viewers[id]->GetSlice() == _viewers[id]->GetSliceMin())
			return;

		_position[id] -= _spacings[id];
		_viewers[id]->SetSlice(_viewers[id]->GetSlice() - 1);

		for (int i = 0; i < 3; i++)
		{
			_pointActor[i]->SetPosition(_position);
			_viewers[i]->GetInteractor()->Render();
		}
		break;
	}
	case vtkCommand::MouseWheelForwardEvent:
	{
		if (_viewers[id]->GetSlice() == _viewers[id]->GetSliceMax())
			return;
		_position[id] += _spacings[id];
		_viewers[id]->SetSlice(_viewers[id]->GetSlice() + 1);

		for (int i = 0; i < 3; i++)
		{
			_pointActor[i]->SetPosition(_position);
			_viewers[i]->GetInteractor()->Render();
		}
		break;
	}
	}
}

//void smipMprManager::Pan()
//{
//	if (this->CurrentRenderer == nullptr)
//	{
//		return;
//	}
//
//	vtkRenderWindowInteractor *rwi = this->Interactor;
//
//	double viewFocus[4], focalDepth, viewPoint[3];
//	double newPickPoint[4], oldPickPoint[4], motionVector[3];
//
//	// Calculate the focal depth since we'll be using it a lot
//
//	vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
//	camera->GetFocalPoint(viewFocus);
//	this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2],
//		viewFocus);
//	focalDepth = viewFocus[2];
//
//	this->ComputeDisplayToWorld(rwi->GetEventPosition()[0],
//		rwi->GetEventPosition()[1],
//		focalDepth,
//		newPickPoint);
//
//	// Has to recalc old mouse point since the viewport has moved,
//	// so can't move it outside the loop
//
//	this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0],
//		rwi->GetLastEventPosition()[1],
//		focalDepth,
//		oldPickPoint);
//
//	// Camera motion is reversed
//
//	motionVector[0] = oldPickPoint[0] - newPickPoint[0];
//	motionVector[1] = oldPickPoint[1] - newPickPoint[1];
//	motionVector[2] = oldPickPoint[2] - newPickPoint[2];
//
//	camera->GetFocalPoint(viewFocus);
//	camera->GetPosition(viewPoint);
//	camera->SetFocalPoint(motionVector[0] + viewFocus[0],
//		motionVector[1] + viewFocus[1],
//		motionVector[2] + viewFocus[2]);
//
//	camera->SetPosition(motionVector[0] + viewPoint[0],
//		motionVector[1] + viewPoint[1],
//		motionVector[2] + viewPoint[2]);
//
//	if (rwi->GetLightFollowCamera())
//	{
//		this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
//	}
//
//	rwi->Render();
//}

