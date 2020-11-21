//
//  MyNewFrameListener.hpp
//  Kinect2Skeleton
//
//  Created by Max on 19/11/2020.
//

#ifndef MyNewFrameListener_hpp
#define MyNewFrameListener_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxNI2.h"
#include "ofxNiTE2.h"
#include "NiTE.h"

class MyNewFrameListener : public nite::UserTracker::NewFrameListener{
public:
    MyNewFrameListener();
    void onNewFrame(nite::UserTracker& userTracker);
    ofImage& getCurrentImage();
    
    vector<ofVec2f>& getHandsUser1();
    vector<ofVec2f>& getHandsUser2();
    
private:
    ofVec2f getJointInDepthCoordinates(nite::UserData& user, nite::JointType jointType, nite::UserTracker& tracker);
    ofImage currentImage;
    
    vector<ofVec2f> handsUser1;
    vector<ofVec2f> handsUser2;
};

#endif /* MyNewFrameListener_hpp */
