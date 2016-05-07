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
    radius = 4;//randFloat(3.5,5.5);
    mass = radius*.6;
    
    moving = true;
    drawing = true;
    lerpControl = 0.8f;
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
    
    
    //force += vec3(0, -.04, 0); //GRAVITY
    vec3 grav = vec3()-position;
    grav = normalize(grav) * 0.15f;
    force += grav;
    
    force /= mass;
    //if (length2(force) > .4*.4) force = normalize(force)*0.4f;
    
    //force *= vec3(1,1,0); //limit force dimentions
    
    vec3 targetVelocity = velocity;
    
    targetVelocity += force;
    velocity = lerp(velocity, targetVelocity, lerpControl);
    velocity *= .998;
    if (length2(velocity) > 8*8) velocity = normalize(velocity)*8.0f;
    
    position += velocity;
    force = vec3();
}

void particle::draw()
{
    if (!drawing) return;
    
    //gl::drawCube(position, vec3(1)*radius);
    //gl::drawSolidCircle(vec2(position.x, position.z), radius);
}