//
//  particleController.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/1/16.
//
//

#include "particleController.hpp"


particleController::particleController()
{
    int i = 0;
    noise = Perlin(4, 123123);
    
    std::vector<vec4> positions;
    //for (i=0; i < MAX_PARTICLES; i++) positions.push_back( vec4(0,0,0,2) );
    
    //Set up Initial state
    for (i=0; i < MAX_PARTICLES; i++)
    {
        vec4 circ = vec4(0,1, 0, 0);
        mat4 ex = rotate( toRadians(i*(360/(float)MAX_PARTICLES )), vec3( 0, 0, 1 )  );
        ex = scale(ex, vec3(400,400, 1));
        circ = circ * ex;
        
        particleArray[i].position = vec3(circ.x, circ.y, randInt(0, 100));
        particleArray[i].prevPosition = particleArray[i].position+(randVec3()*vec3(1.5));
        particleArray[i].moving = true;
        particleArray[i].drawing = false;
        
        inactiveParticles.push_back(&particleArray[i]);
        vec4 p = vec4(particleArray[i].position, particleArray[i].radius);
        positions.push_back( p );
    }
    
    //Setup "Motions"
    test = new flockingMotion(inactiveParticles);
    test->running = true;
    spTest = new springMotion(inactiveParticles);
    
    //INSTANCTED DRAWING
    shader = gl::GlslProg::create( loadResource( "shader.vert" ), loadResource( "shader.frag" ) );
    gl::VboMeshRef mesh = gl::VboMesh::create( geom::Cube() >> geom::Scale(2.0) ) ;
    
    mInstanceDataVbo = gl::Vbo::create( GL_ARRAY_BUFFER, positions.size() * sizeof(vec4), positions.data(), GL_STREAM_DRAW );
    geom::BufferLayout instanceDataLayout;
    instanceDataLayout.append( geom::Attrib::CUSTOM_0, 4, sizeof(vec4), 0, 1 /* per instance */ );
    mesh->appendVbo( instanceDataLayout, mInstanceDataVbo );
    mBox = gl::Batch::create( mesh, shader, { { geom::Attrib::CUSTOM_0, "vInstancePosition" } } );
}

void particleController::update()
{
    //float testP = lerp( 0.0f, 0.4f, min(max(getElapsedFrames()-180.0f, 0.0f)/400.0f, 1.0f) );
    //test->update(testP);
    //spTest->update( lerp(0.0f, 0.95f, min((getElapsedFrames()/180.0f), 1.0f) ));
    
    test->update(.85);
    spTest->update(.95);
    //spTest->update(.95/2);
    
    vec4 *positions = (vec4*)mInstanceDataVbo->map(GL_WRITE_ONLY);//mInstanceDataVbo->mapReplace();
    
    for (auto it : inactiveParticles)
    {
        if (it->moving)
        {
            //if (getElapsedSeconds() < 8)
            //{
                it->addForce( noise.dfBm(it->position*0.03f) * 0.06f );
            //}
            
            it->update();
            *positions = vec4(it->position, it->radius);
            
            positions++;
        }
    }
    mInstanceDataVbo->unmap();
}

void particleController::draw()
{

    //gl::scale(vec3(0.9));
    gl::disableBlending();
    gl::enableDepth();
    
    //gl::color( .184, .671, 1.0);
    //gl::drawSphere(vec3(), 200);
    
    //spTest->draw();
    shader->bind();
    test->draw();
    gl::color(.4, .4, .4);
    
    mBox->drawInstanced( inactiveParticles.size() );
    
}






