#include "Plugin.h"
#include "PluginController.h"
#include "ProjectionSurfaces.h"

FrostPlugin::FrostPlugin(){
	enabled = true;
}

void FrostPlugin::applyFloorProjection(){
	(getPlugin<ProjectionSurfaces*>(controller))->applyFloorProjection();
}
