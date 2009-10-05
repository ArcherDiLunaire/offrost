
#include "PluginController.h"
#include "PluginIncludes.h"

LaLinea::LaLinea(){
	type = OUTPUT;
	cam = 2;
	extrude = 400.0;
	lineWidth = 0.025;
	yPosition = 0.95;
}

void LaLinea::setup(){
}

void LaLinea::update(){
	
}
void LaLinea::draw(){
}

ofxVec2f LaLinea::camOffset(float x){
	ofxVec2f start = *projection()->getColumn(0)->corners[2];
	ofxVec2f b = *projection()->getColumn(2)->corners[3] - *projection()->getColumn(0)->corners[2];
	ofxVec2f a = (*projection()->getColumn(1)->corners[2] - *projection()->getColumn(1)->corners[3])/2.0 + *projection()->getColumn(1)->corners[3] - *projection()->getColumn(0)->corners[2];
	ofxVec2f ab = b * a.dot(b)/(b.length()*b.length());
	ofxVec2f c = a - ab;
	
	if(x < ab.length()){
		float s = (float)(c.length())/ab.length();
		return ofxVec2f(0,x*s);
	} else {
		float s = (float)-(c.length())/(1.0-ab.length());		
		return ofxVec2f(0,(c.length())+x*s);
	}
}

void LaLinea::drawOnWall(){
	ofPushStyle();
	
	ofEnableSmoothing();
	glDisable(GL_DEPTH_TEST);
	ofFill();
	
	glColor4f(1.0, 1.0,1.0, 1.0);
	
	
	
	glPopMatrix();
	
	projection()->applyCurtainProjection(0, 2);
	
	ofxVec2f lineWidthPoint = projection()->convertBetweenSurfaces(projection()->getWall(), projection()->getCurtain(2), ofxVec2f(0,lineWidth));
	ofxVec2f lineWidthPoint2 = projection()->convertBetweenSurfaces(projection()->getWall(), projection()->getCurtain(2), ofxVec2f(0,0));
	
	float lineWidthScaled = lineWidthPoint.y-lineWidthPoint2.y;
	
	ofRect(0.0, yPosition-lineWidthScaled, projection()->getCurtain(2)->aspect , lineWidthScaled);
	
	glPopMatrix();
	
	projection()->applyCurtainProjection(1, 2);
	
	lineWidthPoint = projection()->convertBetweenSurfaces(projection()->getWall(), projection()->getCurtain(5), ofxVec2f(0,lineWidth));
	lineWidthPoint2 = projection()->convertBetweenSurfaces(projection()->getWall(), projection()->getCurtain(5), ofxVec2f(0,0));
	
	lineWidthScaled = lineWidthPoint.y-lineWidthPoint2.y;
	
	ofRect(0.0, yPosition-lineWidthScaled, projection()->getCurtain(5)->aspect , lineWidthScaled);
	
	glPopMatrix();
	
	projection()->applyWallProjection();
	
	camOffset(0.5);
	for(int i=0;i<blob(cam)->numBlobs();i++){
		vector<ofxVec2f> points;
		int n = 0;

		ofxCvBlob b = blob(cam)->getBlob(i);
		for(int j=b.nPts-1;j>=0;j--){
			points.push_back(projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[j].x, b.pts[j].y)-2*camOffset(b.pts[j].x)));
//			points[n] += camOffset(points[n].x);
			n++;
		}
		/*points.push_back(projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[b.nPts-1].x, b.pts[b.nPts-1].y)+camOffset(b.pts[b.nPts-1].x)));
		 points.push_back(projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[b.nPts-2].x, b.pts[b.nPts-2].y)+camOffset(b.pts[b.nPts-2].x)));
		 points.push_back(projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[b.nPts-3].x, b.pts[b.nPts-3].y)+camOffset(b.pts[b.nPts-3].x)));*/
		
		drawContour(&points, lineWidth, lineWidth);
	}
	
	
	ofFill();
	
	glColor4f(0.0, 0.0, 0.0, 1.0);
	
	ofBeginShape();
	
	for(int i=0;i<blob(cam)->numBlobs();i++){
		ofxCvBlob b = blob(cam)->getBlob(i);
		for(int j=0;j<b.nPts;j++){
			ofxVec2f p = projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[j].x, b.pts[j].y));
			ofVertex(p.x, p.y);
		}
	}
	
	ofEndShape(true);
	
	
	glPopMatrix();
	
	projection()->applyCurtainProjection(0, 2);
	
	ofRect(-1000, yPosition, 1000+projection()->getCurtain(2)->aspect+0.1, 1000);
	
	glPopMatrix();
	
	projection()->applyCurtainProjection(1, 2);
	
	ofRect(-0.1, yPosition, 1000, 1000);
	
	glPopMatrix();
	
	projection()->applyCurtainProjection(1, 2);
	
	ofRect(0.0, yPosition, 1000, 1000);
	
	
	ofPopStyle();
	
	
}

void LaLinea::drawContour(vector<ofxVec2f> * _points, float _lineWidth, float _extrusion){
	if (_points->size()>0) {
		
		vector<ofxVec2f> p2;
		
		p2.assign(_points->size(), ofxVec2f());
		
		vector<ofxVec2f> normals;
		
		contourSimp.smooth(*_points, p2, 0.6);
		
		contourNorm.makeNormals(p2, normals);
		
		glColor4f(255, 255, 255,255);
		
		//	glLineWidth(_lineWidth);
		glColor4f(255, 255, 255,255);
		
		glBegin(GL_QUAD_STRIP);
		for(int i =0;i<p2.size();i++){
			glVertex3d(p2[i].x+normals[i].x*_lineWidth, p2[i].y+normals[i].y*_lineWidth,0);
			glVertex3d(p2[i].x, p2[i].y,0);		
			
		}
		
		glEnd();	
		
		ofPopStyle();
		
	}
	
} 
