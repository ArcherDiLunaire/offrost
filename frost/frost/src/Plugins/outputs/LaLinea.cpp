
#include "PluginController.h"
#include "PluginIncludes.h"

bool lineIntersection(
					  double Ax, double Ay,
					  double Bx, double By,
					  double Cx, double Cy,
					  double Dx, double Dy,
					  double *X, double *Y) {
	
	double  distAB, theCos, theSin, newX, ABpos ;
	
	//  Fail if either line segment is zero-length.
	if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy) return false;
	
	//  Fail if the segments share an end-point.
	if (Ax==Cx && Ay==Cy || Bx==Cx && By==Cy
		||  Ax==Dx && Ay==Dy || Bx==Dx && By==Dy) {
		return false; }
	
	//  (1) Translate the system so that point A is on the origin.
	Bx-=Ax; By-=Ay;
	Cx-=Ax; Cy-=Ay;
	Dx-=Ax; Dy-=Ay;
	
	//  Discover the length of segment A-B.
	distAB=sqrt(Bx*Bx+By*By);
	
	//  (2) Rotate the system so that point B is on the positive X axis.
	theCos=Bx/distAB;
	theSin=By/distAB;
	newX=Cx*theCos+Cy*theSin;
	Cy  =Cy*theCos-Cx*theSin; Cx=newX;
	newX=Dx*theCos+Dy*theSin;
	Dy  =Dy*theCos-Dx*theSin; Dx=newX;
	
	//  Fail if segment C-D doesn't cross line A-B.
	if (Cy<0. && Dy<0. || Cy>=0. && Dy>=0.) return false;
	
	//  (3) Discover the position of the intersection point along line A-B.
	ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);
	
	//  Fail if segment C-D crosses line A-B outside of segment A-B.
	if (ABpos<0. || ABpos>distAB) return false;
	
	//  (4) Apply the discovered position to line A-B in the original coordinate system.
	*X=Ax+ABpos*theCos;
	*Y=Ay+ABpos*theSin;
	
	//  Success.
	return true; 
}


LaLinea::LaLinea(){
	type = OUTPUT;
	cam = 2;
	extrude = 400.0;
}

void LaLinea::setup(){
}

void LaLinea::update(){
	
}
void LaLinea::draw(){
}

void LaLinea::drawOnWall(){
	ofPushStyle();
	/*int sections = 100;
	 float s[sections];
	 for(int i=0;i<sections;i++){
	 s[i] = 0.95;
	 }
	 
	 for(int i=0;i<blob(cam)->numBlobs();i++){
	 ofxCvBlob b = blob(cam)->getBlob(i);
	 
	 glColor4f(255, 0, 255,255);
	 
	 glBegin(GL_LINE_STRIP);
	 
	 for(int i=0;i<b.nPts;i++){
	 int section = 0;
	 ofxVec2f r = projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[i].x, b.pts[i].y));			
	 for(int u=0;u<sections;u++){
	 if(r.x < u*1.0/sections){
	 section = u;
	 break;	
	 }
	 }
	 if(s[section] > r.y){
	 s[section] = r.y;
	 }
	 
	 glVertex3d(r.x, r.y,0);
	 }
	 glEnd();	
	 }
	 
	 ofSetColor(255, 255, 255);
	 ofSetLineWidth(4);
	 
	 float x = 0;
	 for(int i=0;i<sections-1;i++){ 
	 ofLine(x, s[i], x+1.0/sections, s[i+1]);		
	 x += 1.0/sections;
	 }
	 */
	
	vector<ofxVec2f> points;
	
	points.push_back(ofxVec2f(0,0.95));

	for(int i=0;i<blob(cam)->numBlobs();i++){
		ofxCvBlob b = blob(cam)->getBlob(i);
		
		//Find all intersections
		vector<ofxVec2f> intersections;
		vector<int> intersectionsId;
		
		for(int u=0;u<b.nPts-1;u++){
						
			ofxVec2f r = projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[u].x, b.pts[u].y));			
			ofxVec2f r2 = projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[u+1].x, b.pts[u+1].y));			
			double x, y;
			if(lineIntersection(
								points[points.size()-1].x,
								points[points.size()-1].y, 
								1.0,
								0.95,
								r.x, r.y, 
								r2.x, r2.y,
								&x, &y)){
				
				intersections.push_back(ofxVec2f(x,y));
				intersectionsId.push_back(u);
			}		
		}
		
		if(intersections.size() > 0){
			
			//Find most left and right interserction
			int left = -1;
			float leftX = 1;
			int right = -1;
			float rightX = 0;
			
			for(int u=0;u<intersections.size();u++){
				if(left == -1 || intersections[u].x < leftX){
					leftX = intersections[u].x;
					left = u;
				}
				
				if(right == -1 || intersections[u].x > rightX){
					rightX = intersections[u].x;
					right = u;
				}
			}
			//	cout<<intersections.size()<<"   "<<intersectionsId[left]<<"   "<<intersectionsId[right]<<endl;
			
			
			//add points between left and right
			if(false){
				points.push_back(intersections[left]);
				for(int u=intersectionsId[left]+1;u>=0;u--){
					ofxVec2f r = projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[u].x, b.pts[u].y));			
					points.push_back(r);
				}
				
				for(int u=b.nPts-1;u>intersectionsId[right]-1;u--){
					ofxVec2f r = projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[u].x, b.pts[u].y));			
					points.push_back(r);
				}
				
				points.push_back(intersections[right]);
			} else {
				points.push_back(intersections[left]);
				for(int u=intersectionsId[left]+1;u>intersectionsId[right]-1;u-=5){
					ofxVec2f r = projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[u].x, b.pts[u].y));			
					points.push_back(r);
					
				}
				
				
				points.push_back(intersections[right]);
			}
			
		} else {
			
			/**
			 * TODO: Paint other blob contours to make sure we have the head and upper part of the body, even if blobs are cut by screen gaps.
			 * see notes in drawContour
			 **/
			
			/** enable code below when drawContour handles both polygon winidng directions and special cases for la ligna
			 
			vector<ofxVec2f> pointsOutsideLine;
						
			for(int u=0;u<b.nPts-1;u++){
				pointsOutsideLine.push_back(projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[u].x, b.pts[u].y)));
			}
			
			drawContour(&pointsOutsideLine, 6.0, extrude);
			//**/
		}

		
	}
	
	points.push_back(ofxVec2f(1,0.95));
		
	drawContour(&points, 6.0, extrude);

}

void LaLinea::drawContour(vector<ofxVec2f> * _points, float _lineWidth, float _extrusion){
	
	/**
	 spline.clear();
	 for(int i=0;i<points.size();i++){
		spline.push_back(points[i]);
	 }
	 **/
	
	vector<ofxVec2f> p2;
	p2.assign(_points->size(), ofxVec2f());
	
	vector<ofxVec2f> normals;
	//normals.assign(points->size(), ofxVec2f());
	
	contourSimp.smooth(*_points, p2, 0.6);
	if(_points->at(_points->size()-1).x == 1.0){
		p2[p2.size()-1].x = 1;
	}
	
	// TODO: find direction (cw or ccw) and make normals in appropriate sequence
	
	contourNorm.makeNormals(p2, normals);

//	// TODO: why is the statement below always false?
//	
//	if (_points->at(0).x == 0.0 && _points->at(0).y == 0.95 ){
		normals[0].y = -1.0;
//	} else {
//		cout << _points->at(0).x << "   " << _points->at(0).y << endl;
//	}

	
	for(int i=0;i<p2.size();i++){
		p2[i] += normals[i]/_extrusion;
		if(p2[i].y > 0.95-1.0/_extrusion){
			p2[i].y = 0.95-1.0/_extrusion;
		}
	}
	
	glColor4f(255, 255, 255,255);
	
	/*glBegin(GL_LINE_STRIP);
	 float spacing = 0.01;
	 for(float f=0; f<1; f+= spacing) {
	 ofxVec2f v = spline.sampleAt(f);
	 glVertex3fv((const float*)&v);
	 }
	 glEnd();*/
	
	//spline.drawSmooth(100, 1, 1);
	
	glLineWidth(_lineWidth);
	glColor4f(255, 255, 255,255);
	
	//glLineWidth(4);
	
	glBegin(GL_QUAD_STRIP);
	for(int i =0;i<p2.size();i++){
		//	if(i!=0){
		glVertex3d(p2[i].x+normals[i].x/(250.0/_lineWidth), p2[i].y+normals[i].y/(250.0/_lineWidth),0);
		//	} else {
		//	glVertex3d(p2[i].x, p2[i].y-1.0/100.0,0);
		//	}
		glVertex3d(p2[i].x, p2[i].y,0);		
		
	}
	glEnd();	
	
	glColor4f(0, 0, 0,255);
	ofFill();
	
	/*ofDisableAlphaBlending();
	 ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	 
	 if(p2.size() > 0){
	 ofBeginShape();
	 for(int i =0;i<p2.size();i++){
	 
	 ofVertex(p2[i].x, p2[i].y);
	 
	 }
	 ofEndShape();
	 }*/
	ofEnableAlphaBlending();
	glColor4f(0, 255, 0,50);
	/*	glBegin(GL_POLYGON);	
	 if(p2.size() > 0){
	 for(int i =0;i<p2.size();i++){
	 
	 glVertex3f(p2[i].x, p2[i].y,0);
	 
	 }
	 
	 }*/
	glEnd();
	//ofRect(0, 0.95-1.0/extrude, 1, 1);
		
	/*
	 glBegin(GL_LINE_STRIP);
	 for(int i =0;i<points.size();i++){
	 glVertex3d(points[i].x, points[i].y,0);
	 
	 }
	 glEnd();	
	 */
	
	ofPopStyle();
	
} 
