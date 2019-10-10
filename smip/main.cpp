#include <vtkAutoInit.h>
#include <vtkMetaImageReader.h>
#include <vtkSmartPointer.h>
#include "smipMpr.h"
#include "smipVr.h"

VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType)

void main()
{
	auto reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName("C:\\1.mha");
	reader->Update();

	smipMpr simpleMpr;
	simpleMpr.SetInput(reader->GetOutput());
	simpleMpr.Render();

	smipVr simpleVr;
	simpleVr.SetInput(reader->GetOutput());
	simpleVr.SetValue(1000);
	simpleVr.SetAxesOn(false);
	simpleVr.Render();
	simpleVr.Start();
}