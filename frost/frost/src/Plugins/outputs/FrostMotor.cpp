
#include "PluginController.h"
#include "FrostMotor.h"


float FrostMotor::randomFactor = 5.0;
float FrostMotor::slider1 = 0.0;
float FrostMotor::slider2 = 0;
float FrostMotor::slider3 = 0;
float FrostMotor::slider4 = 0;
float FrostMotor::slider5 = 0;
float FrostMotor::slider6 = 0.0;

bool FrostMotor::applyToOther = false;


#pragma mark IceBlock Background object

IceBlockBackgroundObject::IceBlockBackgroundObject(float x, float y){
	position.x = x;
	position.y = y;
	generate();
	upTimer = 0;
	downTimer = 0;
}
void IceBlockBackgroundObject::generate(){
	a = -3;
	int n = int(ofRandom(4, 9));
	points.clear();
	points.reserve(n);
	float offset = ofRandom(0,TWO_PI);
	for(int i=0;i<n;i++){
		float p = ((float)i/n)*TWO_PI+offset;
		points.push_back(ofxVec2f(cos(p),sin(p)) * 0.12*ofRandom(0.5,1) );
	}
}
void IceBlockBackgroundObject::draw(){
	ofEnableAlphaBlending();
	float drawA = 1.0-MAX(0.0,MIN(1.0,a));
	if(drawA > 0){
		glPushMatrix();
		
		glTranslated(position.x, position.y, 0);
		ofSetColor(255, 255,255,255*drawA*(1.0-FrostMotor::slider4));
		//	ofSetColor(0, 0, 0, 255);
		glLineWidth(2);
		glBegin(GL_POLYGON);
		for(int i=0;i<points.size();i++){
			glVertex2f(points[i].x, points[i].y);
		}
		glEnd();
		glPopMatrix();
	}
	/*	glPushMatrix();
	 
	 glTranslated(position.x, position.y, 0);
	 
	 glBegin(GL_POLYGON);
	 for(int i=0;i<points.size();i++){
	 glVertex2f(points[i].x, points[i].y);
	 }
	 glEnd();
	 glPopMatrix();
	 */
}


#pragma mark FrostMotor


FrostMotor::FrostMotor(){

}

void FrostMotor::setup(){/*
	//blackSpots.push_back(BlackSpotObject());
	//blackSpots.push_back(BlackSpotObject());
	iceBlocks.push_back(IceBlock());
	iceBlocks.push_back(IceBlock());
	
	int n = 35;
	for(int i=0;i<n*projection()->getFloor()->aspect;i++){
		for(int u=0;u<n;u++){
			float x = (float)i/n;
			float y = (float)u/n;
			ofxPoint2f p = ofxPoint2f(x,y);
			//	if(((ofxVec2f)p-ofxVec2f(0.5*projection()->getFloor()->aspect,0.5)).y < ofRandom(0.4, 0.5)){
			//		if(((ofxVec2f)p-ofxVec2f(0.5*projection()->getFloor()->aspect,0.5)).x < ofRandom(0.4, 0.5)*projection()->getFloor()->aspect){
			
			iceblockBackgrounds.push_back(IceBlockBackgroundObject(x, y));
			//			}
			//		}
		}
	}
	
	float r = 0.03;
	float rSq = r*r;
	
	int num = iceblockBackgrounds.size();
	for(int i=0;i<num;i++){
		for(int u=0;u<num;u++){
			if((iceblockBackgrounds[i].position - iceblockBackgrounds[u].position).lengthSquared() < rSq){
				iceblockBackgrounds[i].closeBackgrounds.push_back(u);
			}			
		}
	}
						  */
}

void FrostMotor::update(){
	//blob(0)->postBlur = 100*Frostscape::slider5;
	//blob(0)->postThreshold = 0;
	/*
	 BlackSpotObject * otherB = &blackSpots[1];
	 for(int i=0;i<MIN(blob(cam)->numPersistentBlobs(),blackSpots.size());i++){
	 ofxCvBlob b = blob(cam)->persistentBlobs[i].blob;
	 blackSpots[i].centroidV = blob(cam)->persistentBlobs[i].centroidV;
	 blackSpots[i].updateBlob(b, controller, otherB);
	 otherB = &blackSpots[i];
	 }*/
	/*
	 for(int u=0;u<iceblockBackgrounds.size();u++){
	 if(iceblockBackgrounds[u].a > 0){
	 iceblockBackgrounds[u].a -= 0.1;
	 }
	 if(u%10==0){
	 if( iceblockBackgrounds[u].a < 0.1){
	 iceblockBackgrounds[u].a = 0.1;
	 //	iceblockBackgrounds[u].generate();
	 }
	 } else {
	 if( iceblockBackgrounds[u].a < 0){
	 iceblockBackgrounds[u].a = 0;
	 iceblockBackgrounds[u].generate();
	 }
	 }
	 }	
	 */
	
	/*
	for(int i=0;i<iceBlocks.size();i++){
		if(blob(cam)->numPersistentBlobs() > i){
			PersistentBlob * pb = &blob(cam)->persistentBlobs[i];
			vector<ofxCvBlob> b = blob(cam)->persistentBlobs[i].blobs;
			vector<ofxVec2f> p;
			for(int g=0;g<b.size();g++){
				for(int u=0;u<b[g].nPts;u++){
					ofxVec2f r = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b[g].pts[u].x,b[g].pts[u].y));			
					p.push_back(r);
				}
			}
			iceBlocks[i].setBlobPoints(p);
			
			//Get backgrund inside radius from centroid
			float r = 0.05;
			float rSq = r*r;
			int numBackgrounds = iceblockBackgrounds.size();
#pragma omp parallel for
			for(int u=0;u<numBackgrounds;u++){
				IceBlockBackgroundObject * obj = &iceblockBackgrounds[u];
				//See if this background is near to centroid and should break
				ofxVec2f centr = projection()->convertToCoordinate(projection()->getFloor(), pb->centroid);			
				
				if((centr - obj->position).length() < r){
					obj->a += 0.08*FrostMotor::slider1;
				} else {
					//Is it close to a black background?
					bool close = false;
					for(int x=0;x<obj->closeBackgrounds.size();x++){
						IceBlockBackgroundObject * obj2 = &iceblockBackgrounds[obj->closeBackgrounds[x]];
						if(obj2->a > 0.9){
							//if((obj2->position - obj->position).lengthSquared() < rSq){
							close = true;
							break;
							//}
						}
					}
					
					if(close){
						//If its close check if its near to a blob point
						for(int pu=0;pu<p.size();pu+=5){
							if((p[pu] - obj->position).length() < r+0.05 ){
								// then it should get black aswell
								//	if(obj->downTimer > 10){
								obj->a += 0.16*FrostMotor::slider2;	
								obj->upTimer = 0;
								//}
								break;
							}							
						}
					}
					
				}
			}
			
			
		}
		iceBlocks[i].update();
		
	}
	
	for(int u=0;u<iceblockBackgrounds.size();u++){
		if(iceblockBackgrounds[u].a > 1.0){
			iceblockBackgrounds[u].a = 1.0;
		}
	}
	
	for(int u=0;u<iceblockBackgrounds.size();u++){
		if(iceblockBackgrounds[u].a < -3){
			iceblockBackgrounds[u].a = -3;
		}
	}
	for(int u=0;u<iceblockBackgrounds.size();u++){
		iceblockBackgrounds[u].upTimer ++;
		iceblockBackgrounds[u].downTimer ++;
	}
	
	
	
	//#pragma omp parallel for
	int numBackgrounds = iceblockBackgrounds.size();	
	
	for (int i=0; i<numBackgrounds; i++) {
		IceBlockBackgroundObject * obj = &iceblockBackgrounds[i];
		//if(obj->a > 0.0){
		//Lets find the closest backgrounds
		float r = 0.03;
		bool collapse = false;
		
		IceBlockBackgroundObject * obj2;
		for(int u=0;u<obj->closeBackgrounds.size();u++){
			obj2 = &iceblockBackgrounds[obj->closeBackgrounds[u]];
			if((obj->position - obj2->position).length() < r && i != u){
				if( FrostMotor::slider6 > 0 && obj->a > 0.9){
					//expand to the close background
					//	if(obj2->downTimer > 10){
					obj2->a += 0.08*FrostMotor::slider6;
					obj2->upTimer = 0;
					//	}
				}
				if( FrostMotor::slider5 > 0 && obj2->a < 0.1){
					collapse = true;
				}
			} 
		}
		
		
		//for(int u=0;u<numBackgrounds;u++){
		
		//}
		if(collapse){
			if(obj->upTimer > 10){
				obj->a -= 0.08*FrostMotor::slider5;
				obj->downTimer = 0;
			}
			
			//cout<<iceblockBackgrounds[i].a<<"  "<<i<<endl;
		}
		//}
	}
	
	
	/*
	 for(int u=0;u<iceblockBackgrounds.size();u++){
	 if(iceBlocks[i].pointInside(iceblockBackgrounds[u].position)){
	 if(iceblockBackgrounds[u].a < 1){
	 iceblockBackgrounds[u].a += 0.5*iceBlocks[i].masterAlpha;
	 }
	 if(iceblockBackgrounds[u].a > 1){
	 iceblockBackgrounds[u].a = 1;
	 }
	 }
	 }	
	 */
	
	
	
	
	//			} 
	/*if(Frostscape::slider5 > 0){
	 //Get backgrund inside radius
	 float r = 0.03;
	 bool whiteFound = false;
	 for(int u=0;u<iceblockBackgrounds.size();u++){
	 if((iceblockBackgrounds[i].position - iceblockBackgrounds[u].position).length() < r && i != u){
	 if(iceblockBackgrounds[u].a < 0.1){
	 whiteFound = true;
	 break;
	 }
	 }
	 }
	 
	 if(whiteFound){
	 iceblockBackgrounds[i].a -= 0.08*Frostscape::slider5;
	 cout<<"YAY"<<endl;
	 }
	 
	 }*/
	//	}
	//}
	
	
	
	
	
	
	
	
}
void FrostMotor::draw(){
}

void FrostMotor::drawOnFloor(){
	/*ofFill();
	ofEnableAlphaBlending();
	
	ofSetColor(128, 128, 128,255*(1.0-FrostMotor::slider4));
	//ofRect(0, 0, projection()->getFloor()->aspect, 1);
	
	ofSetColor(0, 0, 0,128);
	/*for(int i=0;i<blackSpots.size();i++){
	 blackSpots[i].draw();
	 }
	 *//*
	for(int i=0;i<iceblockBackgrounds.size();i++){
		iceblockBackgrounds[i].draw();
	}
	
	for(int i=0;i<iceBlocks.size();i++){
		iceBlocks[i].draw();
	}*/
}

void FrostMotor::setslider1(float val){
	FrostMotor::slider1 = val;
}

void FrostMotor::setslider2(float val){
	FrostMotor::slider2 = val;
}
void FrostMotor::setslider3(float val){
	FrostMotor::slider3 = val;
}
void FrostMotor::setslider4(float val){
	FrostMotor::slider4 = val;
}
void FrostMotor::setslider5(float val){
	FrostMotor::slider5 = val;
}
void FrostMotor::setslider6(float val){
	FrostMotor::slider6 = val;
}
