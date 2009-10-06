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

#include <Scene/TransformationNode.h>

using namespace OpenEngine::Core;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Geometry;

class Airplane: public IListener<ProcessEventArg>, public IListener<InitializeEventArg> {
    struct Point {
        Vector<3,float> pos;
        float mass;
    };

    Vector<3,float> speed;
    
    TransformationNode* node;
    float trottle;
    Timer timer;
    vector<Point> points;
    
public:
    Airplane(TransformationNode* node);
    void Handle(InitializeEventArg arg);
    void Handle(ProcessEventArg arg);
    
    Vector<3,float> GetSpeed();
    
    void SetTrottle(float t);
    float GetTrottle();
    
    void Pitch(float dp);
    
    void GenerateSimpleGeometry(FaceSet* fs);
};

#endif
