/*
 *  PluginController.cpp
 *  openFrameworks
 *
 *  Created by Jonas Jongejan on 14/09/09.
 *  Copyright 2009 HalfdanJ. All rights reserved.
 *
 */

#include "PluginController.h"

void PluginController::addPlugin(FrostPlugin* plugin){
	plugin->controller = this;
	plugins.push_back(plugin);
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
		if(plugin->type == FrostPlugin::OUTPUT && plugin->enabled){
			plugin->draw(); 
		}
	}
	
}

