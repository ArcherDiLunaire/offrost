
#include "PluginController.h"
#include "PluginIncludes.h"



float Frostscape::randomFactor = 5.0;
float Frostscape::slider1 = 0.0;
float Frostscape::slider2 = 0;
float Frostscape::slider3 = 0;
float Frostscape::slider4 = 0;
float Frostscape::slider5 = 0;
float Frostscape::slider6 = 0.0;

bool Frostscape::applyToOther = false;

#pragma mark Blackspot

BlackSpotObject::BlackSpotObject(){
}


void BlackSpotObject::updateBlob(ofxCvBlob b, PluginController * controller, BlackSpotObject * otherObject){
	ProjectionSurfaces* proj = getPlugin<ProjectionSurfaces*>(controller);
	//points.clear();
	tmpPoints.clear();
	
	for(int p=0;p<b.nPts;p++){
		ofxVec2f r = proj->convertToCoordinate(proj->getFloor(), ofxVec2f(b.pts[p].x,b.pts[p].y));			
		tmpPoints.push_back(r);
	}
	
	if(points.size()>0){
		contourNorm.makeNormals(tmpPoints, normals);
	}
	
	for(int p=0;p<points.size();p++){
		tmpPoints[p] -= normals[p] * 0.03;
	}
	
	contourSimp.simplify(&tmpPoints, &tmpPoints2, 0.01*Frostscape::slider1);
	
	/*while (points.size() > 100) {
	 contourSimp.simplify(tmpPoints, points, 0.001);
	 }*/
	if(points.size()<MIN(tmpPoints2.size(),100)){
		for(int i=0;i<MIN(tmpPoints2.size(),100)-points.size();i++){
			points.push_back(ofxVec2f());
			pointsV.push_back(ofxVec2f());
			vector<ofxVec2f> v;
			for(int i=0;i<numNoise;i++){
				v.push_back(ofxVec2f());
			}
			noise.push_back(v);
		}
		
	}
	
	
	//	cout<<points.size()<<"   "<<tmpPoints2.size()<<endl;
	
	float tmpId = 0;
	
	centroidV = ofxVec2f();
	for(int i=0;i<pointsV.size();i++){
		centroidV += pointsV[i];
	}
	centroidV /= pointsV.size();
	if(Frostscape::applyToOther){
		float a = (float)tmpPoints2.size()/MIN(points.size(),otherObject->points.size());
		
		for(int i=0;i<MIN(points.size(),otherObject->points.size());i++){
			otherObject->pointsV[i] *= Frostscape::slider2;
			otherObject->pointsV[i] += (tmpPoints2[floor(tmpId)]-points[i])*10.0*Frostscape::slider3;
			otherObject->points[i] += pointsV[i]*1.0/ofGetFrameRate();
			for(int u=0;u<numNoise;u++){
				otherObject->noise[i][u]  *= Frostscape::slider6;
				
				otherObject->noise[i][u] = otherObject->noise[i][u] + ofxVec2f(ofRandom(-1, 1),ofRandom(-1, 1)) * (otherObject->pointsV[i] - otherObject->centroidV).length()*0.1*Frostscape::slider4;
			}
			tmpId += a;
		}
		
	} else {
		float a = (float)tmpPoints2.size()/points.size();
		
		for(int i=0;i<points.size();i++){
			//	cout<<i<<endl;
			pointsV[i] *= Frostscape::slider2;
			pointsV[i] += (tmpPoints2[floor(tmpId)]-points[i])*10.0*Frostscape::slider3;
			points[i] += pointsV[i]*1.0/ofGetFrameRate();
			for(int u=0;u<numNoise;u++){
				noise[i][u]  *= Frostscape::slider6;
				
				noise[i][u] = noise[i][u] + ofxVec2f(ofRandom(-1, 1),ofRandom(-1, 1)) * (pointsV[i] - centroidV).length()*0.1*Frostscape::slider4;
			}
			
			/*	for(int u=0;u<numNoise;u++){
			 //noise[i][u]  *= 0.9;
			 noise[i][u] = -normals[i] * pointsV[i].length()*0.1;
			 }*/
			tmpId += a;
		}
	}
	
}

void BlackSpotObject::draw(){
	/*for(int i=0;i<10;i++){
	 glBegin(GL_POLYGON);
	 for(int p=0;p<points.size();p++){
	 glVertex2f(points[p].x+ofRandom(-0.01, 0.01)*Frostscape::randomFactor,points[p].y+ofRandom(-0.01, 0.01)*Frostscape::randomFactor);
	 }
	 glEnd();
	 }*/
	/*	glBegin(GL_POLYGON);
	 for(int p=0;p<points.size();p++){
	 glVertex2f(points[p].x,points[p].y);
	 }
	 glEnd();*/
	if(points.size()>0){
		for(int i=0;i<noise[0].size();i++){
			ofBeginShape();
			for(int p=0;p<points.size();p++){
				ofVertex(points[p].x+noise[p][i].x,points[p].y+noise[p][i].y);
			}
			/*for(int p=0;p<points.size();p++){
			 ofVertex(points[p].x,points[p].y);
			 }*/
			ofEndShape(true);
			
		}
	}
}

#pragma mark IceBlock Background object

IceBlockBackgroundObject::IceBlockBackgroundObject(float x, float y){
	position.x = x;
	position.y = y;
	generate();
}
void IceBlockBackgroundObject::generate(){
	a = -0.5;
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
		ofSetColor(255, 255,255,255*drawA*(1.0-Frostscape::slider4));
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



#pragma mark IceBlock Joint

void IceBlockJoint::update(){
	
}



#pragma mark IceBlock

IceBlock::IceBlock(){
	int num = 10;
	for(int i=0;i<num;i++){
		joints.push_back(new IceBlockJoint);
		joints[i]->position = ofxVec2f(cos(((float)i/num)*TWO_PI), sin(((float)i/num)*TWO_PI))/3.0 + ofxVec2f(0.5,0.5);
		/*if(i%2 == 0){
		 joints[i]->springJoint = true;
		 } else {
		 joints[i]->springJoint = false;	
		 }*/
		joints[i]->springJoint = false;	
		joints[i]->joints = &joints;
	}
	
	masterAlpha = 1.0;
}

void IceBlock::draw(){
	glPushMatrix();
	for(int i=0;i<joints.size();i++){
		IceBlockJoint * next;
		if(i < joints.size()-1){
			next = joints[i+1];
		} else {
			next = joints[0];
		}
		
		ofSetColor(255, 255, 255, 255.0*Frostscape::slider4);
		
		//		ofEllipse(joints[i]->position.x, joints[i]->position.y, 0.02, 0.02);
		ofLine(joints[i]->position.x, joints[i]->position.y, next->position.x, next->position.y);
		
		ofxVec2f f = joints[i]->position + joints[i]->force;
		ofSetColor(255, 255, 255);
		//ofLine(joints[i]->position.x, joints[i]->position.y, f.x, f.y);
		
	}
	ofSetColor(255, 255, 255, 255.0*Frostscape::slider4);
	
	glBegin(GL_POLYGON);
	for(int i=0;i<joints.size();i++){
		glVertex2f(joints[i]->position.x, joints[i]->position.y);
	}
	glEnd();
	
	
	/***
	 *  Debug 
	 ***/
	
	for (int i=0; i<blobPoints.size(); i++) {
		ofSetColor(255, 0, 0, 255);
		if(!pointInside(blobPoints[i])){
			ofSetColor(255, 255, 0, 255);
			bool jointFound = false;
			for(int u=0;u<joints.size();u++){
				IceBlockJoint * next;
				IceBlockJoint * cur = joints[u];
				if(u < joints.size()-1){
					next = joints[u+1];
				} else {
					next = joints[0];
				}
				
				ofxVec2f b = next->position - cur->position;
				ofxVec2f a = blobPoints[i] - cur->position;
				
				ofxVec2f ab = (a.dot(b))/(b.length()*b.length())*b;
				
				if(ab.align(b, 10) && ab.length()<b.length()){		
					ofxVec2f c = a-ab;
					ofxVec2f hat = ofxVec2f(-ab.y, ab.x);
					if(!hat.align(c, 10)){
						float p = ab.length() / b.length();
						cur->force += c * (1.0-p)*Frostscape::slider3;
						next->force += c * (p) *Frostscape::slider3;
						
						jointFound = true;
						/*		ofSetColor(255, 255, 0);
						 ofLine(cur->position.x,cur->position.y, (cur->position+ab).x, (cur->position+ab).y);
						 ofSetColor(0, 255, 255);
						 ofLine((cur->position+ab).x, (cur->position+ab).y, (cur->position+ab+c).x, (cur->position+ab+c).y);*/
					}
				}
				
			}
			if(!jointFound){
				float shortestDist = 0;
				int bestJoint = -1;
				for(int u=0;u<joints.size();u++){
					if(bestJoint == -1 || (joints[u]->position-blobPoints[i]).length() < shortestDist){
						bestJoint = u;
						shortestDist = (joints[u]->position-blobPoints[i]).length();
					}
				}
				
				/*
				 ofSetColor(0, 158, 255);
				 ofLine(joints[bestJoint]->position.x, joints[bestJoint]->position.y, blobPoints[i].x, blobPoints[i].y);
				 */
				if(bestJoint != -1){
					ofxVec2f c =  blobPoints[i]-joints[bestJoint]->position;
					joints[bestJoint]->force += c *Frostscape::slider3;
				}
			}
		}
		ofSetColor(255, 0, 0, 255.0*Frostscape::slider4);
		//ofRect(blobPoints[i].x, blobPoints[i].y, 0.01, 0.01);
	}
	
	glPopMatrix();
	
}
void IceBlock::update(){
	for(int i=0;i<blobPoints.size();i++){
		if(!pointInside(blobPoints[i])){
			//Go through all joints, and make projection on the line
			for(int u=0;u<joints.size();u++){
				IceBlockJoint * next;
				IceBlockJoint * cur = joints[u];
				if(i < joints.size()-1){
					next = joints[i+1];
				} else {
					next = joints[0];
				}
				
				ofxVec2f b = next->position - cur->position;
				ofxVec2f a = blobPoints[i] - cur->position;
				
				ofxVec2f ab = (a.dot(b))/(b.length()*b.length())*b;
				
				
			}
		}
	}
	ofxVec2f totalV;
	for(int i=0;i<joints.size();i++){
		IceBlockJoint * cur = joints[i];
		
		IceBlockJoint * next;
		if(i < joints.size()-1){
			next = joints[i+1];
		} else {
			next = joints[0];
		}
		IceBlockJoint * prev;
		if(i > 0){
			prev = joints[i-1];
		} else {
			prev = joints[joints.size()-1];
		}	
		
		cur->force += (next->position - cur->position) * 0.1*Frostscape::slider1;
		cur->force += (prev->position - cur->position) * 0.1*Frostscape::slider1;
		
		//if(cur->force.length() > 0.1){
		
		cur->speed *= Frostscape::slider2;
		//	if(cur->force.length() > cur->force.scale(0.01).length()){
		//		cur->speed += cur->force.scale(0.01);
		//	} else {
		cur->speed += cur->force;
		totalV += cur->speed;
		//	}
		cur->position += cur->speed * 1.0/ofGetFrameRate();
		if(cur->position.x > 2){
			cur->position.x = 2;	
		}
		if(cur->position.x < 0){
			cur->position.x = 0;	
		}
		if(cur->position.y > 1){
			cur->position.y = 1;	
		}
		if(cur->position.y < 0){
			cur->position.y = 0;	
		}
		
		cur->force = ofxVec2f();
		//}
		
	}
	
	masterAlpha += (MIN(totalV.length()/5.0,1)-masterAlpha)*0.07;
	
	
}

void IceBlock::setBlobPoints(vector<ofxVec2f> points){
	blobPoints = points;
}

bool IceBlock::pointInside(ofxVec2f point){
	/* The points creating the polygon. */
	int numPoints = joints.size();
	float x[numPoints];
	float y[numPoints];
	float x1,x2;
	
	/* The coordinates of the point */
	float px = point.x;
	float py = point.y;
	
	/* How many times the ray crosses a line-segment */
	int crossings = 0;
	
	/* Coordinates of the points */
	for(int i=0;i<numPoints;i++){
		x[i] = joints[i]->position.x;
		y[i] = joints[i]->position.y;
	}
	
	/* Iterate through each line */
	for ( int i = 0; i < numPoints; i++ ){
		
		/* This is done to ensure that we get the same result when
		 the line goes from left to right and right to left */
		if ( x[i] < x[ (i+1)%numPoints ] ){
			x1 = x[i];
			x2 = x[(i+1)%numPoints];
		} else {
			x1 = x[(i+1)%numPoints];
			x2 = x[i];
		}
		
		/* First check if the ray is possible to cross the line */
		if ( px > x1 && px <= x2 && ( py < y[i] || py <= y[(i+1)%numPoints] ) ) {
			static const float eps = 0.000001;
			
			/* Calculate the equation of the line */
			float dx = x[(i+1)%numPoints] - x[i];
			float dy = y[(i+1)%numPoints] - y[i];
			float k;
			
			if ( fabs(dx) < eps ){
				k = INFINITY;	// math.h
			} else {
				k = dy/dx;
			}
			
			float m = y[i] - k * x[i];
			
			/* Find if the ray crosses the line */
			float y2 = k * px + m;
			if ( py <= y2 ){
				crossings++;
			}
		}
	}
	if ( crossings % 2 == 1 ){
		return true;
	}
	return false;
	
}

#pragma mark Frostscape


Frostscape::Frostscape(){
	type = OUTPUT;
	cam = 0;
}

void Frostscape::setup(){
	//blackSpots.push_back(BlackSpotObject());
	//blackSpots.push_back(BlackSpotObject());
	iceBlocks.push_back(IceBlock());
	iceBlocks.push_back(IceBlock());
	
	int n = 35;
	for(int i=0;i<n*projection()->getFloor()->aspect;i++){
		for(int u=0;u<n;u++){
			iceblockBackgrounds.push_back(IceBlockBackgroundObject((float)i/n, (float)u/n));
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
}

void Frostscape::update(){
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
					obj->a += 0.08;
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
								obj->a += 0.08;
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
	
	//#pragma omp parallel for
	int numBackgrounds = iceblockBackgrounds.size();	
	
	for (int i=0; i<numBackgrounds; i++) {
		IceBlockBackgroundObject * obj = &iceblockBackgrounds[i];
		if(obj->a > 0.0){
			//Lets find the closest backgrounds
			float r = 0.03;
			bool collapse = false;
			
			IceBlockBackgroundObject * obj2;
			for(int u=0;u<obj->closeBackgrounds.size();u++){
				obj2 = &iceblockBackgrounds[obj->closeBackgrounds[u]];
				if((obj->position - obj2->position).length() < r && i != u){
					if( Frostscape::slider6 > 0 && obj->a > 0.9){
						//expand to the close background
						obj2->a += 0.08*Frostscape::slider6;
					}
					if( Frostscape::slider5 > 0 && obj2->a < 0.1){
						collapse = true;
					}
				} 
			}
			
			
			//for(int u=0;u<numBackgrounds;u++){
			
			//}
			if(collapse){
				obj->a -= 0.08*Frostscape::slider5;
				//cout<<iceblockBackgrounds[i].a<<"  "<<i<<endl;
			}
		}
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
void Frostscape::draw(){
}

void Frostscape::drawOnFloor(){
	ofFill();
	ofEnableAlphaBlending();
	
	ofSetColor(128, 128, 128,255*(1.0-Frostscape::slider4));
	//ofRect(0, 0, projection()->getFloor()->aspect, 1);
	
	ofSetColor(0, 0, 0,128);
	/*for(int i=0;i<blackSpots.size();i++){
	 blackSpots[i].draw();
	 }
	 */
	for(int i=0;i<iceblockBackgrounds.size();i++){
		iceblockBackgrounds[i].draw();
	}
	
	for(int i=0;i<iceBlocks.size();i++){
		iceBlocks[i].draw();
	}
}

void Frostscape::setslider1(float val){
	Frostscape::slider1 = val;
}

void Frostscape::setslider2(float val){
	Frostscape::slider2 = val;
}
void Frostscape::setslider3(float val){
	Frostscape::slider3 = val;
}
void Frostscape::setslider4(float val){
	Frostscape::slider4 = val;
}
void Frostscape::setslider5(float val){
	Frostscape::slider5 = val;
}
void Frostscape::setslider6(float val){
	Frostscape::slider6 = val;
}
