//
//  ParameterSlider.hpp
//  Kinect2Skeleton
//
//  Created by Max on 09/11/2020.
//

#ifndef ParameterSlider_hpp
#define ParameterSlider_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxMidi.h"

class ParameterSlider {
    public:
        ParameterSlider(ofxMidiOut& midiOut, int posX, int posY, int width, int height, int channel, int cc);
        
        void update(vector<ofVec2f>& hands, int userID);
        
        void draw();
        
    private:
        ofxMidiOut& midiOut;
        ofRectangle boundingRect;
        ofRectangle handRect;

        int channel, midiCC, velocity;
        
        // Which hand collided
        vector<bool> collisionMap;
        vector<bool> prevCollisionMap;
};

#endif /* ParameterSlider_hpp */
