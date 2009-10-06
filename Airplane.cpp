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

}

void Airplane::Handle(InitializeEventArg arg) {
    timer.Reset();
    timer.Start();
}

void Airplane::Handle(ProcessEventArg arg) {
    
    Time t = timer.GetElapsedTimeAndReset();
    
    float dt = t.AsInt()/1000.0;
        
    node->Move(0,0,0.01*dt*trottle);            
}

void Airplane::SetTrottle(float t) { 
    trottle = fmax(0.0f,fmin(t,1.0f)); 
}

float Airplane::GetTrottle() {
    return trottle;
}


void Airplane::GenerateSimpleGeometry(FaceSet* fs) {
   
    FaceBuilder::FaceState state;
    state.color = Vector<4,float>(0,0,1,1);
    
    // fuselage    
    FaceBuilder::MakeABox(fs,
                          state,
                          Vector<3,float>(0,0,0),
                          Vector<3,float>(2,2,2));
    
    // wings
    float wingsOffset = 4;
    FaceBuilder::MakeABox(fs,
                          state,
                          Vector<3,float>(wingsOffset,0,0),
                          Vector<3,float>(1,1,1));
    FaceBuilder::MakeABox(fs,
                          state,
                          Vector<3,float>(-wingsOffset,0,0),
                          Vector<3,float>(1,1,1));
    

    // tail    
    FaceBuilder::MakeABox(fs,
                          state,
                          Vector<3,float>(0,0,-4),
                          Vector<3,float>(1,1,1));
    
    
    
}