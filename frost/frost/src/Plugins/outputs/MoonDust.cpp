

#include "MoonDust.h"



MoonDust::MoonDust(){
	type = OUTPUT;
	
	for(int i=0;i<100;i++){
		particles.push_back(DustParticle(0,ofRandom(0, 200),0));
	}
	
	cout<<ofToDataPath("MoonDustParticle.png",true)<<endl;
	particleImg.loadImage("MoonDustParticle30.png");
}


void MoonDust::update(){

}

void MoonDust::draw(){
	ofSetColor(255, 255, 255);
	particleImg.draw(0, 0,30,30);
}


DustParticle::DustParticle(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}