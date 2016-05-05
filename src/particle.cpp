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
    radius = randFloat(2.5,4.5);
    mass = radius*.4;
    
    moving = true;
    drawing = true;
    lerpControl = 0.85f;
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
    //if (length2(force) > .4*.4) force = normalize(force)*0.4f;
    
    vec3 targetVelocity = velocity;
    
    targetVelocity += force;
    velocity = lerp(velocity, targetVelocity, lerpControl);
    velocity *= .998;
    if (length2(velocity) > 6*6) velocity = normalize(velocity)*6.0f;
    
    position += velocity;
    force = vec3();
}

void particle::draw()
{
    if (!drawing) return;
    
    gl::drawCube(position, vec3(1)*radius);
    //gl::drawSolidCircle(vec2(position.x, position.z), radius);
}