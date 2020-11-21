//
//  MyNewFrameListener.cpp
//  Kinect2Skeleton
//
//  Created by Max on 19/11/2020.
//

#include "MyNewFrameListener.hpp"

MyNewFrameListener::MyNewFrameListener(){
    currentImage.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
};

void MyNewFrameListener::onNewFrame(nite::UserTracker& tracker){
    nite::Status status = nite::STATUS_OK;
    nite::UserTrackerFrameRef newFrame;
    status = tracker.readFrame(&newFrame);
    if(!newFrame.isValid())
        return;
    
    const nite::UserMap& userMap = newFrame.getUserMap();
    
    if(userMap.getWidth() != int(currentImage.getWidth()) || userMap.getHeight() != int(currentImage.getHeight()))
        return;
    
    if(!newFrame.getUsers().isEmpty()){
        bool isAnyVisible = false;
        int visibleUser = -1;
        for (int i = 0; i < newFrame.getUsers().getSize(); i++)
        {
            if(newFrame.getUsers()[i].isVisible()){
                isAnyVisible = true;
                visibleUser = i;
                break;
            }
        }
        if(isAnyVisible){
            // Get raw pixel data
            // The getPixels() method returns an array the size of the depth map
            // i.e. 640x480. In this 2D array every value represents the user ID of the person in the pixel :)
            // The value 0 means that no user is assigned to the pixel => it is a background pixel
            const short int *userPixels = userMap.getPixels();

            for(int x = 0; x < userMap.getWidth(); x++){
                for(int y = 0; y < userMap.getHeight(); y++){
                    int index = (x + y * userMap.getWidth()) * 4;
                    if (userPixels) {
                        if (*userPixels != 0) {
                            // Mask with white
                            currentImage.setColor(x % userMap.getWidth(), y % userMap.getHeight(), ofColor::red);
                            
                        } else {
                            currentImage.setColor(x % userMap.getWidth(), y % userMap.getHeight(), ofColor::black);
                        }
                    }
                    userPixels++;
                }
            }
        }
        
        // Music elements
        for (int i = 0; i < newFrame.getUsers().getSize(); i++)
        {
            auto userData = newFrame.getUsers()[i];
            // Get joints
            ofVec2f leftHand = getJointInDepthCoordinates(userData, nite::JointType::JOINT_LEFT_HAND, tracker);
            ofVec2f rightHand = getJointInDepthCoordinates(userData, nite::JointType::JOINT_RIGHT_HAND, tracker);

            if(i == 0){
                if(handsUser1.size() == 0){
                    handsUser1.push_back(leftHand);
                    handsUser1.push_back(rightHand);
                } else {
                    handsUser1[0] = leftHand;
                    handsUser1[1] = rightHand;
                }
            } else {
                if(handsUser2.size() == 0){
                    handsUser2.push_back(leftHand);
                    handsUser2.push_back(rightHand);
                } else {
                    handsUser2[0] = leftHand;
                    handsUser2[1] = rightHand;
                }
            }
        }
    }
};

ofImage& MyNewFrameListener::getCurrentImage(){
    return currentImage;
}

vector<ofVec2f>& MyNewFrameListener::getHandsUser1(){
    return handsUser1;
};

vector<ofVec2f>& MyNewFrameListener::getHandsUser2(){
    return handsUser2;
};

ofVec2f MyNewFrameListener::getJointInDepthCoordinates(nite::UserData& user, nite::JointType jointType, nite::UserTracker& tracker)
{
  const nite::SkeletonJoint& joint = user.getSkeleton().getJoint(jointType);
  float x, y;

  tracker.convertJointCoordinatesToDepth(joint.getPosition().x, joint.getPosition().y, joint.getPosition().z, &x, &y);

  return ofVec2f(x, y);
};
