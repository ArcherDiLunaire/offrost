
#include "ProjectionSurfaces.h"

ProjectionSurfacesObject::ProjectionSurfacesObject(){
	for(int i=0;i<4;i++){
		corners[i] = new ofxPoint2f;
	}
}

void ProjectionSurfacesObject::recalculate(){
	for(int i=0;i<4;i++){
		warp->SetCorner(i, (*corners[i]).x, (*corners[i]).y);
	}
	
	warp->MatrixCalculate();
	ofxPoint2f a[4];
	a[0] = ofxPoint2f(0,0);
	a[1] = ofxPoint2f(1,0);
	a[2] = ofxPoint2f(1,1);
	a[3] = ofxPoint2f(0,1);
	coordWarp->calculateMatrix(a, warp->corners);
}

void ProjectionSurfacesObject::SetCorner(int n, float x, float y){
	corners[n]->set(x,y);
}

ProjectionSurfaces::ProjectionSurfaces(){
	type = DATA;

	for(int i=0;i<11;i++){
		objects.push_back(new ProjectionSurfacesObject);
		objects[i]->warp = new Warp();
		objects[i]->coordWarp = new coordWarping;
		objects[i]->aspect = 1.0;
	}
	
	objects[10]->corners[0] = getCurtain(0)->corners[1];
	objects[10]->corners[1] = getCurtain(3)->corners[1];	
	objects[10]->corners[2] = getCurtain(5)->corners[2];		
	objects[10]->corners[3] = getCurtain(2)->corners[3];				
	
	objects[0]->name = "FLOOR";
	objects[1]->name = "COLUMN 0";
	objects[2]->name = "COLUMN 1";
	objects[3]->name = "COLUMN 2";
	objects[4]->name = "CURTAIN 0";
	objects[5]->name = "CURTAIN 1";
	objects[6]->name = "CURTAIN 2";
	objects[7]->name = "CURTAIN 3";
	objects[8]->name = "CURTAIN 4";
	objects[9]->name = "CURTAIN 5";
	objects[10]->name = "WALL";

	
	drawDebug = false;
	selectedCorner = 0;
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
				getFloor()->SetCorner( keystoneXml->getAttribute("corner", "number", 0, i) ,  keystoneXml->getAttribute("corner", "x", 0.0, i),  keystoneXml->getAttribute("corner", "y", 0.0, i));
			}
			getFloor()->aspect = keystoneXml->getAttribute("aspect", "value", 1.0, 0);
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
					getColumn(u)->SetCorner( keystoneXml->getAttribute("corner", "number", 0, i) ,  keystoneXml->getAttribute("corner", "x", 0.0, i),  keystoneXml->getAttribute("corner", "y", 0.0, i));
				}
			}
			getColumn(u)->aspect = keystoneXml->getAttribute("aspect", "value", 1.0, 0);
			
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
					getCurtain(u)->SetCorner( keystoneXml->getAttribute("corner", "number", 0, i) ,  keystoneXml->getAttribute("corner", "x", 0.0, i),  keystoneXml->getAttribute("corner", "y", 0.0, i));
				}
			}
			getCurtain(u)->aspect = keystoneXml->getAttribute("aspect", "value", 1.0, 0);			
			keystoneXml->popTag();
			
		}
		keystoneXml->popTag();
		
		
	}
	
	for(int i=0;i<objects.size();i++){
		objects[i]->recalculate();
	}
	
	
}
void ProjectionSurfaces::update(){
}

void ProjectionSurfaces::drawOnFloor(){
}
void ProjectionSurfaces::draw(){
	if(drawDebug){
		for(int i=0;i<objects.size();i++){
			float a = 0.3;
			
			if(selectedKeystoner == i){
				a = 1.0;
			}
			applyProjection(objects[i]);					
			if(i>0&&i<4){
				drawGrid(objects[i]->name, (objects[i]->aspect), 1.0/ (objects[i]->aspect), false, a, 0.02);	
				
			} else {
				drawGrid(objects[i]->name, (objects[i]->aspect), 10, (i==0)?true : false, a, 1.0);	
			}
			
			glPopMatrix();
			
		}
	}
	ofSetColor(255, 255, 0);

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
		ofxVec2f v = objects[selectedKeystoner]->warp->corners[i];
		ofEllipse(v.x, v.y, 0.05, 0.05);
	}	
	glPopMatrix();
	
	//	drawDebugGrids(w,h);
	
	for(int i=0;i<objects.size();i++){
		float a = 0.3;
		if(selectedKeystoner == i){
			a = 1.0;
		}		
		applyProjection(objects[i], w, h);	
		
		if(i>0&&i<4){
			drawGrid(objects[i]->name, (objects[i]->aspect), 1.0/ (objects[i]->aspect), false, a, 0.02);	
			
		} else {
			drawGrid(objects[i]->name, (objects[i]->aspect), 10, (i==0)?true : false, a, 1.0);	
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
	selectedCorner = objects[selectedKeystoner]->warp->GetClosestCorner(curMouse.x, curMouse.y);
	lastMousePos = curMouse;
}

void ProjectionSurfaces::mouseDragged(ofMouseEventArgs & args){
	ofxVec2f curMouse = ofxVec2f((float)(glDelegate->mouseX-offset)/w, (float)(glDelegate->mouseY-offset)/w);
	ofxVec2f newPos =  objects[selectedKeystoner]->warp->corners[selectedCorner] + (curMouse-lastMousePos);
	objects[selectedKeystoner]->SetCorner(selectedCorner, newPos.x, newPos.y);
	lastMousePos = curMouse;
	for(int i=0;i<objects.size();i++){
		objects[i]->recalculate();
	}	
	saveXml();
}

void ProjectionSurfaces::keyPressed(ofKeyEventArgs & args){
	ofxVec2f newPos =  objects[selectedKeystoner]->warp->corners[selectedCorner] ;
	
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
	objects[selectedKeystoner]->SetCorner(selectedCorner, newPos.x, newPos.y);
	for(int i=0;i<objects.size();i++){
		objects[i]->recalculate();
	}	
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
			keystoneXml->setAttribute("corner", "x", getFloor()->warp->corners[i].x, i);
			keystoneXml->setAttribute("corner", "y", getFloor()->warp->corners[i].y, i);
		}
		keystoneXml->setAttribute("aspect", "value", getFloor()->aspect, 0);
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
				keystoneXml->setAttribute("corner", "x", getColumn(u)->warp->corners[i].x, i);
				keystoneXml->setAttribute("corner", "y", getColumn(u)->warp->corners[i].y, i);
			}
		}
		keystoneXml->setAttribute("aspect", "value", getColumn(u)->aspect, 0);
		
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
				keystoneXml->setAttribute("corner", "x", getCurtain(u)->warp->corners[i].x, i);
				keystoneXml->setAttribute("corner", "y", getCurtain(u)->warp->corners[i].y, i);
			}
		}
		keystoneXml->setAttribute("aspect", "value", getCurtain(u)->aspect, 0);
		
		keystoneXml->popTag();
		
	}
	keystoneXml->popTag();
	
	
	keystoneXml->saveFile("keystoneSettings.xml");
	
	
}

ProjectionSurfacesObject * ProjectionSurfaces::getFloor(){
	return objects[0];
}
ProjectionSurfacesObject * ProjectionSurfaces::getColumn(int n){
	return objects[1+n];	
}
ProjectionSurfacesObject * ProjectionSurfaces::getCurtain(int n){
	return objects[4+n];
}
ProjectionSurfacesObject * ProjectionSurfaces::getWall(){
	return objects[10];
}


ofxVec2f  ProjectionSurfaces::convertToFloorCoordinate(ofxVec2f v){
	ofxVec2f r;
	ofxPoint2f p = getFloor()->coordWarp->inversetransform(v.x, v.y);
	r.x = p.x*getFloor()->aspect;
	r.y = p.y;
	return r;
}


ofxVec2f  ProjectionSurfaces::convertToCoordinate(ProjectionSurfacesObject * obj,  ofxVec2f v){
	ofxVec2f r;
	ofxPoint2f p = obj->coordWarp->inversetransform(v.x, v.y);
	r.x = p.x*obj->aspect;
	r.y = p.y;
	return r;
}

ofxPoint2f ProjectionSurfaces::getColumnCoordinate(int column){
	ofxVec2f p1 = getColumn(column)->warp->corners[3]; 
	ofxVec2f p2 = getColumn(column)->warp->corners[2];

	ofxVec2f p = (p1-p2)/2.0+p2;
	ofxPoint2f r = getFloor()->coordWarp->inversetransform(p.x, p.y);
	r.x *= getFloor()->aspect;
	return r;
}



void ProjectionSurfaces::applyFloorProjection(float _w, float _h){
	glPushMatrix();
	float setW = 1.0/getFloor()->aspect;
	float setH = 1.0;

	glScaled(_w, _h, 1.0);
	getFloor()->warp->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}

void ProjectionSurfaces::applyColumnProjection(int column, float _w, float _h){
	glPushMatrix();
	float setW = 1.0/getColumn(column)->aspect;;
	float setH = 1.0;
	
	glScaled(_w, _h, 1.0);
	getColumn(column)->warp->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}

void ProjectionSurfaces::applyCurtainProjection(int column, int row, float _w, float _h){
	int n = row + column*3;
	glPushMatrix();
	float setW = 1.0/getCurtain(n)->aspect;
	float setH = 1.0;

	glScaled(_w, _h, 1.0);
	getCurtain(n)->warp->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}
void ProjectionSurfaces::applyWallProjection(float _w, float _h){
	glPushMatrix();
	float setW = 1.0/getWall()->aspect;
	float setH = 1.0;
	
	glScaled(_w, _h, 1.0);
	getWall()->warp->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}

void ProjectionSurfaces::applyProjection(ProjectionSurfacesObject * obj,float _w, float _h){
	glPushMatrix();
	float setW = 1.0/ (obj->aspect);
	float setH = 1.0;
	
	glScaled(_w, _h, 1.0);
	obj->warp->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}