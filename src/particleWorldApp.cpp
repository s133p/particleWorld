#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"

#include "particleController.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class particleWorldApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    particleController particles;
    CameraPersp cam;
};

void particleWorldApp::setup()
{
    cam = CameraPersp(1280, 800, 35);
}

void particleWorldApp::mouseDown( MouseEvent event )
{
}

void particleWorldApp::update()
{
    particles.update();
}

void particleWorldApp::draw()
{
    gl::setMatrices(cam);
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::color(1, 1, 1);
    gl::translate(getWindowWidth()/2, getWindowHeight()/2, 0);
    gl::rotate(toRadians(app::getElapsedSeconds()*20.0f), vec3(0,1,0));
    
    particles.draw();
}

CINDER_APP( particleWorldApp, RendererGl( RendererGl::Options().msaa(4) ), [&]( App::Settings *settings ) {
    
    settings->setWindowSize(1280, 800);
    //settings->setHighDensityDisplayEnabled();
    settings->setFrameRate(60.0);
    
} )