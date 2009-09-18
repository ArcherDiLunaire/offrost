#include "Plugin.h"
#include "PluginController.h"
#include "ProjectionSurfaces.h"

FrostPlugin::FrostPlugin(){
	enabled = true;
	dt = 0;
	glDelegate = NULL;
}

void FrostPlugin::applyFloorProjection(){
	(getPlugin<ProjectionSurfaces*>(controller))->applyFloorProjection();
}
