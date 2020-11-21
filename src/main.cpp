#include "ofMain.h"
#include "ofApp.h"
#include "NiTE.h"

//========================================================================
int main(){
	ofSetupOpenGL(640, 480, OF_WINDOW);			// <-------- setup the GL context

    ofxNI2::Device device;
    ofxNiTE2::UserTracker tracker;
        
    device.setLogLevel(OF_LOG_VERBOSE);
    device.setup(0);
    tracker.setup(device);
    tracker.enableTrackingOutOfFrame(true);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    ofRunApp(new ofApp(device,
                       tracker,
                       tracker.get()
                       ));

}
