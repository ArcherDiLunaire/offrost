
#include "ProjectionSurfaces.h"

ProjectionSurfaces::ProjectionSurfaces(){
	type = DATA;
	floorProjection = new Warp();
	for(int i=0;i<3;i++){
		columnProjection[i] = new Warp();
		columnAspect[i] = 1.0;
	}
	for(int i=0;i<6;i++){
		curtainProjection[i] = new Warp();
		curtainAspect[i] = 1.0;
	}
	
	for(int i=0;i<10;i++){
		objects.push_back(new ProjectionSurfacesObject);
		objects[i]->warp = getWarp(i);
	}
	
	
	objects[0]->aspect = &floorAspect;
	objects[0]->name = "FLOOR";
	
	objects[1]->aspect = &columnAspect[0];
	objects[1]->name = "COLUMN 0";
	
	objects[2]->aspect = &columnAspect[1];
	objects[2]->name = "COLUMN 1";
	
	objects[3]->aspect = &columnAspect[2];
	objects[3]->name = "COLUMN 2";
	
	objects[4]->aspect = &curtainAspect[0];
	objects[4]->name = "CURTAIN 0";
	
	objects[5]->aspect = &curtainAspect[1];
	objects[5]->name = "CURTAIN 1";
	
	objects[6]->aspect = &curtainAspect[2];
	objects[6]->name = "CURTAIN 2";
	
	objects[7]->aspect = &curtainAspect[3];
	objects[7]->name = "CURTAIN 3";
	
	objects[8]->aspect = &curtainAspect[4];
	objects[8]->name = "CURTAIN 4";
	
	objects[9]->aspect = &curtainAspect[5];
	objects[9]->name = "CURTAIN 5";
	
	
	drawDebug = false;
	gridFloorResolution = 10;
	selectedCorner = 0;
	floorAspect =  1.0;
	selectedKeystoner = 0;
	
}

void ProjectionSurfaces::guiWakeup(){
	ofAddListener(glDelegate->mousePressed,this, &ProjectionSurfaces::mousePressed);
	ofAddListener(glDelegate->mouseDragged,this, &ProjectionSurfaces::mouseDragged);
	ofAddListener(glDelegate->keyPressed,this, &ProjectionSurfaces::keyPressed);
	
	w = glDelegate->m_Width/3.0;
	h = glDelegate->m_Width/3.0;
	offset = 50;
}


void ProjectionSurfaces::setup(){
	
	verdana.loadFont("verdana.ttf",40);
	
	keystoneXml = new ofxXmlSettings;
	keystoneXml->loadFile("keystoneSettings.xml");
	int numFloor = keystoneXml->getNumTags("floor");
	if(numFloor != 1){
		cout<<"====== ERROR: No floor in keystone xml ======"<<endl;
	} else {
		keystoneXml->pushTag("floor", 0);
		int numCorners = keystoneXml->getNumTags("corner");
		if(numCorners != 4){
			cout<<"====== ERROR: Wrong number of corners i floor ======"<<endl;
		} else {
			for(int i=0;i<4;i++){
				floorProjection->SetCorner( keystoneXml->getAttribute("corner", "number", 0, i) ,  keystoneXml->getAttribute("corner", "x", 0.0, i),  keystoneXml->getAttribute("corner", "y", 0.0, i));
			}
			floorAspect = keystoneXml->getAttribute("aspect", "value", 1.0, 0);
		}
		//floorAspect = keystoneXml->getAttribute("aspect", "val", 1.0, 0);
		
		keystoneXml->popTag();
		
		keystoneXml->pushTag("columns", 0);
		for(int u=0;u<3;u++){
			keystoneXml->pushTag("column", u);
			int numCorners = keystoneXml->getNumTags("corner");
			if(numCorners != 4){
				cout<<"====== ERROR: Wrong number of corners i floor ======"<<endl;
			} else {
				for(int i=0;i<4;i++){
					columnProjection[u]->SetCorner( keystoneXml->getAttribute("corner", "number", 0, i) ,  keystoneXml->getAttribute("corner", "x", 0.0, i),  keystoneXml->getAttribute("corner", "y", 0.0, i));
				}
			}
			columnAspect[u] = keystoneXml->getAttribute("aspect", "value", 1.0, 0);
			
			keystoneXml->popTag();
			
		}
		keystoneXml->popTag();
		
		keystoneXml->pushTag("curtains", 0);
		for(int u=0;u<6;u++){
			keystoneXml->pushTag("curtain", u);
			int numCorners = keystoneXml->getNumTags("corner");
			if(numCorners != 4){
				cout<<"====== ERROR: Wrong number of corners i floor ======"<<endl;
			} else {
				for(int i=0;i<4;i++){
					curtainProjection[u]->SetCorner( keystoneXml->getAttribute("corner", "number", 0, i) ,  keystoneXml->getAttribute("corner", "x", 0.0, i),  keystoneXml->getAttribute("corner", "y", 0.0, i));
				}
			}
			curtainAspect[u] = keystoneXml->getAttribute("aspect", "value", 1.0, 0);			
			keystoneXml->popTag();
			
		}
		keystoneXml->popTag();
		
		
	}
	
	for(int i=0;i<10;i++){
		getWarp(i)->MatrixCalculate();
	}
	
	
}
void ProjectionSurfaces::update(){
}

void ProjectionSurfaces::drawOnFloor(){
}
void ProjectionSurfaces::draw(){
	if(drawDebug){
		for(int i=0;i<10;i++){
			float a = 0.3;
			
			if(selectedKeystoner == i){
				a = 1.0;
			}
			applyProjection(objects[i]);					
			if(i>0&&i<4){
				drawGrid(objects[i]->name, *(objects[i]->aspect), 1.0/ *(objects[i]->aspect), false, a, 0.02);	
				
			} else {
				drawGrid(objects[i]->name, *(objects[i]->aspect), 10, (i==0)?true : false, a, 1.0);	
			}
			
			glPopMatrix();
			
		}
		
	}
}

void ProjectionSurfaces::drawSettings(){
	glPushMatrix();
	glTranslated(offset, offset, 0);
	glPushMatrix();
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255,70);
	ofRect(0, 0, w, h);
	
	glScaled(w, h, 1.0);
	for(int i=0;i<4;i++){
		ofSetColor(255,0, 0);
		if(selectedCorner == i){
			ofSetColor(255,255, 0);
		}
		ofxVec2f v = getWarp(selectedKeystoner)->corners[i];
		ofEllipse(v.x, v.y, 0.05, 0.05);
	}	
	glPopMatrix();
	
	//	drawDebugGrids(w,h);
	
	for(int i=0;i<10;i++){
		float a = 0.3;
		if(selectedKeystoner == i){
			a = 1.0;
		}
		
		applyProjection(objects[i], w, h);	
		
		if(i>0&&i<4){
			drawGrid(objects[i]->name, *(objects[i]->aspect), 1.0/ *(objects[i]->aspect), false, a, 0.02);	
			
		} else {
			drawGrid(objects[i]->name, *(objects[i]->aspect), 10, (i==0)?true : false, a, 1.0);	
		}
		
		glPopMatrix();
		
	}
	
	
	glPopMatrix();
	
}

void ProjectionSurfaces::drawGrid(string text, float aspect, int resolution, bool drawBorder, float alpha, float fontSize){
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255*alpha);
	int xNumber = resolution+floor((aspect-1)*resolution);
	int yNumber = resolution;
	
	for(int i=0;i<=yNumber;i++){
		ofLine(0, i*1.0/resolution, aspect, i*1.0/resolution);
	}
	for(int i=0;i<=xNumber;i++){
		ofLine(i*1.0/resolution, 0, i*1.0/resolution, 1.0);
	}
	if(drawBorder){
		ofNoFill();
		ofSetLineWidth(5);
		
		ofSetColor(255, 0, 255255*alpha);
		ofRect(0, 0, 1*aspect, 1);
		
		ofFill();
		ofSetColor(255, 255, 255,255*alpha);
		ofSetLineWidth(1);
	} else {
		ofLine(aspect, 0, aspect, 1);
	}
	fontSize *= 0.003;
	glScaled(fontSize, fontSize, 1.0);
	//	glTranslated( aspect*0.5*1/0.003-verdana.stringWidth(text)/2.0,  0.5*1/0.003+verdana.stringHeight(text)/2.0, 0);
	
	if(aspect < 1.0){
		glTranslated( aspect*0.5*1.0/fontSize-verdana.stringHeight(text)/2.0,  10, 0);	
		
		glRotated(90, 0, 0, 1.0);
	} else {
		glTranslated( aspect*0.5*1.0/fontSize-verdana.stringWidth(text)/2.0,  0.5*1.0/fontSize+verdana.stringHeight(text)/2.0, 0);	
	}
	
	verdana.drawString(text,0,0);
}

void ProjectionSurfaces::mousePressed(ofMouseEventArgs & args){
	ofxVec2f curMouse = ofxVec2f((float)(glDelegate->mouseX-offset)/w, (float)(glDelegate->mouseY-offset)/w);
	selectedCorner = getWarp(selectedKeystoner)->GetClosestCorner(curMouse.x, curMouse.y);
	lastMousePos = curMouse;
}

void ProjectionSurfaces::mouseDragged(ofMouseEventArgs & args){
	ofxVec2f curMouse = ofxVec2f((float)(glDelegate->mouseX-offset)/w, (float)(glDelegate->mouseY-offset)/w);
	ofxVec2f newPos =  getWarp(selectedKeystoner)->corners[selectedCorner] + (curMouse-lastMousePos);
	getWarp(selectedKeystoner)->SetCorner(selectedCorner, newPos.x, newPos.y);
	lastMousePos = curMouse;
	getWarp(selectedKeystoner)->MatrixCalculate();
	saveXml();
}

void ProjectionSurfaces::keyPressed(ofKeyEventArgs & args){
	ofxVec2f newPos =  getWarp(selectedKeystoner)->corners[selectedCorner] ;
	
	if(args.key == 63233){
		newPos -= ofxVec2f(0,-0.001);
	}
	if(args.key == 63232){
		newPos += ofxVec2f(0,-0.001);
	}
	if(args.key == 63234){
		newPos += ofxVec2f(-0.001,0);
	}
	if(args.key == 63235){
		newPos -= ofxVec2f(-0.001,0);
	}
	getWarp(selectedKeystoner)->SetCorner(selectedCorner, newPos.x, newPos.y);
	getWarp(selectedKeystoner)->MatrixCalculate();
	
	saveXml();
}

void ProjectionSurfaces::saveXml(){
	
	keystoneXml->pushTag("floor", 0);
	int numCorners = keystoneXml->getNumTags("corner");
	if(numCorners != 4){
		cout<<"====== ERROR: Wrong number of corners i floor ======"<<endl;
	} else {
		for(int i=0;i<4;i++){
			keystoneXml->setAttribute("corner", "number", i, i);
			keystoneXml->setAttribute("corner", "x", floorProjection->corners[i].x, i);
			keystoneXml->setAttribute("corner", "y", floorProjection->corners[i].y, i);
		}
		keystoneXml->setAttribute("aspect", "value", floorAspect, 0);
	}
	keystoneXml->popTag();
	
	keystoneXml->pushTag("columns", 0);
	for(int u=0;u<3;u++){
		keystoneXml->pushTag("column", u);
		
		int numCorners = keystoneXml->getNumTags("corner");
		if(numCorners != 4){
			cout<<"====== ERROR: Wrong number of corners ======"<<endl;
		} else {
			for(int i=0;i<4;i++){
				keystoneXml->setAttribute("corner", "number", i, i);
				keystoneXml->setAttribute("corner", "x", columnProjection[u]->corners[i].x, i);
				keystoneXml->setAttribute("corner", "y", columnProjection[u]->corners[i].y, i);
			}
		}
		keystoneXml->setAttribute("aspect", "value", columnAspect[u], 0);
		
		keystoneXml->popTag();
		
	}
	keystoneXml->popTag();
	
	keystoneXml->pushTag("curtains", 0);
	for(int u=0;u<6;u++){
		keystoneXml->pushTag("curtain", u);
		
		int numCorners = keystoneXml->getNumTags("corner");
		if(numCorners != 4){
			cout<<"====== ERROR: Wrong number of corners ======"<<endl;
		} else {
			for(int i=0;i<4;i++){
				keystoneXml->setAttribute("corner", "number", i, i);
				keystoneXml->setAttribute("corner", "x", curtainProjection[u]->corners[i].x, i);
				keystoneXml->setAttribute("corner", "y", curtainProjection[u]->corners[i].y, i);
			}
		}
		keystoneXml->setAttribute("aspect", "value", curtainAspect[u], 0);
		
		keystoneXml->popTag();
		
	}
	keystoneXml->popTag();
	
	
	keystoneXml->saveFile("keystoneSettings.xml");
	
	
}


Warp * ProjectionSurfaces::getWarp(int i){
	switch (i) {
		case 0:
			return floorProjection;
			break;
		case 1:
			return columnProjection[0];
			break;
		case 2:
			return columnProjection[1];
			break;
		case 3:
			return columnProjection[2];
			break;
		case 4:
			return curtainProjection[0];
			break;
		case 5:
			return curtainProjection[1];
			break;
		case 6:
			return curtainProjection[2];
			break;
		case 7:
			return curtainProjection[3];
			break;
		case 8:
			return curtainProjection[4];
			break;
		case 9:
			return curtainProjection[5];
			break;
			
		default:
			break;
	}
}

void ProjectionSurfaces::applyFloorProjection(float _w, float _h){
	glPushMatrix();
	float setW = 1.0;
	float setH = 1.0;
	if(floorAspect < 1.0){
		setW *= 1.0/floorAspect;
	} else {
		setW *= 1.0/floorAspect;
	}
	
	glScaled(_w, _h, 1.0);
	floorProjection->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}

void ProjectionSurfaces::applyColumnProjection(int column, float _w, float _h){
	glPushMatrix();
	float setW = 1.0;
	float setH = 1.0;
	if(columnAspect[column] < 1.0){
		setW *= 1.0/columnAspect[column];
	} else {
		setW *= 1.0/columnAspect[column];
	}
	
	glScaled(_w, _h, 1.0);
	columnProjection[column]->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}

void ProjectionSurfaces::applyCurtainProjection(int column, int row, float _w, float _h){
	int n = row + column*3;
	glPushMatrix();
	float setW = 1.0;
	float setH = 1.0;
	if(curtainAspect[n] < 1.0){
		setW *= 1.0/curtainAspect[n];
	} else {
		setW *= 1.0/curtainAspect[n];
	}
	
	glScaled(_w, _h, 1.0);
	curtainProjection[n]->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}

void ProjectionSurfaces::applyProjection(ProjectionSurfacesObject * obj,float _w, float _h){
	glPushMatrix();
	float setW = 1.0;
	float setH = 1.0;
	if(*(obj->aspect) < 1.0){
		//setH *= *(obj->aspect);
		setW *= 1.0/ *(obj->aspect);
		
	} else {
		setW *= 1.0/ *(obj->aspect);
	}
	
	glScaled(_w, _h, 1.0);
	obj->warp->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}