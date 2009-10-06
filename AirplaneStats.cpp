/*
 *  AirplaneStats.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 06/10/09.
 *  Copyright 2009 OpenEngine. All rights reserved.
 *
 */

#include "AirplaneStats.h"

#include <Logging/Logger.h>

AirplaneStats::AirplaneStats(Airplane* a) : airplane(a) {
}



void AirplaneStats::Handle(InitializeEventArg arg) {
    timer.Start();
}
void AirplaneStats::Handle(ProcessEventArg arg) {
    int interval = timer.GetElapsedIntervalsAndReset(1000000);
    if (interval) {
        logger.info << 
        "Airplane Statistics:\n" <<
        "Trottle: " << airplane->GetTrottle() <<
        " Speed: " << airplane->GetSpeed() <<
        logger.end;
    }
}