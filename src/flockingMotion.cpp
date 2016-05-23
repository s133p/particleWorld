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
    
    float y = B * x + C * x * glm::abs(x);
    
    const float Q = 0.775;
    const float P = 0.225;
    
    y =  Q * y + P * y * glm::abs(y);
    return y;
}

flockPair::flockPair(particle* aa, particle* bb)
{
    a = aa;
    b = bb;
    l = 1000000;
    const float ZONE_MULT = 30.0;
    zoneRadiusSq = (a->radius*a->radius*ZONE_MULT)*(b->radius*b->radius*ZONE_MULT);
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
    //a->neighbors++;
    //b->neighbors++;

    float p = l/(zoneRadiusSq);
    float inp  = 1.0;//0.55;
    float midp = 0.8;
    float newl;
    
    if (p < inp){ //repel
        newl = p/inp;
        newl = glm::cos( newl * M_PI) * 0.5 + 0.5 ;
        //newl = newl*newl;
        newl *= .45;
        
        force = normalize(force)*newl;
        a->addForce(force);
        b->addForce(-force);
    }/*else if (p > midp){ // attract
        newl = p-midp;
        newl = ( 0.5 - glm::cos( newl * M_PI * 2.0) * 0.5);
        newl *= .45;
        
        
        force = normalize(force)*newl;
        a->addForce(-force);
        b->addForce(force);
    }  else{ //align
        newl = p-(inp);
        newl = newl/((midp)-(inp));
        newl = ( 0.5 - glm::cos( newl * M_PI * 2.0) * 0.5);
        newl *= .04;
        
        force = b->velocity * newl;
        a->addForce(force);
        
        force = a->velocity * newl;
        b->addForce(force);
    }*/

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
    
    for (auto it = particles.begin(); it != particles.end(); ++it)
    {
        auto it2 = it;
        ++it2;
        for (; it2 != particles.end(); ++it2)
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
    
    for (auto f = flock.begin(); f != flock.end(); ++f)
    {
        (*f).update();
        
    }
    
    /*for (auto p : particles)
    {
        static const float MAX_FORCE = .27;
        p->force = p->force - p->velocity;
        if (length2(p->force) > MAX_FORCE*MAX_FORCE)
            p->force = normalize(p->force)*MAX_FORCE;
        else
            p->force *= forceScale;
            
    }*/
    motion::update(forceScale);
    
    motion::avgForce();
}

void flockingMotion::draw()
{
    //motion::draw();
}
