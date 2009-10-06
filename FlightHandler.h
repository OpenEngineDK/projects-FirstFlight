/*
 *  FlightHandler.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 06/10/09.
 *  Copyright 2009 OpenEngine. All rights reserved.
 *
 */

#ifndef _FIRST_FLIGHT_FLIGHTHANDLER_H_
#define _FIRST_FLIGHT_FLIGHTHANDLER_H_

#include <Devices/IKeyboard.h>
#include <Core/IListener.h>

#include "Airplane.h"

using namespace OpenEngine::Core;
using namespace OpenEngine::Devices;

class FlightHandler : public IListener<KeyboardEventArg> {
    
    Airplane* plane;
    
public:
    FlightHandler(Airplane* plane);
    
    virtual void Handle(KeyboardEventArg arg);
    
};

#endif
