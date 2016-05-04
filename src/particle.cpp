//
//  particle.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/1/16.
//
//

#include "particle.hpp"


particle::particle()
{
    position = vec3(0, 0, 0);
    radius = 2;
    mass = 2;
    
    moving = true;
    drawing = true;
    lerpControl = 0.75f;
}

void particle::addForce(vec3 f)
{
    force += f;
}

void particle::update()
{
    if (!moving) return;
    
    velocity = position - prevPosition;
    prevPosition = position;
    
    
    //force += vec3(0, -.01, 0);
    
    force /= mass;
    if (length2(force) > .1*.1) force = normalize(force)*0.1f;
    
    vec3 targetVelocity = velocity;
    
    targetVelocity += force;
    velocity = lerp(velocity, targetVelocity, lerpControl);
    velocity *= .99;
    if (length2(velocity) > 2*2) velocity = normalize(velocity)*2.0f;
    
    position += velocity;
    force = vec3();
}

void particle::draw()
{
    if (!drawing) return;
    
    gl::drawSphere(position, radius);
    //gl::drawSolidCircle(vec2(position.x, position.z), radius);
}