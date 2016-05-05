//
//  particle.hpp
//  particleWorld
//
//  Created by Luke Purcell on 5/1/16.
//
//

#ifndef particle_hpp
#define particle_hpp

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class particle
{
public:
    vec3 position, prevPosition;
    vec3 force, sForce, velocity;
    float radius, mass;
    float lerpControl;
    
    bool moving;
    bool drawing;
    
    particle();
    
    void addForce(vec3);
    
    void update(); //Potentially add timestep?
    void draw();
};


#endif /* particle_hpp */
