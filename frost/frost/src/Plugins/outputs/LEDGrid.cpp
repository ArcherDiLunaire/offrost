/*
 *  LEDGrid.cpp
 *  openFrameworks
 *
 *  Created by frost on 06/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "PluginController.h"
#include "PluginIncludes.h"
lamp::lamp(){
	r = 0;
	g = 0;
	b = 0;
	a = 0;
	sentR = -1;
	sentG = -1;
	sentB = -1;
	sentA = -1;
	
	channel = 0;
}

void lamp::update(){
	
}

LEDGrid::LEDGrid(){
	type = OUTPUT;
	cam = 0;
	r = 255;
	g = 255;
	b = 255;
	r2 = 0;
	g2 = 0;
	b2 = 0;
}

#pragma mark Callback methods

void LEDGrid::setup(){
	serial.setup("/dev/tty.usbserial-A6008iyw", 115200);
	
	float dx = 1.0/8.0;
	float dy = 1.0/8.0;
	
	float sx = 0.28;
	float y = 0.25;
	
	for(int i=0;i<5;i++){
		float x = sx;
		
		for(int u=0;u<8;u++){
			lamp l;
			l.pos.x = x;
			l.pos.y = y;
			lamps.push_back(l);
			
			x += dx;
			
		}
		y += dy;
	}
	
	for(int i=0;i<4;i++){
		lamps[i].channel = 221+i*4;	
	}
	for(int i=0;i<5;i++){
		lamps[8+i].channel = 181+i*4;	
	}
	for(int i=0;i<7;i++){
		lamps[8*2+i].channel = 151+i*4;	
	}
	for(int i=0;i<8;i++){
		lamps[8*3+i].channel = 121+i*4;	
	}
	for(int i=0;i<8;i++){
		lamps[8*4+i].channel = 91+i*4;	
	}
	
	
	ok = true;
	
}

void LEDGrid::draw(){
	
}

void LEDGrid::drawOnFloor(){
	/*for(int i=0;i<lamps.size();i++){
		
		ofSetColor(lamps[i].r,lamps[i].g, lamps[i].b);
//		ofSetColor(255, 0, 0);
		ofFill();
		float a = lamps[i].b + lamps[i].g + lamps[i].r;
		a /= 255.0*3.0;
		ofEllipse(lamps[i].pos.x, lamps[i].pos.y, 0.1*a,  0.1*a)	;
		/*ofNoFill();
		ofEllipse(lamps[i].pos.x, lamps[i].pos.y, 0.1,  0.1)	;

	}*/
	
}

void LEDGrid::update(){
	if(serial.available()){
		serial.flush(true, false);
		ok = true;
	}
	
	if(ok){
		for(int i=0;i<lamps.size();i++){
			if(lamps[i].channel > 0){
				//				ofxPoint2f p = ofxPoint2f(mouseX, mouseY);
				if(blob(0)->numPersistentBlobs() > 0){
					ofxPoint2f p = blob(0)->persistentBlobs[0].getLowestPoint();
					p = projection()->convertToFloorCoordinate(p);
					
					float a = 0;
					
					float d = p.distance(lamps[i].pos);
					d = (radius-d)/radius;
					a = d;
					if(a > 1){
						a = 1;
					}
					if(a < 0){
						a = 0;	
					}
					
				/*	if(a < 0.1)
						a = 0;
					
					if(a < 0.5)
						a = 0;
					else 
						a = 1;*/
					lamps[i].a =255;
					lamps[i].r += ((r-lamps[i].r  )*a + (r2-lamps[i].r  )*(1.0-a) ) * 0.2;		
					lamps[i].g += ((g-lamps[i].g  )*a + (g2-lamps[i].g  )*(1.0-a) ) * 0.2;		
					lamps[i].b += ((b-lamps[i].b  )*a + (b2-lamps[i].b  )*(1.0-a) ) * 0.2;		


					
					
					if(lamps[i].a > 254){
						lamps[i].a = 254;
					}
					if(lamps[i].r > 254){
						lamps[i].r = 254;
					}				
					if(lamps[i].g > 254){
						lamps[i].g = 254;
					}
					if(lamps[i].b > 254){
						lamps[i].b = 254;
					}
					unsigned char *mBuf= new unsigned char[3*4];
					int n;
					if(lamps[i].r != lamps[i].sentR){
						lamps[i].sentR = lamps[i].r;
						unsigned char *buffer = new unsigned char[3];
						buffer[0] = (unsigned char)255;
						buffer[1] = (unsigned char)lamps[i].channel;
						buffer[2] = (unsigned char)lamps[i].r;
						serial.writeBytes(buffer, 3);
						ok = false;
					}
					if(lamps[i].g != lamps[i].sentG ){
						lamps[i].sentG = lamps[i].g;
						unsigned char *buffer = new unsigned char[3];
						buffer[0] = (unsigned char)255;
						buffer[1] = (unsigned char)lamps[i].channel+1;
						buffer[2] = (unsigned char)lamps[i].g;
						serial.writeBytes(buffer, 3);
						ok = false;
					}
					if(lamps[i].b != lamps[i].sentB){
						lamps[i].sentB = lamps[i].b;
						unsigned char *buffer = new unsigned char[3];
						buffer[0] = (unsigned char)255;
						buffer[1] = (unsigned char)lamps[i].channel+2;
						buffer[2] = (unsigned char)lamps[i].b;
						serial.writeBytes(buffer, 3);
						ok = false;
					}
					if(lamps[i].a != lamps[i].sentA){
						lamps[i].sentA = lamps[i].a;
						unsigned char *buffer = new unsigned char[3];
						buffer[0] = (unsigned char)255;
						buffer[1] = (unsigned char)lamps[i].channel+3;
						buffer[2] = (unsigned char)lamps[i].a;
						serial.writeBytes(buffer, 3);
						ok = false;
					}
				}
				/*	string s;
				 if(lamps[i].r != lamps[i].sentR){
				 lamps[i].sentR = lamps[i].r;
				 s += ofToString(lamps[i].channel, 0) + "c"+ofToString(lamps[i].r, 0)+"w";
				 }
				 if(lamps[i].g != lamps[i].sentG ){
				 lamps[i].sentG = lamps[i].g;
				 s += ofToString(lamps[i].channel+1, 0) + "c"+ofToString(lamps[i].g, 0)+"w";
				 }
				 
				 if(lamps[i].b != lamps[i].sentB){
				 lamps[i].sentG = lamps[i].g;
				 s +=  ofToString(lamps[i].channel+2, 0) + "c"+ofToString(lamps[i].b, 0)+"w";
				 }
				 if(lamps[i].a != lamps[i].sentA){
				 lamps[i].sentG = lamps[i].g;
				 s +=  ofToString(lamps[i].channel+3, 0) + "c"+ofToString(lamps[i].a, 0)+"w";
				 }	
				 serial.writeBytes((unsigned char*)s.c_str(), strlen(s.c_str()));
				 */
			}
		}
	}
}