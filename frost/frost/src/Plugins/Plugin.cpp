
#include "Plugin.h"
#include "PluginController.h"
#include "ProjectionSurfaces.h"
#include "BlobTracking.h"

FrostPlugin::FrostPlugin(){
	enabled = true;
	dt = 0;
	glDelegate = NULL;
}

void FrostPlugin::applyFloorProjection(){
	(getPlugin<ProjectionSurfaces*>(controller))->applyFloorProjection();
}

ProjectionSurfaces* FrostPlugin::projection(){	
	return 	(getPlugin<ProjectionSurfaces*>(controller));
}Tracker* FrostPlugin::blob(int n){	
	return 	(getPlugin<BlobTracking*>(controller)->trackers[n]);
}