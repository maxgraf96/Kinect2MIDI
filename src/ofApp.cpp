#include "ofApp.h"

//ofEasyCam cam;

ofApp::ofApp(
             ofxNI2::Device& device,
             ofxNiTE2::UserTracker& tracker,
             const nite::UserTracker& userTracker
             ) : device(device), tracker(tracker), userTracker(userTracker)
{
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0, 0);
    ofSetWindowTitle("Kinect2Skeleton");
        
    // Setup OSC
    oscSender.setup("localhost", OSC_PORT);
    
    // Setup MIDI
    midiOut.listOutPorts();
    
    // Connect to MIDI port 0
    midiOut.openPort(0);
    
    // Y position of drum triggers in screen coordinates
    int drumTriggerY = 150;
    int parameterSliderY = 0;
    
    int pitchmod = 24;
    
    // Create drum triggers and add to container
    auto kickTrigger = make_unique<DrumTrigger>(midiOut, 0, drumTriggerY, 50, 50, 1, 36 + pitchmod);
    auto snareTrigger = make_unique<DrumTrigger>(midiOut, 60, drumTriggerY, 50, 50, 1, 37 + pitchmod);
    auto trigger3 = make_unique<DrumTrigger>(midiOut, 120, drumTriggerY, 50, 50, 1, 38 + pitchmod);
    auto trigger4 = make_unique<DrumTrigger>(midiOut, 180, drumTriggerY, 50, 50, 1, 39 + pitchmod);
    auto trigger5 = make_unique<DrumTrigger>(midiOut, 240, drumTriggerY, 50, 50, 1, 40 + pitchmod);
    auto trigger6 = make_unique<DrumTrigger>(midiOut, 300, drumTriggerY, 50, 50, 1, 41 + pitchmod);
    auto trigger7 = make_unique<DrumTrigger>(midiOut, 360, drumTriggerY, 50, 50, 1, 42 + pitchmod);
    auto trigger8 = make_unique<DrumTrigger>(midiOut, 420, drumTriggerY, 50, 50, 1, 43 + pitchmod);
    auto trigger9 = make_unique<DrumTrigger>(midiOut, 480, drumTriggerY, 50, 50, 1, 44 + pitchmod);
    auto trigger10 = make_unique<DrumTrigger>(midiOut, 540, drumTriggerY, 50, 50, 1, 45 + pitchmod);

    drumTriggers.push_back(move(kickTrigger));
    drumTriggers.push_back(move(snareTrigger));
    drumTriggers.push_back(move(trigger3));
    drumTriggers.push_back(move(trigger4));
    drumTriggers.push_back(move(trigger5));
    drumTriggers.push_back(move(trigger6));
    drumTriggers.push_back(move(trigger7));
    drumTriggers.push_back(move(trigger8));
    drumTriggers.push_back(move(trigger9));
    drumTriggers.push_back(move(trigger10));
    
    // Create parameter sliders and add to container
    auto paramSlider1 = make_unique<ParameterSlider>(midiOut, 150, parameterSliderY, 250, 50, 1, 55);
    
    parameterSliders.push_back(move(paramSlider1));
    
    modeSwitch = make_unique<ModeSwitch>(isPlayMode, 0, 0, 50, 50);
    
    // Init user mask for segmentation
    userMask.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
    colourPixels.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
}

void ofApp::updateHands(int userID){
    modeSwitch->update(hands, userID);
    
    if(isPlayMode){
        for(auto& drumTrigger : drumTriggers){
            drumTrigger->update(hands, userID);
        }
    } else {
        for(auto& parameterSlider : parameterSliders){
            parameterSlider->update(hands, userID);
        }
    }
}

void ofApp::userLogic(){
    mtx.lock();
    // Segmentation currently only for 1 user
    if(tracker.get().isValid()
       && tracker.getNumUser() > 0
       && tracker.getNumUser() == tracker.getFrame().getUsers().getSize()
       && tracker.getFrame().isValid()){
        
        
        const nite::UserMap& userMap = tracker.getFrame().getUserMap();
        if(userMap.getWidth() != int(userMask.getWidth()) || userMap.getHeight() != int(userMask.getHeight())){
            mtx.unlock();
            return;
        }
        
        // Initialise mask array
        long total = userMap.getWidth() * userMap.getHeight();
        unsigned char maskPixels[total];
        
        // Get raw pixel data
        // The getPixels() method returns an array the size of the depth map
        // i.e. 640x480. In this 2D array every value represents the user ID of the person in the pixel :)
        // The value 0 means that no user is assigned to the pixel => it is a background pixel
        const short int *userPixels = userMap.getPixels();
        
        
        for(int i = 0; i < total; i++){
            if (userPixels) {
                if (*userPixels != 0) {
                    // Mask with white
                    userMask.setColor(i * 4, ofColor(255, 0, 0, 255));

                } else {
                    userMask.setColor(i * 4, ofColor(0, 0, 0, 0));
                }
                userPixels++;
            }
        }
        
        userMask.update();

        nite::UserTracker local = userTracker;
        
        // Music elements
        for (int i = 0; i < tracker.getNumUser(); i++)
        {
            auto user = tracker.getUser(i);
            nite::UserData userData = tracker.getUser(i)->get();

            // Get joints
            ofVec2f leftHand = user->getJointInDepthCoordinates(userData, nite::JointType::JOINT_LEFT_HAND, local);
            ofVec2f rightHand = user->getJointInDepthCoordinates(userData, nite::JointType::JOINT_RIGHT_HAND, local);

            if(hands.size() == i * 2){
                hands.push_back(leftHand);
                hands.push_back(rightHand);
            } else {
                hands[i * 2] = leftHand;
                hands[i * 2 + 1] = rightHand;
            }

            updateHands(i);
        }
    }
    mtx.unlock();
}

//--------------------------------------------------------------
void ofApp::update(){
    device.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofEnableAlphaBlending();
    
    depthPixels = tracker.getPixelsRef(1000, 4000);
    depthTexture.loadData(depthPixels);
    
    userLogic();

    // draw in 2D
    ofSetColor(255, 255, 255, 255);
//    depthTexture.draw(0, 0);
    // tracker.draw();
    
    // Draw user mask
    userMask.draw(0, 0, userMask.getWidth(), userMask.getHeight());

    // Draw mode switch
    modeSwitch->draw();
    
    if(isPlayMode){
        // Draw drum triggers
        for(auto& drumTrigger : drumTriggers){
            drumTrigger->draw();
        }
    } else {
        // Draw parameters
        for(auto& parameterSlider : parameterSliders){
            parameterSlider->draw();
        }
    }
     
    // draw 3D skeleton in 2D
    ofPushView();
    tracker.getOverlayCamera().begin(ofRectangle(0, 0, depthTexture.getWidth(), depthTexture.getHeight()));
    ofDrawAxis(100);
    tracker.draw3D();
    
    tracker.getOverlayCamera().end();
    ofPopView();
        
    // draw in 3D
//    cam.begin();
//    ofDrawAxis(100);
//    tracker.draw3D();
//
//    // draw box
//    ofNoFill();
//    ofSetColor(255, 0, 0);
//    for (int i = 0; i < tracker.getNumUser(); i++)
//    {
//        ofxNiTE2::User::Ref user = tracker.getUser(i);
//        const ofxNiTE2::Joint &joint = user->getJoint(nite::JOINT_HEAD);
//
//        joint.transformGL();
//        ofDrawBox(100);
//        joint.restoreTransformGL();
//    }
//
//    cam.end();

    ofDrawBitmapString("Tracker FPS: "+ofToString(tracker.getFrameRate()),20,ofGetHeight()-40);
//    ofDrawBitmapString("Application FPS: "+ofToString(ofGetFrameRate()),20,ofGetHeight()-20);
    
}

void ofApp::exit(){
    midiOut.closePort();
    tracker.get().destroy();
    tracker.exit();
    device.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
