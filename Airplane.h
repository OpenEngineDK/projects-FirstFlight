/*
 *  Airplane.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 06/10/09.
 *  Copyright 2009 OpenEngine. All rights reserved.
 *
 */

#ifndef _FIRST_FLIGHT_AIRPLANE_H_
#define _FIRST_FLIGHT_AIRPLANE_H_

#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Utils/Timer.h>
#include <Geometry/FaceSet.h>
#include <Physics/IOERigidBody.h>
#include <Physics/RigidBox.h>

#include <Scene/TransformationNode.h>

using namespace OpenEngine::Core;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Physics;

class Airplane: public IListener<ProcessEventArg>, public IListener<InitializeEventArg> {
    struct Point {
        Point() : flap(0), forceIdx(0) {}

        Vector<3,float> pos;        
        float mass;
        int flap;
        int forceIdx;
    };

    Point *center,
        *left_wing, *right_wing,*left_tail,*right_tail;

    //Vector<3,float> speed;
    
    TransformationNode* node;
    float trottle;
    Timer timer;
    vector<Point*> points;

    RigidBox* box;
    
    float LiftCoefficient(float angle, int flaps);
    float DragCoefficient(float angle, int flaps);
    

public:
    Airplane(TransformationNode* node);
    void Handle(InitializeEventArg arg);
    void Handle(ProcessEventArg arg);
    
    Vector<3,float> GetSpeed();
    
    void SetTrottle(float t);
    float GetTrottle();
    
    void Pitch(float dp);

    RigidBox* GetRigidBody();
    
    void GenerateSimpleGeometry(FaceSet* fs);
};

#endif
