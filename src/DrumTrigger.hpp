//
//  DrumTrigger.hpp
//  Kinect2Skeleton
//
//  Created by Max on 08/11/2020.
//
#pragma once

#ifndef DrumTrigger_hpp
#define DrumTrigger_hpp

#include "ofMain.h"
#include "ofxMidi.h"

class DrumTrigger {
public:
    DrumTrigger(ofxMidiOut& midiOut, int posX, int posY, int width, int height, int channel, int note);
    
    void update(vector<ofVec2f>& hands, int userID);
    
    void draw();
    
private:
    ofxMidiOut& midiOut;
    ofRectangle boundingRect;
    ofRectangle handRect;

    int channel, note, velocity;
    bool isNoteOn = false;
    bool isCollision = false;
    float timeSinceNoteOn = 0.0f;
    
    float frameRateForCapture = 60.0f;
    float lastTime = 0.0f;
    
    // Time in seconds to wait before retriggering is allowed
    const float RETRIGGER_TIME = 0.1;
    // Contains the collision maps for each user
    vector<bool> collisionMap;
    vector<bool> prevCollisionMap;
};

#endif /* DrumTrigger_hpp */
