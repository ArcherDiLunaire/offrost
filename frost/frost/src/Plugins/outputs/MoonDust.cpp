
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
	
	rotation = 45;
	size = 0.007;
	length=3000.0;
	density = 1.0;
	debug = false;
	
	cam = 0;
}

void MoonDust::setup(){
	particleImg = new ofImage;
	particleTrack = new ofImage;
	particleImg->loadImage("MoonDustParticle30.png");
	particleTrack->loadImage("MoonDustTrack.png");
}

void MoonDust::update(){
    vector<DustParticle>::iterator it;
    it = particles.begin();
	ofxCvBlob b = blob(cam)->getLargestBlob();
	
	/*for(int i=0;i<b->nPts;i++){
	 ofxPoint2f r = projection()->getFloor()->coordWarp->inversetransform(b->pts[i].x, b->pts[i].y);
	 if(r.y > particles[i].pos().y - 0.01 && r.y < particles[i].pos().y + 0.01){
	 bMin = r.x;
	 bMax = -r.x;
	 }
	 }*/
	ofxPoint2f p = projection()->getColumnCoordinate(1);
	ofxVec2f dir = ofxVec2f(-1,0).rotated(-rotation);
	int sections = 50;
	float section[sections];
	for(int i=0;i<sections;i++){
		section[i] = ofRandom(0.04,0);	
	}
	
	int lowestSection = 0;
	if(blob(cam)->numBlobs() > 0){
		for(int i=0;i<b.nPts-10;i+=10){
			ofxVec2f r = projection()->convertToFloorCoordinate(ofxVec2f(b.pts[i].x, b.pts[i].y));			
			ofxVec2f a = ofxVec2f(r.x-p.x, r.y-p.y);
			ofxVec2f ab = ((a.dot(dir)) * dir);
			for(int u=0;u<sections;u++){
				if(ab.length() < (u+1)*1.0/sections && (ab+p).y > p.y){
					if(fabs((p+ab).distance(r)) > section[u]){
						section[u] = fabs((p+ab).distance(r));
					}
					if(lowestSection < u  ){
						lowestSection = u;
					}
					break;
				}	
			}	
		}
		
		
	}
	
	for(int i=0; i<particles.size();i++){
		particles[i].visible = false;
		float bMin = -0.6; float bMax = 0.6;
		for(int u=0;u<sections;u++){
			if(particles[i].pos().y < (u+1)*1.0/sections){
				if(u  < lowestSection){
					particles[i].visible = true;
				}
				bMin = -section[u];
				bMax = section[u];
				break;
			}	
		}	
		
		
		particles[i].update(force, 1, bMin, bMax);
	}
	
}
void MoonDust::draw(){
	ofSetColor(255, 0, 0, 255);
	ofxCvBlob b = blob(cam)->getLargestBlob();
	if(blob(cam)->numBlobs() > 0 && debug){
		//		ofxVec2f center = projection()->convertToFloorCoordinate(b.centroid);
		ofEllipse(b.centroid.x*ofGetWidth(), b.centroid.y*ofGetHeight(), 10, 10);
	}
}

void MoonDust::drawOnFloor(){
	ofxPoint2f p = projection()->getColumnCoordinate(1);
	//float size = 0.007;
	
	
	//applyFloorProjection();
	glPushMatrix();
	glTranslated(p.x, p.y, 0);
	glRotated(rotation, 0, 0, 1.0);
	
	
	glBlendFunc (GL_SRC_COLOR, GL_ONE);	
    vector<DustParticle>::iterator it;
    it = particles.begin();
	int n = 0;
    while( it != particles.end() && n < density) {
		if((*it).alpha > 0){
			ofSetColor(255*it->alpha, 255*it->alpha, 255*it->alpha);
			
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
			ofxVec2f dir = ofxVec2f(-1,0).rotated(-rotation);
			
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
}


DustParticle::DustParticle(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
	v.x = ofRandom(-0.01, 0.01);
	vEffect = ofRandom(0.8, 1.2);
	goingMax = false;
	visible = false;
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
	if(!visible){
		alpha -= 0.01;
		if(alpha < 0)
			alpha = 0;
	} else {
		alpha += 0.01;
		if(alpha > 1)
			alpha = 1;
	}
}



ofxVec3f DustParticle::pos(){
	return ofxVec3f(x,y,z);
}