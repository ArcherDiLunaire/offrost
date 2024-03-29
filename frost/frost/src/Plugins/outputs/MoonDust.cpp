
#include "PluginController.h"
#include "PluginIncludes.h"



MoonDust::MoonDust(){
	type = OUTPUT;
	
	for(int i=0;i<4000;i++){
		particles.push_back(DustParticle(0,ofRandom(-0.9, 1),0));
	}
	
	force = 1;
	
	min = 0.2;
	max = 0.5;
	size = 0.007;
	length=3000.0;
	density = 1.0;
	debug = false;
	
	cam = 0;
	
	columnMaskAlpha = 0;
}

void MoonDust::setup(){
	particleImg = new ofImage;
	particleTrack = new ofImage;
	particleImg->loadImage("MoonDustParticle30.png");
	particleTrack->loadImage("MoonDustTrack.png");
}

void MoonDust::update(){

	controller->projectorMaskAlpha = 1.0;

    vector<DustParticle>::iterator it;
    it = particles.begin();
		
	ofxPoint2f p = projection()->getColumnCoordinate(1);

	ofxPoint2f pColumnTop = projection()->convertToProjectionCoordinate(projection()->getFloor(), projection()->getColumnCoordinateTop(1));
	ofxPoint2f pColumnBottom = projection()->convertToProjectionCoordinate(projection()->getFloor(), projection()->getColumnCoordinate(1));
	
	dir = ofxVec2f(pColumnTop-pColumnBottom);
	dir.rotate(180);

	// moving to floor space
	
	ofxPoint2f pColumnOnFloor = projection()->convertToFloorCoordinate(pColumnBottom); 
	ofxPoint2f pDirOnFloor = projection()->convertToFloorCoordinate(pColumnBottom+dir); 

	dir = pDirOnFloor-pColumnOnFloor;
	dir.normalize();
	
	rotation = -dir.angle(ofxVec2f(0,1));
	
	int sections = 90;
	float section[sections];
	for(int i=0;i<sections;i++){
		section[i] = ofRandom(0.04,0);	
	}
	
	int lowestSection = 0;
	
	for(int i=0;i<blob(cam)->numBlobs();i++){
		ofxCvBlob b = blob(cam)->getBlob(i);
		//		ofxCvBlob b = blob(cam)->getLargestBlob();		
		/*for(int i=0;i<b->nPts;i++){
		 ofxPoint2f r = projection()->getFloor()->coordWarp->inversetransform(b->pts[i].x, b->pts[i].y);
		 if(r.y > particles[i].pos().y - 0.01 && r.y < particles[i].pos().y + 0.01){
		 bMin = r.x;
		 bMax = -r.x;
		 }
		 }*/
		
		
		//		if(blob(cam)->numBlobs() > 0){
		for(int i=0;i<b.nPts-5;i+=5){
			
			ofxVec2f r = projection()->convertToFloorCoordinate(ofxVec2f(b.pts[i].x, b.pts[i].y));			
			ofxVec2f a = ofxVec2f(r.x-p.x, r.y-p.y);
			ofxVec2f ab = ((a.dot(dir)) * dir);
			for(int u=0;u<sections;u++){
				if(ab.length() < (u+1)*1.0/sections && (ab+p).y > p.y){
					if(fabs((p+ab).distance(r)) > section[u]){
						if(u < 10){

						} else {
							section[u] = fabs((p+ab).distance(r));
						}
					}
					if(lowestSection < u  ){
						lowestSection = u;
					}
					break;
				}	
			}	
		}
		
		//		}
	}
	
	// even out sections
	
	for(int u=0;u<sections-3;u++){
		if (section[u] < section[u+1]) {
			section[u] += (section[u+1] - section[u]) * 0.75;
		}
		if (section[u] < section[u+2]) {
			section[u] += (section[u+2] - section[u]) * 0.50;
		}
		if (section[u] < section[u+3]) {
			section[u] += (section[u+3] - section[u]) * 0.25;
		}
	}	
	
	// update particles that are hit by the blob 
	
	for(int i=0; i<particles.size();i++){
		particles[i].visible = false;
		float bMin = -0.6; float bMax = 0.6;
		if(particles[i].pos().y < 0){
			particles[i].visible = true;
			particles[i].alpha = 1.0+particles[i].pos().y;
			bMin = - ofRandom(0.04,0);
			bMax =  ofRandom(0.04,0);
		}  else {
			for(int u=0;u<sections;u++){
				if(particles[i].pos().y < (u+1)*1.0/sections){
					if(u  < lowestSection+6){
						particles[i].visible = true;
					}
					bMin = -section[u];
					bMax = section[u];
					if(u  < lowestSection+6 && u > lowestSection && lowestSection > 6){
						particles[i].visible = true;
						bMin = -section[u-6];
						bMax = section[u-6];
					}
					break;
				}
			}
		}
		particles[i].update(force, 1, bMin*1.2, bMax*1.2);
	}
	
	
}
void MoonDust::draw(){
	/*ofSetColor(255, 0, 0, 255);
	 ofxCvBlob b = blob(cam)->getLargestBlob();
	 if(blob(cam)->numBlobs() > 0 && debug){
	 //		ofxVec2f center = projection()->convertToFloorCoordinate(b.centroid);
	 ofEllipse(b.centroid.x*ofGetWidth(), b.centroid.y*ofGetHeight(), 10, 10);
	 }*/
}

void MoonDust::drawOnFloor(){
	if(masterAlpha > 0){
		
		ofxPoint2f p = projection()->getColumnCoordinate(1);
		//float size = 0.007;
		
		//applyFloorProjection();
		glPushMatrix();
		glTranslated(p.x, p.y, 0);
		glRotated(rotation, 0, 0, 1.0);
		
		ofEnableAlphaBlending();
		glEnable(GL_BLEND);
		glBlendFunc (GL_ONE, GL_ONE);
		glDisable(GL_DEPTH);
		vector<DustParticle>::iterator it;
		it = particles.begin();
		int n = 0;
		while( it != particles.end() && n < density) {
			if((*it).alpha > 0){
				ofSetColor(255*it->alpha*masterAlpha, 255*it->alpha*masterAlpha, 255*it->alpha*masterAlpha,255);
				
				if(it->pos().y<0){
					ofSetColor(it->alpha*255*columnAlpha*masterAlpha, it->alpha*255*columnAlpha*masterAlpha, it->alpha*255*columnAlpha*masterAlpha,255);
				} 
				
				particleTrack->draw((*it).x, (*it).y, -(*it).v.x*length*size, size);
				//particleImg->draw((*it).x-size/2.0-(*it).v.x*100.0*size, (*it).y,size,size);
				float a = 100;
			}
			++it;
			n++;
		}
		
		ofSetColor(255, 255, 255);
		
		glPopMatrix();
		
		
		//Debug stuff: 
		if(debug){
			ofSetColor(255, 0, 0);
			ofNoFill();
			ofEllipse(p.x, p.y, 0.1, 0.1);
			ofFill();
			
			
			
			ofxCvBlob b = blob(cam)->getLargestBlob();
			if(blob(cam)->numBlobs() > 0){
				//dir = ofxVec2f(-1,0).rotated(-rotation);
				
				glColor4f(255, 255, 255,255);
				
				glBegin(GL_LINE_STRIP);
				
				for(int i=0;i<b.nPts;i++){
					ofxVec2f r = projection()->convertToFloorCoordinate(ofxVec2f(b.pts[i].x, b.pts[i].y));			
					glVertex3d(r.x, r.y,0);
				}
				glEnd();	
				glColor4f(255, 0, 255,255);
				
				for(int i=0;i<b.nPts-10;i+=10){
					ofxVec2f r = projection()->convertToFloorCoordinate(ofxVec2f(b.pts[i].x, b.pts[i].y));			
					
					ofxVec2f a = ofxVec2f(r.x-p.x, r.y-p.y);
					ofxVec2f ab = ((a.dot(dir)) * dir);
					ofLine(r.x, r.y, (p.x+ab.x), p.y+ab.y);
				}
				glColor4f(0, 255, 255,255);		
				ofLine(p.x, p.y, p.x+dir.x,p.y+dir.y);
				
				ofxVec2f center = projection()->convertToFloorCoordinate(b.centroid);
				
				//ofEllipse(center.x, center.y, 0.05, 0.05);
				
			}
		}
		
		ofEnableAlphaBlending();

		glPopMatrix();		

		ofFill();

		ofSetColor(0, 0, 0,255*columnMaskAlpha);
		
		for(int i=0;i<3;i++){
			projection()->applyColumnProjection(i);		
			ofRect(0, 0, projection()->getColumn(i)->aspect+0.01, 1.01);		
			glPopMatrix();		
		}
		
	}
}

DustParticle::DustParticle(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
	v.x = ofRandom(-0.01, 0.01);
	vEffect = ofRandom(0.8, 1.2);
	goingMax = false;
	visible = false;
	alpha = 0;
}

void DustParticle::update(float force, float damp, float _min, float _max){
	min = _min;
	max = _max;
	history.push_back(pos());
	if(history.size() > 40){
		history.erase(history.begin());
	}
	
	float a = vEffect*force*0.0001*60.0/ofGetFrameRate(); 
	if(!goingMax){
		a *= -1;
	}	
	
	float timeToStop = fabs(v.x)/fabs(a);
	float p = 0.5*-a*timeToStop*timeToStop+v.x*timeToStop+x;
	
	if(p < min){
		goingMax = true;
	} else if(p > max){
		goingMax = false;
	}
	
	v.x += a ;//(max - x)*0.01*force*60.0/ofGetFrameRate();		
	x += v.x;//*100.0/ofGetFrameRate();
	
	if(pos().y > 0){
		if(!visible){
			alpha -= 0.025;
			if(alpha < 0)
				alpha = 0;
		} else {
			alpha += 0.04;
			if(alpha > 1)
				alpha = 1;
		}
	}
}



ofxVec3f DustParticle::pos(){
	return ofxVec3f(x,y,z);
}