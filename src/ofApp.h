#pragma once

#include "ofMain.h"
#include "ofxNI2.h"
#include "ofxNiTE2.h"
#include "ofxOsc.h"
#include "ofxMidi.h"
#include "DrumTrigger.hpp"
#include "ParameterSlider.hpp"
#include "ModeSwitch.hpp"
#include "NiTE.h"
#include "MyNewFrameListener.hpp"
#include <mutex>

using namespace std;

// OSC port
#define OSC_PORT 1234

class ofApp : public ofBaseApp{

public:
    ofApp(ofxNI2::Device& device,
          ofxNiTE2::UserTracker& tracker,
          const nite::UserTracker& userTracker
          );
    
    void setup();
    void exit();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void updateHands(int userID);
    void userLogic();
    
    mutex mtx;

    ofxNI2::Device& device;
    ofxNiTE2::UserTracker& tracker;
    const nite::UserTracker& userTracker;
    
    float userDepth = -1.0f;
        
    ofPixels depthPixels;
    ofTexture depthTexture;
    
    ofPixels colourPixels;
    ofImage colourImage;
    
    // User mask i.e. pixels on screen that belong to user
    ofImage userMask;

    ofRectangle handRect;
    
    ofSoundPlayer mySound;
    
    // OSC
    ofxOscSender oscSender;
    float timeSent;
    
    // MIDI
    ofxMidiOut midiOut;
    int channel;
    
    unsigned int currentPgm;
    int note, velocity;
    float isNoteOn;
    float timeSinceNoteOn;
    
    float frameRateForCapture;
    float lastTime;  
    
private:
    vector<ofVec2f> hands;
    vector<unique_ptr<DrumTrigger>> drumTriggers;
    vector<unique_ptr<ParameterSlider>> parameterSliders;
    unique_ptr<ModeSwitch> modeSwitch;
    
    // Whether we're in drum trigger or parameter mode
    bool isPlayMode = true;
    
    int torsoGreyValue = -1;
};
