//
//  motion.hpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#ifndef motion_hpp
#define motion_hpp

#include <list>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"

#include "particle.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class motion
{
public:
    list<particle *> particles;
    bool running, drawing;
    
    motion();
    motion(list<particle *> &);
    
    void update();
    void draw();
};

#endif