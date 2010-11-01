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
	
	isOldAndSucks = false;
	
	channel = 0;
}

void lamp::update(){
	
}

LEDGridThread::LEDGridThread(){
	
	r = 255;
	g = 255;
	b = 255;
	r2 = 0;
	g2 = 0;
	b2 = 0;
	
	p.x = 0;
	p.y = 0;
	
	master = 1.0;
	sentMaster = 0.0;
	
	connected = serial.setup("/dev/tty.usbserial-A6008iyw", 115200);
	
	//Grid goes from front stage in reading direction, with  locations 0-1 in both x and y axis
	
	int channel = 1;
	for(int y=6; y>=0; y--){
		float xCount = 8;
		if(y == 0)
			xCount = 4;
		if(y == 1)
			xCount = 6;
		if(y == 2)
			xCount = 7;
		
		for(int x=0;x<xCount; x++){
			lamp l;
			l.pos.x = x/7.0;
			l.pos.y = y/5.0;
			l.channel = channel;
			channel += 4;
			l.isOldAndSucks = true;
			//	cout<<"Lamp pos "<<l.pos.x<<","<<l.pos.y<<" ch. "<<l.channel<<endl;
			
			lamps.push_back(l);
		}
		
	}
	
	/*float sx = 0.28 - dx;
	 float y = 0.25;
	 
	 for(int i=0;i<6;i++){
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
	 
	 // dmx adresses - andreas' grid is fun in the first row, so we override at i=0 for these exceptions
	 // we start from the stage back
	 
	 for(int i=0;i<5;i++){
	 lamps[i].channel = 217+i*4;
	 if(i==0) lamps[i].channel = 2;
	 }
	 for(int i=0;i<6;i++){
	 lamps[8+i].channel = 177+i*4;
	 if(i==0) lamps[8+i].channel = 6; 
	 }
	 for(int i=0;i<8;i++){
	 lamps[8*2+i].channel = 147+i*4;	
	 if(i==0) lamps[8*2+i].channel = 10; 
	 if(i < 4 && i > 0){
	 lamps[8*2+i].isOldAndSucks = true;
	 }
	 }
	 for(int i=0;i<8;i++){
	 lamps[8*3+i].channel = 117+i*4;	
	 if(i==0) lamps[8*3+i].channel = 14; 
	 if(i < 4){
	 lamps[8*3+i].isOldAndSucks = true;
	 }
	 }
	 for(int i=0;i<8;i++){
	 lamps[8*4+i].channel = 87+i*4;
	 if(i < 4){
	 lamps[8*4+i].isOldAndSucks = true;
	 }
	 }
	 for(int i=0;i<8;i++){
	 lamps[8*5+i].channel = 37+i*4;
	 lamps[8*5+i].isOldAndSucks = true;
	 }
	 */
	ok = true;
	
	alphaSet = false;
	
}


void LEDGridThread::start(){
	if(connected){
		startThread(false, false);   // blocking, verbose
	}
}

void LEDGridThread::stop(){
	stopThread();
}

void LEDGridThread::threadedFunction(){
	
	while( isThreadRunning() != 0 ){
		
		if(serial.available()){
			serial.flush(true, false);
			ok = true;
			byteCounter = 0;
		}
		if(ok){
			for(int i=0;i<lamps.size();i++){
				//				cout << i << ": " << lamps[i].r << endl;
				if(lamps[i].channel > 0){
					//				ofxPoint2f p = ofxPoint2f(mouseX, mouseY);
					
					if( lock() ){
						
						float a = 0;
						
						ofxPoint2f calibratedLampPosition = lamps[i].pos * ofxPoint2f(ledGridObject->pt2x*2-ledGridObject->pt1x, ledGridObject->pt2y-ledGridObject->pt1y) + ofxPoint2f(ledGridObject->pt1x, ledGridObject->pt1y);
						//ofxPoint2f calibratedLampPosition = lamps[i].pos;
						float d = p.distance(calibratedLampPosition);
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
						
						lamps[i].a += (master*254*a - lamps[i].a) *0.032;
						//lamps[i].a = 255*a;
						
						/*lamps[i].r += ((r-lamps[i].r  )*a + (r2-lamps[i].r  )*(1.0-a) ) * 0.092;		
						 lamps[i].g += ((g-lamps[i].g  )*a + (g2-lamps[i].g  )*(1.0-a) ) * 0.092;		
						 lamps[i].b += ((b-lamps[i].b  )*a + (b2-lamps[i].b  )*(1.0-a) ) * 0.092;		
						 */
						
						//Sender kun alpha kanal, for så styrer MA farven
						lamps[i].r = 0;
						lamps[i].g = 0;
						lamps[i].b = 0;
						
						//cout<<ledGridObject->debug<<endl;
						if(debugFlag){
 							lamps[i].a = 0;
							/*							lamps[i].r = 0;
							 lamps[i].g = 0;
							 lamps[i].b = 0;*/
							
							//(	cout<<"debug"<<endl;
							lamps[3].a = 255;
							
							lamps[45].a = 255;
						}
						
						if(lamps[i].a > 254){
							lamps[i].a = 254;
						}
						
						/*	if(lamps[i].isOldAndSucks){
						 lamps[i].a = (lamps[i].a * (190.0) / 255.0) ;
						 }
						 */
						if(lamps[i].r > 254){
							lamps[i].r = 254;
						}				
						if(lamps[i].g > 254){
							lamps[i].g = 254;
						}
						if(lamps[i].b > 254){
							lamps[i].b = 254;
						}
						
						if(!ledGridObject->enabled){
							lamps[i].r = 0;
							lamps[i].g = 0;
							lamps[i].b = 0;
							lamps[i].a = 0;
						}
						
						unlock();
						
					}
					
					//unsigned char *mBuf= new unsigned char[3*4];
					if(byteCounter < 115){
						int n;
						if(master != sentMaster){
							sentMaster = master;
							unsigned char *buffer = new unsigned char[3];
							buffer[0] = (unsigned char)255;
							buffer[1] = (unsigned char)0;
							buffer[2] = (unsigned char)round(master*254);
							serial.writeBytes(buffer, 3);
							delete buffer;
							ok = false;
							
							byteCounter+=3;
						}
						if(lamps[i].r != lamps[i].sentR){
							lamps[i].sentR = lamps[i].r;
							unsigned char *buffer = new unsigned char[3];
							buffer[0] = (unsigned char)255;
							buffer[1] = (unsigned char)lamps[i].channel;
							buffer[2] = (unsigned char)lamps[i].r;
							serial.writeBytes(buffer, 3);
							delete buffer;
							ok = false;
							
							byteCounter+=3;
						}
						if(lamps[i].g != lamps[i].sentG ){
							lamps[i].sentG = lamps[i].g;
							unsigned char *buffer = new unsigned char[3];
							buffer[0] = (unsigned char)255;
							buffer[1] = (unsigned char)lamps[i].channel+1;
							buffer[2] = (unsigned char)lamps[i].g;
							serial.writeBytes(buffer, 3);
							delete buffer;
							ok = false;
							
							byteCounter+=3;
						}
						if(lamps[i].b != lamps[i].sentB){
							lamps[i].sentB = lamps[i].b;
							unsigned char *buffer = new unsigned char[3];
							buffer[0] = (unsigned char)255;
							buffer[1] = (unsigned char)lamps[i].channel+2;
							buffer[2] = (unsigned char)lamps[i].b;
							serial.writeBytes(buffer, 3);
							delete buffer;
							ok = false;
							
							byteCounter+=3;
						}
						if(lamps[i].a != lamps[i].sentA){
							lamps[i].sentA = lamps[i].a;
							unsigned char *buffer = new unsigned char[3];
							buffer[0] = (unsigned char)255;
							buffer[1] = (unsigned char)lamps[i].channel+3;
							buffer[2] = (unsigned char)lamps[i].a;
							serial.writeBytes(buffer, 3);
							delete buffer;
							ok = false;
							
							byteCounter+=3;
							
							/*if(!alphaSet){
							 alphaSet = true;
							 unsigned char *buffer = new unsigned char[3];
							 buffer[0] = (unsigned char)255;
							 buffer[1] = (unsigned char)254;
							 buffer[2] = (unsigned char)lamps[i].channel+3;
							 serial.writeBytes(buffer, 3);
							 delete buffer;
							 ok = false;
							 
							 byteCounter+=3;
							 }*/
						}
					}
				}
			}
		}
		ofSleepMillis(1);	
	}	
	//
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
	
	p.x = 0;
	p.y = 0;
	
	master = 1.0;
	
	debug = false;
	
}

#pragma mark Callback methods

void LEDGrid::setup(){
	
	updateThreadValues();
	serialThread.ledGridObject = this; 
	serialThread.start();
	
	filterx = new Filter();	
	filterx->setNl(9.413137469932821686e-04, 2.823941240979846506e-03, 2.823941240979846506e-03, 9.413137469932821686e-04);
	filterx->setDl(1, -2.5818614306773719263, 2.2466666427559748864, -.65727470210265670262);
	
	filtery = new Filter();	
	filtery->setNl(9.413137469932821686e-04, 2.823941240979846506e-03, 2.823941240979846506e-03, 9.413137469932821686e-04);
	filtery->setDl(1, -2.5818614306773719263, 2.2466666427559748864, -.65727470210265670262);
	
	
	
}

void LEDGrid::draw(){
	
}

void LEDGrid::drawOnFloor(){
	if(debug){
		ofSetColor(255, 255, 255);
		ofFill();
		ofCircle(pt1x*projection()->getFloor()->aspect, pt1y, 0.02);
		ofCircle(pt2x*projection()->getFloor()->aspect, pt2y, 0.02);
		
	}
	/**
	 if (debug) {
	 for(int i=0;i<lamps.size();i++){
	 ofSetColor(lamps[i].r,lamps[i].g, lamps[i].b);
	 ofFill();
	 float a = lamps[i].b + lamps[i].g + lamps[i].r;
	 a /= 255.0*3.0;
	 ofEllipse(lamps[i].pos.x, lamps[i].pos.y, 0.1*a,  0.1*a)	;
	 ofSetColor(255, 255, 255);
	 ofNoFill();
	 ofEllipse(lamps[i].pos.x, lamps[i].pos.y, 0.1,  0.1)	;
	 //ofDrawBitmapString( ofToString(lamps[i].channel), lamps[i].pos.x, lamps[i].pos.y);
	 }
	 }
	 **/
}

void LEDGrid::update(){
	
	updateThreadValues();
	
}

void LEDGrid::updateThreadValues(){
	
	if(blob(0)->numPersistentBlobs() > 0){
		ofxPoint2f pDst = blob(0)->persistentBlobs[0].getLowestPoint();
		pDst = projection()->convertToFloorCoordinate(pDst);
		
		if(pDst.x > 0 
		   && pDst.x < projection()->getFloor()->aspect 
		   && pDst.y > 0
		   && pDst.y < 1
		   ){
			
			if(p.distance(pDst) > 0.1){				
				ofxVec2f v = pDst - p;
				v.limit(0.1);
				pDst = p + v;
			}
			//		p = pDst;
			p.x = filterx->filter(pDst.x);
			p.y = filtery->filter(pDst.y);
			
		}
	}	
	
	if(serialThread.lock()){
		
		serialThread.r = r;
		serialThread.g = g;
		serialThread.b = b;
		serialThread.r2 = r2;
		serialThread.g2 = g2;
		serialThread.b2 = b2;
		
		serialThread.p.x = p.x;
		serialThread.p.y = p.y;
		
		serialThread.master = master;
		serialThread.radius = radius;
		
		serialThread.debugFlag = debug;
		
		serialThread.unlock();
		
	}	
	
}