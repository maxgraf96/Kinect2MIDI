//
//  ModeSwitch.hpp
//  Kinect2Skeleton
//
//  Created by Max on 09/11/2020.
//

#ifndef ModeSwitch_hpp
#define ModeSwitch_hpp

#include <stdio.h>
#include "ofMain.h"

class ModeSwitch {
public:
    ModeSwitch(bool& isPlayMode, int posX, int posY, int width, int height);
    
    void update(vector<ofVec2f>& hands, int userID);
    
    void draw();
    
private:
    bool& isPlayMode;
    ofRectangle boundingRect;
    ofRectangle handRect;
    
    vector<bool> collisionMap;
    vector<bool> prevCollisionMap;
};

#endif /* ModeSwitch_hpp */
