

#include "PluginController.h"

void PluginController::addPlugin(FrostPlugin* plugin){
	plugin->controller = this;
	plugins.push_back(plugin);
}


void PluginController::setup(){
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		plugin->setup(); 
	}
}

void PluginController::update(){
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		if(plugin->enabled){
			plugin->update(); 
		}
	}
}
void PluginController::draw(){

	
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		//		if(plugin->type == FrostPlugin::OUTPUT && plugin->enabled){
		if(plugin->enabled){
			plugin->draw(); 
		}
	}
}

void PluginController::drawFloor(){
	FrostPlugin* plugin = plugins[0];
	plugin->applyFloorProjection();
	
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
//		if(plugin->type == FrostPlugin::OUTPUT && plugin->enabled){
		if(plugin->enabled){
			plugin->drawOnFloor(); 
		}
	}
	
}

