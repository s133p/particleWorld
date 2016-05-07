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
    noise = Perlin(4, 123123);
    
    std::vector<vec3> positions;
    int i = 0;
    for (; i < 0/*MAX_PARTICLES*/; i++)
    {
        vec4 circ = vec4(0,1, 0, 0);
        mat4 ex = rotate( toRadians(i*(360/(float)MAX_PARTICLES )), vec3( 0, 0, 1 )  );
        ex = scale(ex, vec3(200,200, 1));
        circ = circ * ex;
        
        particleArray[i].position = vec3(circ.x, circ.y, randInt(0, 20));
        particleArray[i].prevPosition = particleArray[i].position+(randVec3()*vec3(1.0));
        particleArray[i].moving = true;
        particleArray[i].drawing = false;
        activeParticles.push_back(&particleArray[i]);
    }
    for (; i < MAX_PARTICLES; i++)
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
        positions.push_back(particleArray[i].position);
    }
    
    //Setup "Motions"
    test = new flockingMotion(inactiveParticles);
    test->running = true;
    spTest = new springMotion(inactiveParticles);
    
    //INSTANCTED DRAWING
    gl::GlslProgRef shader = gl::GlslProg::create( loadAsset( "shader.vert" ), loadAsset( "shader.frag" ) );
    gl::VboMeshRef mesh = gl::VboMesh::create( geom::Icosahedron() >> geom::Scale(2.0) ) ;
    
    mInstanceDataVbo = gl::Vbo::create( GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), positions.data(), GL_STREAM_DRAW );
    geom::BufferLayout instanceDataLayout;
    instanceDataLayout.append( geom::Attrib::CUSTOM_0, 3, 0, 0, 1 /* per instance */ );
    mesh->appendVbo( instanceDataLayout, mInstanceDataVbo );
    mBox = gl::Batch::create( mesh, shader, { { geom::Attrib::CUSTOM_0, "vInstancePosition" } } );
}

void particleController::update()
{
    //float testP = lerp( 0.0f, 0.4f, min(max(getElapsedFrames()-180.0f, 0.0f)/400.0f, 1.0f) );
    //test->update(testP);
    //spTest->update( lerp(0.0f, 0.95f, min((getElapsedFrames()/180.0f), 1.0f) ));
    
    test->update(.2);
    spTest->update(.98);
    //spTest->update(.95/2);
    
    //vec3 *positions = (vec3*)mInstanceDataVbo->map(GL_WRITE_ONLY);//mInstanceDataVbo->mapReplace();
    
    for (auto it : inactiveParticles)
    {
        if (it->moving)
        {
            //if (getElapsedSeconds() < 8)
            //{
                //it->addForce( noise.dfBm(it->position*0.01f) * 0.05f );
            //}
            
            it->update();
            //*positions = it->position;
            //positions++;
        }
    }
    //mInstanceDataVbo->unmap();
}

void particleController::draw()
{

    //gl::scale(vec3(0.9));
    gl::disableBlending();
    gl::enableDepth();
    
    spTest->draw();
    gl::color(.4, .4, .4);
    
    //mBox->drawInstanced( (MAX_PARTICLES-100) );
    
}






