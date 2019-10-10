#include "smipVr.h"
#include <vtkProperty.h>
#include <vtkContourValues.h>

smipVr::smipVr()
{
	_renderer = vtkSmartPointer<vtkRenderer>::New();
	_window = vtkSmartPointer<vtkRenderWindow>::New();
	_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	_camera = vtkSmartPointer<vtkCamera>::New();
	_style = vtkSmartPointer<vtkInteractorStyleSwitch>::New();
	_style->SetCurrentStyleToJoystickCamera();

	_renderer->SetActiveCamera(_camera);
	_window->AddRenderer(_renderer);
	_interactor->SetInteractorStyle(_style);
	_window->SetInteractor(_interactor);
	_camera->ParallelProjectionOn();

	CreateAxesActor();
}


smipVr::~smipVr()
{
}

void smipVr::SetInput(vtkSmartPointer<vtkImageData> input)
{
	_input = input;
	CreateVolume();
	_renderer->AddVolume(_volume);
}

void smipVr::SetValue(int value)
{
	_threshold = value;
	UpdateVolume();
	_interactor->Render();
}

void smipVr::Render()
{
	_renderer->ResetCamera();
	_window->Render();
}

void smipVr::Start()
{
	_interactor->Start();
}


void smipVr::CreateVolume()
{
	_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	_mapper->SetInputData(_input);
	_mapper->SetRequestedRenderModeToGPU();
	_mapper->SetBlendModeToIsoSurface();

	_colorFunc = vtkSmartPointer<vtkColorTransferFunction>::New();
	_colorFunc->AddRGBPoint(_threshold, 1.0, 0.5, 0.3);

	_opacityFunc = vtkSmartPointer<vtkPiecewiseFunction>::New();
	_opacityFunc->AddPoint(_threshold, 1.0);

	_property = vtkSmartPointer<vtkVolumeProperty>::New();
	_property->GetIsoSurfaceValues()->SetValue(0, _threshold);
	_property->SetColor(_colorFunc);
	_property->SetScalarOpacity(_opacityFunc);
	_property->SetInterpolationTypeToLinear();
	_property->ShadeOn();
	_property->SetAmbient(0.4);
	_property->SetDiffuse(0.6);
	_property->SetSpecular(0.2);

	_volume = vtkSmartPointer<vtkVolume>::New();
	_volume->SetProperty(_property);
	_volume->SetMapper(_mapper);
}

void smipVr::UpdateVolume()
{
	_colorFunc->RemoveAllPoints();
	_colorFunc->AddRGBPoint(_threshold, 1.0, 0.5, 0.3);

	_opacityFunc->RemoveAllPoints();
	_opacityFunc->AddPoint(_threshold, 1.0);

	_property->GetIsoSurfaceValues()->SetValue(0, _threshold);
	_volume->Update();
}

void smipVr::CreateAxesActor()
{
	_axesActor = vtkSmartPointer<vtkAnnotatedCubeActor>::New();
	_axesActor->SetXMinusFaceText("L");
	_axesActor->SetXPlusFaceText("R");
	_axesActor->SetYMinusFaceText("A");
	_axesActor->SetYPlusFaceText("P");
	_axesActor->SetZMinusFaceText("S");
	_axesActor->SetZPlusFaceText("I");
	_axesActor->GetTextEdgesProperty()->SetColor(0.1, .1, .1);
	_axesActor->GetTextEdgesProperty()->SetLineWidth(1);
	_axesActor->GetCubeProperty()->SetColor(0.7, 0.7, 0.7);

	_axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	_axesWidget->SetOrientationMarker(_axesActor);
}

void smipVr::SetAxesOn(bool flag)
{
	if (flag)
	{
		_axesWidget->SetInteractor(_interactor);
		_axesWidget->EnabledOn();
		_axesWidget->InteractiveOff();
	}
	else
	{
		_axesWidget->SetInteractor(_interactor);
		_axesWidget->EnabledOff();
	}
}
