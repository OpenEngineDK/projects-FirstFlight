/*
 *  AirplaneStats.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 06/10/09.
 *  Copyright 2009 OpenEngine. All rights reserved.
 *
 */

#ifndef _FIRST_FLIGHT_AIRPLANE_STATS_H_
#define _FIRST_FLIGHT_AIRPLANE_STATS_H_

#include <Core/EngineEvents.h>
#include <Core/IListener.h>
#include <Utils/Timer.h>

#include "Airplane.h"

using namespace OpenEngine::Core;
using namespace OpenEngine::Utils;

class AirplaneStats : public IListener<ProcessEventArg>, public IListener<InitializeEventArg> {
    
    Timer timer;
    Airplane* airplane;
    
public:
    AirplaneStats(Airplane* a);
    
    void Handle(InitializeEventArg arg);
    void Handle(ProcessEventArg arg);
};

#endif