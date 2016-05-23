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
    drawing = true;
    running = true;
}

springMotion::springMotion(list<particle*> & availableParticles) : motion(availableParticles)
{
    drawing = false;
}

void springMotion::addSpring(particle * a, particle * b)
{
    spring s(a, b);
    springs.push_front(s);
}

void springMotion::update(float forceScale)
{
    if (!running) return;
    
    motion::saveForce();
    
    for (auto & s : springs)
    {
        s.update(forceScale);
    }
    
    motion::update(forceScale);
    
    motion::avgForce();
}

void springMotion::draw()
{
    
    if (!drawing) return;
    
    for (auto & s : springs)
    {
        s.draw();
    }
    
}
