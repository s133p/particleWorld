//
//  particleController.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/1/16.
//  COPYRIGHT 2016
//

#include <list>
#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/Timeline.h"

#include "particleController.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class particleWorldApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void keyUp(KeyEvent event) override;
	void update() override;
	void draw() override;
    
    particleController particles;
    CameraPersp cam;
    Surface voidImg, instinctImg;
    gl::Texture2dRef voidTex;
    
    vector<vec2> instinctPoints;
    vector<vec2> voidPoints;
    gl::GlslProgRef shader;
    bool running;
	bool mFullscreen;
    
    Anim<float> logoAlpha, logoScale, logoOffset;
};

void particleWorldApp::setup()
{
    //hideCursor();
    logoScale = 0.0f;
    logoAlpha = 0.0f;
    logoOffset = 0.0;
    
    timeline().apply( &logoScale, 0.85f, 1.5f, EaseInOutQuad());
    timeline().apply( &logoAlpha, 1.0f, 1.5f, EaseInOutQuad());

    
    timeline().apply( &logoOffset, 900.0f, 1.0f, EaseInBack()).timelineEnd();
    
    timeline().appendTo( &logoAlpha, 0.0f, 1.0f, EaseOutAtan() );
    timeline().appendTo( &logoScale, 1.1f, 1.0f, EaseOutCirc() );
    
    running = true;
	mFullscreen = false;
    
	shader = gl::GlslProg::create(loadAsset("shader.vert"), loadAsset("shader.frag"));
    
    cam = CameraPersp(getWindowWidth(), getWindowHeight(), 35);
    cam.setFarClip(20000);
    
	voidImg = loadImage(loadAsset("VoidResearch.png"));

    gl::enableDepthWrite();
    gl::enableDepthRead();
}

void particleWorldApp::mouseDown(MouseEvent event)
{
	running = !running;
}

void particleWorldApp::keyUp(KeyEvent event)
{
	if(event.getCode() == KeyEvent::KEY_ESCAPE){
		quit();
	}if(event.getCode() == KeyEvent::KEY_f){
		mFullscreen = !mFullscreen;
		setFullScreen(mFullscreen);
	}
}

void particleWorldApp::update()
{
    if (running)
    {
        particles.update();
    }

	if(app::getElapsedFrames() == (32 * 116) + 90){
		quit();
	}
}

void particleWorldApp::draw()
{
    gl::clear( Color( .98, .98, .98 ) );
    
	cam = CameraPersp(getWindowWidth(), getWindowHeight(), 35);
	cam.setFarClip(20000);
    gl::setMatrices(cam);
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    gl::translate(getWindowWidth()/2, getWindowHeight()/2, -1000);
    gl::rotate(toRadians(-16.0f + app::getElapsedFrames()/-2.0f), vec3(0,1,0));
    
    
    //Draw "floor" with scoped fog shader
    if(1){
        gl::pushMatrices();
        
        gl::ScopedGlslProg shade(shader);
        gl::translate(0,-200-getWindowHeight()/2, 0);
        gl::color(.75, .75, .75);
        gl::drawCube(vec3(), vec3(20000, 5,20000));
        gl::popMatrices();
    }
    
    //Draw particle controller
    particles.draw();
    
    
    //Draw logo ^_^
    if (logoAlpha > 0.001)
    {
        gl::disableDepthWrite();
        gl::enableAlphaBlending();
        gl::color(0, 0, 0, logoAlpha);
        gl::TextureRef tex = gl::Texture2d::create(voidImg);
        Rectf x( 0, 0, voidImg.getWidth(), voidImg.getHeight() );
        x.scale( vec2( logoScale, -logoScale ) );
        x.offsetCenterTo( vec2(0, -logoOffset) );
        gl::draw(tex, x );
        gl::disableAlphaBlending();
    }
}

CINDER_APP( particleWorldApp, RendererGl( RendererGl::Options().msaa(8) ), [&]( App::Settings *settings ) {
    
    settings->setWindowSize(1280, 800);
	//vector<DisplayRef> displays = Display::getDisplays();

	//if(displays.size() > 1) {

		//settings->setDisplay(displays[1]);
	//}
    //settings->setFullScreen();
    //settings->setHighDensityDisplayEnabled();
    settings->setFrameRate(60.0);
    
} )