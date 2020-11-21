//
//  ParameterSlider.cpp
//  Kinect2Skeleton
//
//  Created by Max on 09/11/2020.
//

#include "ParameterSlider.hpp"

ParameterSlider::ParameterSlider(ofxMidiOut& midiOut, int posX, int posY, int width, int height, int channel, int cc)
    : midiOut(midiOut), channel(channel), midiCC(cc){
        // Create bounding box
        boundingRect.set(posX, posY, width, height);
        
        // Collision map for keeping track of which hand touches the rectangle
        collisionMap.push_back(false);
        collisionMap.push_back(false);
        prevCollisionMap.push_back(false);
        prevCollisionMap.push_back(false);
        
        // User 2
        collisionMap.push_back(false);
        collisionMap.push_back(false);
        prevCollisionMap.push_back(false);
        prevCollisionMap.push_back(false);
}

void ParameterSlider::update(vector<ofVec2f>& hands, int userID){
    for(int i = 0; i < hands.size(); i++){
        ofVec2f handData = hands[i];
        handRect.set(handData.x, handData.y, 1, 1);
        
        int idx = userID == 0 ? i : i + 2;
        collisionMap[idx] = boundingRect.intersects(handRect);
    }
    
    for(int i = 0; i < hands.size(); i++){
        int idx = userID == 0 ? i : i + 2;
        bool isCollision = collisionMap[idx];
        
        if(isCollision){
            // Get value by mapping position of hand in slider to [0...127]
            int value = ofClamp(ofMap(hands[i].x, boundingRect.x, boundingRect.x + boundingRect.width, 0, 127), 0, 127);
            // Send MIDI CC
            midiOut.sendControlChange(channel, midiCC, value);
            break;
        }
    }
        
    // Update previous collision map
    for(int i = 0; i < hands.size(); i++){
        int idx = userID == 0 ? i : i + 2;
       prevCollisionMap[idx] = collisionMap[idx];
    }
}

void ParameterSlider::draw(){
    ofDrawRectangle(boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height);
};
