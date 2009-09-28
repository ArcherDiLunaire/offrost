
#include "PluginController.h"
#include "PluginIncludes.h"



Folding::Folding(){
	type = OUTPUT;
	cam = 1;
}

void Folding::setup(){
}

void Folding::update(){
	
}
void Folding::draw(){
	projection()->applyCurtainProjection(0, 0);
	ofSetColor(255, 0, 0);
	ofRect(0, 0, 1, 1);
	
	blob(1)->grayDiff.draw(0,0,1,1);
}

