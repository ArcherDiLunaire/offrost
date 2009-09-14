

#include "MoonDust.h"



MoonDust::MoonDust(){
	type = OUTPUT;
	
	for(int i=0;i<100;i++){
		cout<<ofGetWidth()<<endl;
		particles.push_back(DustParticle(ofGetWidth()/2.0+ofRandom(-20,20),ofRandom(0, 200),0));
	}
	
	cout<<ofToDataPath("MoonDustParticle.png",true)<<endl;
	particleImg.loadImage("MoonDustParticle30.png");
	force = 1;
	damp = 0.999;
}


void MoonDust::update(){
    vector<DustParticle>::iterator it;
	cout<<force<<endl;
    it = particles.begin();
    while( it != particles.end() ) {
		(*it).update(force, damp);
		
		++it;
    }
}

void MoonDust::draw(){
	ofSetColor(255, 255, 255);
	
    vector<DustParticle>::iterator it;
	
    it = particles.begin();
    while( it != particles.end() ) {
		particleImg.draw((*it).x, (*it).y,10,10);

		++it;
    }
	
}


DustParticle::DustParticle(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
	v.x = ofRandom(-2, 2);
	v.y = ofRandom(-0.4, 0.4);

}

void DustParticle::update(float force, float damp){
	v *= damp;
	v.x += (ofGetWidth()/2.0 - x)*0.01*force;
	x += v.x;
}