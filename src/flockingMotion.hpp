//
//  flockingMotion.hpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#ifndef flockingMotion_hpp
#define flockingMotion_hpp

#include <list>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"

#include "motion.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class flockPair
{
public:
    particle * a;
    particle * b;
    int tt;
    float l;
    float zoneRadius;
    float zoneRadiusSq;
    
    flockPair(particle *, particle *);
    
    void update();
    void draw();
};

class flockingMotion : public motion
{
public:
    list<flockPair> flock;
    
    flockingMotion();
    flockingMotion( list<particle *> & );
    
    void update();
    void draw();
};



#endif /* flockingMotion_hpp */
