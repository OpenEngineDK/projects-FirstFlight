/*
 *  FlightHandler.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 06/10/09.
 *  Copyright 2009 OpenEngine. All rights reserved.
 *
 */

#include "FlightHandler.h"


FlightHandler::FlightHandler(Airplane *plane) : plane(plane) {

}

void FlightHandler::Handle(KeyboardEventArg arg) {
    if (arg.type == EVENT_PRESS) {
     
        if (arg.sym == KEY_w) {
            plane->SetTrottle(plane->GetTrottle()+0.1);
        } else if (arg.sym == KEY_s) {
            plane->SetTrottle(plane->GetTrottle()-0.1);
        }
        
        
    }
}