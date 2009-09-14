

#include "MoonDust.h"



MoonDust::MoonDust(){
	type = OUTPUT;
	
	for(int i=0;i<100;i++){
		particles.push_back(DustParticle(0,ofRandom(0, 200),0));
	}
	
	particleImg.loadImage(<#string fileName#>)
}


void MoonDust::update(){

}

void MoonDust::draw(){
	ofSetColor(255, 0, 0);
	ofRect(100, 100, 100, 100);
}


DustParticle::DustParticle(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}