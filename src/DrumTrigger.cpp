//
//  DrumTrigger.cpp
//  Kinect2Skeleton
//
//  Created by Max on 08/11/2020.
//

#include "DrumTrigger.hpp"

DrumTrigger::DrumTrigger(ofxMidiOut& midiOut, int posX, int posY, int width, int height, int channel, int note)
    : midiOut(midiOut), channel(channel), note(note){
        // Create bounding box
        boundingRect.set(posX, posY, width, height);
        velocity = 127;
        
        // Collision map for keeping track of which hand touches the rectangle
        // 2 users max
        collisionMap.push_back(false);
        collisionMap.push_back(false);
        prevCollisionMap.push_back(false);
        prevCollisionMap.push_back(false);
        
        collisionMap.push_back(false);
        collisionMap.push_back(false);
        prevCollisionMap.push_back(false);
        prevCollisionMap.push_back(false);
}

void DrumTrigger::update(vector<ofVec2f>& hands, int userID){
    // Check for collision
    isCollision = false;
    
    for(int i = 0; i < hands.size(); i++){
        ofVec2f handData = hands[i];
        handRect.set(handData.x, handData.y, 1, 1);
        
        if(userID == 0){
            collisionMap[i] = boundingRect.intersects(handRect);
        } else {
            collisionMap[i + 2] = boundingRect.intersects(handRect);
        }
    }
    
    for(int i = 0; i < hands.size(); i++){
        bool isCollision = false;
        if(userID == 0){
            isCollision = collisionMap[i];
        } else {
            isCollision = collisionMap[i + 2];
        }
        
        for(bool c : prevCollisionMap){
            if(isCollision == c){
                isCollision = false;
                break;
            }
        }
        
        if(isCollision && !isNoteOn){
            // Send MIDI trigger
            midiOut.sendNoteOn(channel, note, velocity);
            
            // Avoid instant retrigger
            isNoteOn = true;
            timeSinceNoteOn = 0.0f;
            
            break;
        }
    }
    
    // Note off if time elapsed
    if(isNoteOn){
        timeSinceNoteOn += 1.0 / frameRateForCapture;
        if (timeSinceNoteOn > RETRIGGER_TIME){
            // Send note off event
            midiOut.sendNoteOff(channel, note, velocity);

            isNoteOn = false;
        }
    }
    
    for(int i = 0; i < hands.size(); i++){
        if(userID == 0){
            prevCollisionMap[i] = collisionMap[i];
        } else {
            prevCollisionMap[i + 2] = collisionMap[i + 2];
        }
    }
}

void DrumTrigger::draw(){
    ofDrawRectangle(boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height);
};
