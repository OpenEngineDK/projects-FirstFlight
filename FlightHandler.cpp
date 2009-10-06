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
     
        const float pitchAngle = 0.05;
        
        switch (arg.sym) {
            case KEY_w:
                plane->SetTrottle(plane->GetTrottle()+0.1);
                break;
            case KEY_s:
                plane->SetTrottle(plane->GetTrottle()-0.1);
                break;
            case KEY_e:
                plane->Pitch(pitchAngle);
                break;
            case KEY_d:
                plane->Pitch(-pitchAngle);
                break;
            default:
                break;
        }
                
    }
}