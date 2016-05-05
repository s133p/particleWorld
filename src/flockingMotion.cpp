//
//  flockingMotion.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#include "flockingMotion.hpp"

float sine(float x)
{
    x += M_PI/2.0;
    const float B = 4/M_PI;
    const float C = -4/(M_PI*M_PI);
    
    float y = B * x + C * x * abs(x);
    
//#ifdef EXTRA_PRECISION
      const float Q = 0.775;
    const float P = 0.225;
    
    y =  Q * y + P * y * abs(y);
//#endif
    return y;
}

flockPair::flockPair(particle* aa, particle* bb)
{
    a = aa;
    b = bb;
    this->tt = 0;
    l = 1000000;
    zoneRadiusSq = (a->radius*14)*(b->radius*14);
}

void flockPair::update()
{
    vec3 force = (a->position - b->position);
    l = length2(force);
    //ss = true;
    
    if (l > zoneRadiusSq)
    {
        this->tt = 0;
        return;
        
    }else{
        //this->tt = 0;
        float p = l/(zoneRadiusSq);
        
        float inp = .5;
        float midp = .7;
        float newl;
        
        
        if (p < inp){ //repel
            this->tt = 1;
            newl = p/inp;
            newl = sine( newl * M_PI) * 0.5 + 0.5 ;
            newl = newl*newl;
            newl *= .45;
            
            force = normalize(force)*newl;
            a->addForce(force);
            b->addForce(-force);
        }else if (p > midp){ // attract
            this->tt = 2;
            newl = p-midp;
            newl = ( 0.5 - sine( newl * M_PI * 2.0) * 0.5);
            newl *= .25;
            
            
            force = normalize(force)*newl;
            a->addForce(-force);
            b->addForce(force);
        }else{ //align
            this->tt = 3;
            newl = p-(inp);
            newl = newl/((midp)-(inp));
            newl = ( 0.5 - sine( newl * M_PI * 2.0) * 0.5);
            newl *= .015;
            
            force = b->velocity * newl;
            a->addForce(force);
            
            force = a->velocity * newl;
            b->addForce(force);
        }
    }
}

void flockPair::draw()
{
    //tt = 1;
    //vec3 force = (b->position - a->position);
    //float l = length2(force);
    //if (l < (zoneRadiusSq*.5)){
    float newl = l/zoneRadiusSq;
    if (newl > .4 && newl < .75){
        gl::drawLine(a->position, b->position);
    }
}


flockingMotion::flockingMotion()
{
    
}

flockingMotion::flockingMotion(list<particle*> & availableParticles) : motion(availableParticles)
{
    
    for (auto it = particles.begin(); it != particles.end(); it++)
    {
        auto it2 = it;
        it2++;
        for (; it2 != particles.end(); it2++)
        {
            flockPair fP( (*it), (*it2) );
            flock.push_back(fP);
        }
    }
    
}

void flockingMotion::update(float forceScale)
{
    if (!running) return;
    for (int i = 0; i < 1; i ++)
    {
        motion::saveForce();
        
        for (auto f = flock.begin(); f != flock.end(); f++)
        {
            (*f).update();
            
        }
        motion::update(forceScale);
        
        motion::avgForce();
    }
}

void flockingMotion::draw()
{
    if (!drawing) return;
    if (0)
    {
        //gl::enableAdditiveBlending();
        gl::color(.6, .6, .6);
        for (auto f = flock.begin(); f != flock.end(); f++)
        {
            (*f).draw();
        }
        //gl::disableBlending();
    }
    gl::color(.15, .15, .15);
    motion::draw();
}
