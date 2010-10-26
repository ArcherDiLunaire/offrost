

#include "PluginController.h"

void PluginController::addPlugin(FrostPlugin* plugin){
	plugin->controller = this;
	plugins.push_back(plugin);
}


void PluginController::setup(){
	projectorMaskAlpha = 0;
	projectorMask.loadImage("maskProjectorLiquid.png");
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		plugin->setup(); 
	}
}

void PluginController::update(float mx, float my){
	projectorMaskAlpha = 0;
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		if(plugin->enabled){
			plugin->mouseX = mx;
			plugin->mouseY = my;
			plugin->update(); 
		}
	}
}
void PluginController::draw(){

	glDisable(GL_DEPTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);	
	
	
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
	
	glPopMatrix();
	
}

void PluginController::drawWall(){
	FrostPlugin* plugin = plugins[0];
	plugin->applyWallProjection();
	
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		if(plugin->enabled){
			plugin->drawOnWall(); 
		}
	}
	
	glPopMatrix();
}

void PluginController::drawMasking(){
		for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		// if(plugin->type == FrostPlugin::OUTPUT && plugin->enabled){
		if(plugin->enabled){
			plugin->drawMasking(); 
		}
	}
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255,255*projectorMaskAlpha);
	projectorMask.draw(0,0, ofGetWidth(), ofGetHeight());
}
