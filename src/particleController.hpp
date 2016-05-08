//
//  particleController.hpp
//  particleWorld
//
//  Created by Luke Purcell on 5/1/16.
//
//

/*
    
 
 */

#ifndef particleController_hpp
#define particleController_hpp

#include <list>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"

#include "springMotion.hpp"
#include "flockingMotion.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

#define MAX_PARTICLES 800

class particleController
{
public:
    gl::VboRef			mInstanceDataVbo;
    gl::BatchRef	mBox;
    gl::GlslProgRef shader;
    
    particle particleArray[MAX_PARTICLES];
    list<particle *> activeParticles, inactiveParticles;
    Perlin noise;
    flockingMotion * test;
    springMotion * spTest;
    
    particleController();
    
    void update();
    void draw();
};

#endif /* particleController_hpp */
