
#include "PluginController.h"
#include "FrostMotor.h"



#pragma mark IceBlock Background object

IceBlockBackgroundObject::IceBlockBackgroundObject(float x, float y,float initA,float _size){
	position.x = x;
	position.y = y;
	generate();
	upTimer = 0;
	downTimer = 0;
	a = initA;
	size = _size;
	pthread_mutex_init(&plock, NULL);
	color = ofColor();
}
void IceBlockBackgroundObject::generate(){
	int n = int(ofRandom(4, 9));
	points.clear();
	points.reserve(n);
	float offset = ofRandom(0,TWO_PI);
	for(int i=0;i<n;i++){
		float p = ((float)i/n)*TWO_PI+offset;
		points.push_back(ofxVec2f(cos(p),sin(p)) * 0.12*ofRandom(0.5,1)*size );
	}
	speed = ofRandom(0.6,1.1);
}
void IceBlockBackgroundObject::draw(){
	//ofEnableAlphaBlending();
	float drawA = 1.0-MAX(0.0,MIN(1.0,a));
	if(drawA > 0){
		glPushMatrix();
		
		ofColor theColor = ofGetStyle().color;
		ofPushStyle();
		
		glTranslated(position.x, position.y, 0);
		//		ofSetColor(255, 255,255,255*drawA*(1.0-FrostMotor::slider4));
		ofSetColor(color.r,color.g,color.b,theColor.a * drawA);
		//	ofSetColor(0, 0, 0, 255);
		glLineWidth(2);
		glBegin(GL_POLYGON);
		for(int i=0;i<points.size();i++){
			glVertex2f(points[i].x, points[i].y);
		}
		glEnd();
		ofPopStyle();
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

void FrostMotor::setColor(ofColor c){
	for(int i=0;i<iceblockBackgrounds.size();i++){
		iceblockBackgrounds[i].color = c;
	}
}
void FrostMotor::setAreaValue(ofxPoint2f point, float radius, float value){
	for(int i=0;i<iceblockBackgrounds.size();i++){
		if(point.distance(iceblockBackgrounds[i].position) < radius){
			iceblockBackgrounds[i].a = value;
			
		}
	}
}	

void FrostMotor::setValueOnAll( float value){
	for(int i=0;i<iceblockBackgrounds.size();i++){
		iceblockBackgrounds[i].a = value;
	}
}


void FrostMotor::generateBackgroundObjects(int resolution, float objectSizes, float w, float h, float initValue){
	r = 0.04*1.0/(resolution/35.0);
	rSq = r*r;
	
	iceblockBackgrounds.clear();
	for(int i=0;i<resolution*w;i++){
		for(int u=0;u<resolution*h;u++){
			float x = (float)i/resolution;
			float y = (float)u/resolution;
			ofxPoint2f p = ofxPoint2f(x,y);
			iceblockBackgrounds.push_back(IceBlockBackgroundObject(x, y, initValue,objectSizes));
		}
	}
	int t = ofGetElapsedTimeMillis();
	int num = iceblockBackgrounds.size();
	int count = 0;
#pragma omp parallel for 
	for(int i=0;i<num;i++){
#pragma omp parallel for 
		for(int u=i+1;u<num;u++){
			count++;
			ofxVec2f v = (iceblockBackgrounds[i].position - iceblockBackgrounds[u].position);
			if(v.lengthSquared() < rSq){
				pthread_mutex_lock(&iceblockBackgrounds[i].plock);
				iceblockBackgrounds[i].closeBackgrounds.push_back(u);
				pthread_mutex_unlock(&iceblockBackgrounds[i].plock);
				
				pthread_mutex_lock(&iceblockBackgrounds[u].plock);
				iceblockBackgrounds[u].closeBackgrounds.push_back(i);
				pthread_mutex_unlock(&iceblockBackgrounds[u].plock);
			}			
		}
	}
	cout<<"Frost motor init time: "<<ofGetElapsedTimeMillis()-t<<endl;
}

void FrostMotor::addBodyPoint(ofPoint p){
	bodyPoints.push_back(p);
}
void FrostMotor::addBodyPoints(vector<ofPoint>* v){
	vector<ofPoint>::iterator it;
	it=v->begin()+1;
	bodyPoints.assign (it,v->end()-1); // the 5 central values of first
}
void FrostMotor::addBodyCenter(ofPoint p){
	bodyCenters.push_back(p);	
}

int FrostMotor::addFreezePoint(ofPoint p, float rate){
	bool ok = true;
	for(int i=0;i<freezePoints.size();i++){
		if(freezePoints[i].position.distanceSquared(p) < rSq*1.5){
			ok = false;
			freezePoints[i].active = true;
			freezePoints[i].rate = rate;
			break;
		}
	}
	if(ok){

		FreezePoint f;
		f.position = p;
		f.rate = rate;
		pthread_mutex_init(&f.plock, NULL);
		
#pragma omp parallel for
		for(int i=0;i<iceblockBackgrounds.size();i++){
			if(((ofxPoint2f)iceblockBackgrounds[i].position).distanceSquared(f.position) < rSq*2.5){
				pthread_mutex_lock(&f.plock);
				f.close.push_back(i);
				pthread_mutex_unlock(&f.plock);
			}
		}
		f.active = true;
		freezePoints.push_back(f);
		return freezePoints.size()-1;
	}
	return -1;
}


void FrostMotor::update(){
	int numBackgrounds = iceblockBackgrounds.size();	
	
	
#pragma omp parallel for
	for(int u=0;u<numBackgrounds;u++){
		
		
		IceBlockBackgroundObject * obj = &iceblockBackgrounds[u];
		//See if this background is near to centroid and should break
		
		bool breaking = false;
		for(int i=0;i<bodyCenters.size();i++){
			if(((ofxVec2f)bodyCenters[i] - obj->position).length() < r){
				obj->a += 0.08*centerBreakRate*obj->speed;
				breaking = true;
				break;
			}
		}
		
		
		if(!breaking) {
			//Is it close to a black background?
			bool close = false;
			for(int x=0;x<obj->closeBackgrounds.size();x++){
				IceBlockBackgroundObject * obj2 = &iceblockBackgrounds[obj->closeBackgrounds[x]];
				if(obj2->a > 0.9){
					close = true;
					break;
				}
			}
			
			if(close){
				//If its close check if its near to a blob point
				for(int pu=0;pu<bodyPoints.size();pu+=5){
					if(((ofxVec2f)bodyPoints[pu] - obj->position).length() < r+0.05 ){
						// then it should get black aswell
						obj->a += 0.16*bodyBreakRate*obj->speed ;	
						obj->upTimer = 0;
						break;
					}							
				}
			}
		}
		
		
		
	}
	
	
	for(int i=0;i<freezePoints.size();i++){			
		if(freezePoints[i].active){
			for(int v=freezePoints[i].close.size()-1;v>=0;v--){				
				IceBlockBackgroundObject * obj = &iceblockBackgrounds[freezePoints[i].close[v]];
				//if(obj->upTimer > 10){	
					pthread_mutex_lock(&obj->plock);
					obj->a -= 0.08*freezePoints[i].rate*obj->speed;
					obj->downTimer = 0;

					pthread_mutex_unlock(&obj->plock);
				/*} else {
					cout<<"uptimer error"<<endl;
				}*/
				
			}		
		}				
	}
	
	for(int u=0;u<numBackgrounds;u++){
		if(iceblockBackgrounds[u].a > 1.0){
			iceblockBackgrounds[u].a = 1.0;
		}
		if(iceblockBackgrounds[u].a < -3){
			iceblockBackgrounds[u].a = -3;
		}
		iceblockBackgrounds[u].upTimer ++;
		iceblockBackgrounds[u].downTimer ++;
	}
	
	
	
	
	for (int i=0; i<numBackgrounds; i++) {
		IceBlockBackgroundObject * obj = &iceblockBackgrounds[i];
		
		//Lets find the closest backgrounds
		bool collapse = false;
		
		IceBlockBackgroundObject * obj2;
		for(int u=0;u<obj->closeBackgrounds.size();u++){
			obj2 = &iceblockBackgrounds[obj->closeBackgrounds[u]];
			if( expandRate > 0 && obj->a > 0.9){
				//expand to the close background
				obj2->a += 0.08*expandRate*obj->speed;
				obj2->upTimer = 0;
			}
			if( decreaseRate > 0 && obj2->a < 0.3){
				collapse = true;
			}
			
		}
		
		if(collapse &&obj->upTimer > 10){
			obj->a -= 0.08*decreaseRate*obj->speed;
			obj->downTimer = 0;
		}		
	}
	
	bodyPoints.clear();
	bodyCenters.clear();
	
	for(int i=0;i<freezePoints.size();i++){
		freezePoints[i].active = false;
	}
	
}
void FrostMotor::draw(){
	for(int i=0;i<iceblockBackgrounds.size();i++){
		iceblockBackgrounds[i].draw();
	}
}
