//
//  flockingMotion.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#include "flockingMotion.hpp"


//Fast **COS** despite the name
float sine(float x)
{
    x += M_PI/2.0;
    const float B = 4/M_PI;
    const float C = -4/(M_PI*M_PI);
    
    float y = B * x + C * x * abs(x);
    
    const float Q = 0.775;
    const float P = 0.225;
    
    y =  Q * y + P * y * abs(y);
    return y;
}

flockPair::flockPair(particle* aa, particle* bb)
{
    a = aa;
    b = bb;
    l = 1000000;
    zoneRadiusSq = 20*20;//(a->radius*13)*(b->radius*13);
}

void flockPair::update()
{
    vec3 force = (a->position - b->position);
    l = length2(force);
    
    /*
    //Crowding
    float radiusAdjustA = 1.0f-(min((float)a->neighbors, 70.0f)/75.0f);
    //radiusAdjustA += 0.5f;
    float radiusAdjustB = 1.0f-(min((float)b->neighbors, 70.0f)/75.0f);
    //radiusAdjustB += 0.5f;
    zoneRadiusSq = (a->radius*30*radiusAdjustA)*(b->radius*30*radiusAdjustB);
    */
    
    if (l > zoneRadiusSq) return;
    a->neighbors++;
    b->neighbors++;

    float p = l/(zoneRadiusSq);
    float inp = .55;
    float midp = .75;
    float newl;
    
    if (p < inp){ //repel
        newl = p/inp;
        newl = sine( newl * M_PI) * 0.5 + 0.5 ;
        newl = newl*newl;
        newl *= .35;
        
        force = normalize(force)*newl;
        a->addForce(force);
        b->addForce(-force);
    }else if (p > midp){ // attract
        newl = p-midp;
        newl = ( 0.5 - sine( newl * M_PI * 2.0) * 0.5);
        newl *= .25;
        
        
        force = normalize(force)*newl;
        a->addForce(-force);
        b->addForce(force);
    }else{ //align
        newl = p-(inp);
        newl = newl/((midp)-(inp));
        newl = ( 0.5 - sine( newl * M_PI * 2.0) * 0.5);
        newl *= .05;
        
        force = b->velocity * newl;
        a->addForce(force);
        
        force = a->velocity * newl;
        b->addForce(force);
    }

}

void flockPair::draw()
{
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
    
    motion::saveForce();
    
    for (auto f = flock.begin(); f != flock.end(); f++)
    {
        (*f).update();
        
    }
    motion::update(forceScale);
    
    motion::avgForce();
    
    for (auto f = flock.begin(); f != flock.end(); f++)
    {
        (*f).a->neighbors = 0;
        (*f).b->neighbors = 0;
        
    }
}

void flockingMotion::draw()
{
    //motion::draw();
}
