//
//  ModeSwitch.cpp
//  Kinect2Skeleton
//
//  Created by Max on 09/11/2020.
//

#include "ModeSwitch.hpp"

ModeSwitch::ModeSwitch(bool& isPlayMode, int posX, int posY, int width, int height)
: isPlayMode(isPlayMode){
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

void ModeSwitch::update(vector<ofVec2f>& hands, int userID){
    // Check for collision
    bool isCollision = false;
    
    for(int i = 0; i < hands.size(); i++){
        ofVec2f handData = hands[i];
        handRect.set(handData.x, handData.y, 1, 1);
        
        int idx = userID == 0 ? i : i + 2;
        collisionMap[idx] = boundingRect.intersects(handRect);
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
        
        if(isCollision){
            isPlayMode = !isPlayMode;
            break;
        }
    }
    
    // Update collision map
    for(int i = 0; i < hands.size(); i++){
        int idx = userID == 0 ? i : i + 2;
        prevCollisionMap[idx] = collisionMap[idx];
    }
}

void ModeSwitch::draw(){
    ofDrawRectangle(boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height);
};
