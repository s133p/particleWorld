//
//  springMotion.hpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#ifndef springMotion_hpp
#define springMotion_hpp

#include <list>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"

#include "motion.hpp"
#include "spring.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class springMotion : public motion
{
public:
    list<spring> springs;
    
    springMotion();
    springMotion( list<particle *> & );
    
    void update(float forceScale);
    void draw();
};



#endif /* springMotion_hpp */
