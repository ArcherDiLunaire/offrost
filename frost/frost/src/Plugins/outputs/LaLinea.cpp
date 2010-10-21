
#include "PluginController.h"
#include "PluginIncludes.h"

LaLinea::LaLinea(){
	type = OUTPUT;
	cam = 2;
	extrude = 400.0;
	lineWidth = 0.025;
	yPosition = 0.95;
	bUsingFilm = false;
	bHasToUseFilm = false;
}

void LaLinea::setup(){
	
}

void LaLinea::update(){
	if (bHasToUseFilm) {
		useFilm();
	} else {
		useCam();
	}
}

void LaLinea::useFilm(){
	bUsingFilm = getPlugin<Cameras*>(controller)->videoPlayerActive(cam);
	if(!bUsingFilm){
		if(dancer < 0.5){
			getPlugin<Cameras*>(controller)->videoPlayerLoadUrl(cam, "LaLineaEdhamMixedShort.mov");
		} else {
			getPlugin<Cameras*>(controller)->videoPlayerLoadUrl(cam, "LaLineaNelsonMixedShort.mov");	
		}
		getPlugin<Cameras*>(controller)->videoPlayerActivate(cam);
		getPlugin<Cameras*>(controller)->videoPlayerPlay(cam);
		bUsingFilm = true;
	}
}

void LaLinea::useCam(){
	bUsingFilm = getPlugin<Cameras*>(controller)->videoPlayerActive(cam);
	if(bUsingFilm){
		getPlugin<Cameras*>(controller)->videoPlayerStop(cam);
		getPlugin<Cameras*>(controller)->videoPlayerDeactivate(cam);
	}
}

void LaLinea::draw(){
	
}

ofxVec2f LaLinea::camOffset(ofxVec2f pointIn){
	/*ofxVec2f start = *projection()->getColumn(0)->corners[2];
	 ofxVec2f b = *projection()->getColumn(2)->corners[3] - *projection()->getColumn(0)->corners[2];
	 ofxVec2f a = (*projection()->getColumn(1)->corners[2] - *projection()->getColumn(1)->corners[3])/2.0 + *projection()->getColumn(1)->corners[3] - *projection()->getColumn(0)->corners[2];
	 ofxVec2f ab = b * a.dot(b)/(b.length()*b.length());
	 ofxVec2f c = a - ab;*/
	
	float s1, s2, sy1, sy2;
	
	if(bUsingFilm){
	
		s1 = (offsetPointMovie.x / offsetPoint2Movie.x);
		s2 = (1.0-offsetPointMovie.x) /(1.0- offsetPoint2Movie.x);
		sy1 = (offsetPointMovie.y / offsetPoint2Movie.x);
		sy2 =  (-offsetPointMovie.y) /(1.0-offsetPoint2Movie.x);	
		
		if(pointIn.x <  offsetPoint2Movie.x ){
			// vi er mellem de to første søjler
			return ofxVec2f(pointIn.x * s1, pointIn.y + (pointIn.x * sy1) );
		} else {
			// vi er mellem de to sidste søjler
			//		return ofxVec2f(pointIn.x - (offsetPoint2.x-offsetPoint2.x) , pointIn.y);
			return ofxVec2f(offsetPointMovie.x + (s2) * (pointIn.x-offsetPoint2Movie.x)  , offsetPointMovie.y+pointIn.y + ((pointIn.x - offsetPoint2Movie.x) * sy2)   );
			//		pointIn.y + ((pointIn.x - offsetPoint2.x) * sy2  + offsetPoint.y )
		}
	} else {
		s1 = (offsetPoint.x / offsetPoint2.x);
		s2 = (1.0-offsetPoint.x) /(1.0- offsetPoint2.x);
		sy1 = (offsetPoint.y / offsetPoint2.x);
		sy2 =  (-offsetPoint.y) /(1.0-offsetPoint2.x);	
		
		if(pointIn.x <  offsetPoint2.x ){
			// vi er mellem de to første søjler
			return ofxVec2f(pointIn.x * s1, pointIn.y + (pointIn.x * sy1) );
		} else {
			// vi er mellem de to sidste søjler
			//		return ofxVec2f(pointIn.x - (offsetPoint2.x-offsetPoint2.x) , pointIn.y);
			return ofxVec2f(offsetPoint.x + (s2) * (pointIn.x-offsetPoint2.x)  , offsetPoint.y+pointIn.y + ((pointIn.x - offsetPoint2.x) * sy2)   );
			//		pointIn.y + ((pointIn.x - offsetPoint2.x) * sy2  + offsetPoint.y )
		}	
	}
}

void LaLinea::drawOnWall(){
	ofPushStyle();
	
	ofEnableSmoothing();
	ofEnableAlphaBlending();

	glDisable(GL_DEPTH_TEST);
	ofFill();
	
	glColor4f(1.0, 1.0,1.0, masterAlpha);
	
	glPopMatrix();
	
	projection()->applyCurtainProjection(0, 2);
	
	ofxVec2f lineWidthPoint = projection()->convertBetweenSurfaces(projection()->getWall(), projection()->getCurtain(2), ofxVec2f(0,lineWidth));
	ofxVec2f lineWidthPoint2 = projection()->convertBetweenSurfaces(projection()->getWall(), projection()->getCurtain(2), ofxVec2f(0,0));
	
	float lineWidthScaled = lineWidthPoint.y-lineWidthPoint2.y;
	int n= 30;
	glBegin(GL_QUAD_STRIP);
	for(int i=0;i<n;i++){
		ofxVec2f noise = ofxVec2f(0,ofRandom(0, noise1*0.3));
		glVertex2f((float)projection()->getCurtain(2)->aspect*i/n, yPosition-lineWidthScaled-noise.y);
		glVertex2f((float)projection()->getCurtain(2)->aspect*i/n, yPosition-noise.y);
	}
	glEnd();
//	ofRect(0.0, yPosition-lineWidthScaled, projection()->getCurtain(2)->aspect , lineWidthScaled);
	
	glPopMatrix();
	
	projection()->applyCurtainProjection(1, 2);
	
	lineWidthPoint = projection()->convertBetweenSurfaces(projection()->getWall(), projection()->getCurtain(5), ofxVec2f(0,lineWidth));
	lineWidthPoint2 = projection()->convertBetweenSurfaces(projection()->getWall(), projection()->getCurtain(5), ofxVec2f(0,0));
	
	lineWidthScaled = lineWidthPoint.y-lineWidthPoint2.y;
	glBegin(GL_QUAD_STRIP);
	for(int i=0;i<n;i++){
		ofxVec2f noise = ofxVec2f(0,ofRandom(0, noise1*0.3));
		glVertex2f((float)projection()->getCurtain(5)->aspect*i/n, yPosition-lineWidthScaled-noise.y);
		glVertex2f((float)projection()->getCurtain(5)->aspect*i/n, yPosition-noise.y);
	}
	glEnd();
	
	//ofRect(0.0, yPosition-lineWidthScaled, projection()->getCurtain(5)->aspect , lineWidthScaled);
	
	glPopMatrix();
	
	projection()->applyWallProjection();
	
	if(tracking){
		for(int i=0;i<blob(cam)->numBlobs();i++){
			vector<ofxVec2f> points;
			int n = 0;
			
			ofxCvBlob b = blob(cam)->getBlob(i);
			for(int j=b.nPts-1;j>=0;j--){
				points.push_back(projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[j].x, b.pts[j].y)));
				//points[n] = camOffset(points[n]);
				n++;
			}
			points.push_back(points[0]);
			points.push_back(points[1]);
			points.push_back(points[2]);
			
			bool reverse = false;
			if(bUsingFilm){
				ofxPoint2f * points = getPlugin<CameraCalibration*>(controller)->cameras[3]->calibHandles;
				float cross = (points[1].x-points[0].x)*(points[2].y-points[1].y) - (points[1].y-points[0].y)*(points[2].x-points[1].x);
				//(xi - xi-1) * (yi+1 - yi) - (yi - yi-1) * (xi+1 - xi)
				if(cross < 0) reverse = true;				
			} else {
				ofxPoint2f * points = getPlugin<CameraCalibration*>(controller)->cameras[2]->calibHandles;
				float cross = (points[1].x-points[0].x)*(points[2].y-points[1].y) - (points[1].y-points[0].y)*(points[2].x-points[1].x);
				 //(xi - xi-1) * (yi+1 - yi) - (yi - yi-1) * (xi+1 - xi)
				if(cross < 0) reverse = true;
			}

			drawContour(&points, lineWidth, lineWidth, reverse);
		}
		
		
		ofFill();
		
		glColor4f(0.0, 0.0, 0.0, 1.0);
		
		
		for(int i=0;i<blob(cam)->numBlobs();i++){
			ofBeginShape();
			
			ofxCvBlob b = blob(cam)->getBlob(i);
			for(int j=0;j<b.nPts;j++){
				ofxVec2f p = projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[j].x, b.pts[j].y));
				//p = camOffset(p);
				
				ofVertex(p.x, p.y);
			}
			ofEndShape(true);
		}
	}
	
	if(noise3 > 0){
		ofDisableAlphaBlending();
		ofSetColor(0, 0, 0, 255);
		ofRect(noise3*projection()->getWall()->aspect, 0, -100, 100);
		ofEnableAlphaBlending();
	}
	
	
	ofFill();
	
	glColor4f(0.0, 0.0, 0.0, maskAlpha);
	
	glPopMatrix();
	
	projection()->applyCurtainProjection(0, 2);
	
	ofRect(-1000, yPosition, 1000+projection()->getCurtain(2)->aspect+0.1, 1000);
	
	glPopMatrix();
	
	projection()->applyCurtainProjection(1, 2);
	
	ofRect(-0.1, yPosition, 1000, 1000);
	
	glPopMatrix();
	
	for(int i=0;i<3;i++){
		projection()->applyProjection(projection()->getColumn(i));
		ofSetColor(0, 0, 0,255)	;
		ofRect(0.0, 0, projection()->getColumn(i)->aspect, 1);
		glPopMatrix();
	}
	ofPopStyle();
	
}

void LaLinea::drawContour(vector<ofxVec2f> * _points, float _lineWidth, float _extrusion, bool _reverse){
	if (_points->size()>0) {
		
		vector<ofxVec2f>  points = *_points;
		if(_reverse){
			std::reverse(points.begin(), points.end());
		}
				
		ofSetColor(255, 255, 255);
		
		vector<ofxVec2f> p2;
		
		p2.assign(points.size(), ofxVec2f());
		
		vector<ofxVec2f> normals;
		
		contourSimp.smooth(points, p2, 0.6);
		contourNorm.makeNormals(p2, normals);
		
		glBegin(GL_QUAD_STRIP);
		for(int i =0;i<p2.size();i++){
			ofxVec2f n = normals[i]*ofRandom(0, noise1*0.1);
			glVertex3d(p2[i].x+normals[i].x*_lineWidth+n.x, p2[i].y+normals[i].y*_lineWidth+n.y,0);
			glVertex3d(p2[i].x+n.x, p2[i].y+n.y,0);		
			
		}
		
		glEnd();	
		
		ofPopStyle();
		
	}
	
} 
