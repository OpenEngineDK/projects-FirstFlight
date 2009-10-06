/*
 *  Airplane.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 06/10/09.
 *  Copyright 2009 OpenEngine. All rights reserved.
 *
 */

#include "Airplane.h"
#include <Core/EngineEvents.h>
#include <Logging/Logger.h>
#include <Geometry/FaceBuilder.h>


using namespace OpenEngine::Core;

Airplane::Airplane(TransformationNode *node) : 
node(node),
trottle(0.0f) {

    Point center,left_wing, right_wing,tail;
    center.pos = Vector<3,float>(0,0,0);
    center.mass = 1.0;
    
    left_wing.pos = Vector<3,float>(4,0,0);
    left_wing.mass = 0.5;
    
    right_wing.pos = Vector<3,float>(-4,0,0);
    right_wing.mass = 0.5;
    
    tail.pos = Vector<3,float>(0,0,-4);
    tail.mass = 0.5;
    
    points.push_back(center);
    points.push_back(left_wing);
    points.push_back(right_wing);
    points.push_back(tail);
    
    
}

void Airplane::Handle(InitializeEventArg arg) {
    timer.Reset();
    timer.Start();
}

void Airplane::Handle(ProcessEventArg arg) {    
    Time t = timer.GetElapsedTimeAndReset();
    float dt = t.AsInt()/1000.0;
    
    Vector<3,float> old_pos;
    Quaternion<float> old_rot;    
    node->GetAccumulatedTransformations(&old_pos, &old_rot);
    
    node->Move(0,0,0.01*dt*trottle);
            
    // calc lift
    
    Vector<3,float> angles = old_rot.GetEulerAngles();
    
    float pitch = -angles[0];
    
    float cl = pitch*0.08+0.5;
    float lift = speed.GetLengthSquared()*cl*.5*2000.0;
    
    node->Move(0,(lift-9.82)*dt,0);
    //logger.info << lift << logger.end;
    
    logger.info << "speed: " << speed.GetLengthSquared()  <<
    " cl: " << cl << 
    " pitch: " << pitch << 
    " lift: " << lift << logger.end;
    
    // stuff
    
    Vector<3,float> pos;
    Quaternion<float> rot;    
    node->GetAccumulatedTransformations(&pos, &rot);
    
    // fix down.
    if (pos[1] < 0) {
        node->SetPosition(Vector<3,float>(pos[0],0.0,pos[2]));
        node->GetAccumulatedTransformations(&pos, &rot);
    }
    
    Vector<3,float> dx = pos-old_pos;
    
    logger.info << "pos: " << pos << " old_pos: " << old_pos << logger.end;
    
    speed = dx/dt;
}

void Airplane::Pitch(float dp) {
    node->Rotate(dp,0,0);
}

void Airplane::SetTrottle(float t) { 
    trottle = fmax(0.0f,fmin(t,10.0f)); 
}

float Airplane::GetTrottle() {
    return trottle;
}

Vector<3,float> Airplane::GetSpeed() {
    return speed;
}

void Airplane::GenerateSimpleGeometry(FaceSet* fs) {
   
    FaceBuilder::FaceState state;
    state.color = Vector<4,float>(0,0,1,1);
    
    for (vector<Point>::iterator itr = points.begin();
         itr != points.end();
         itr++) {
        Point p = *itr;
        
        FaceBuilder::MakeABox(fs,
                              state,
                              p.pos,
                              Vector<3,float>(1,1,1)*p.mass);
    }        
}