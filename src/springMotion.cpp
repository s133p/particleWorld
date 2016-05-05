//
//  springMotion.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#include "springMotion.hpp"


springMotion::springMotion()
{
    
}

springMotion::springMotion(list<particle*> & availableParticles) : motion(availableParticles)
{
    for (auto it = particles.begin(); it != particles.end(); it++)
    {
        particle * p = new particle;
        p->position = vec3();
        p->moving = false;
        spring s( (*it), (p) );
        springs.push_back(s);
    }
}

void springMotion::update(float forceScale)
{
    if (!running) return;
    
    motion::saveForce();
    
    for (auto s : springs)
    {
        s.update(1.0);
    }
    
    motion::update(forceScale);
    
    motion::avgForce();
}

void springMotion::draw()
{
    /*
    if (!drawing) return;
    gl::color(.25, .25, .25);
    motion::draw();
     */
}
