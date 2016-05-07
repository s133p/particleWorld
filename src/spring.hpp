//
//  spring.hpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#ifndef spring_hpp
#define spring_hpp

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"

#include "particle.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class spring
{
public:
    particle* a;
    particle* b;
    float k, d;
    float drawing;
    
    spring(particle*, particle*);
    
    void update(float iterations);
    void draw();
};

#endif /* spring_hpp */
