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
using namespace OpenEngine::Physics;

Airplane::Airplane(TransformationNode *node) : 
node(node),
trottle(0.0f) {

    //Point center,left_wing, right_wing,left_tail,right_tail;

    center = new Point();
    left_wing = new Point();
    right_wing = new Point();
    left_tail = new Point();
    right_tail = new Point();

    center->pos = Vector<3,float>(0,0,0);
    center->mass = 1.0;
    center->forceIdx = 0;
    
    left_wing->pos = Vector<3,float>(4,0,0);
    left_wing->mass = 0.5;
    left_wing->forceIdx = 1;
    
    right_wing->pos = Vector<3,float>(-4,0,0);
    right_wing->mass = 0.5;
    right_wing->forceIdx = 5;
    
    left_tail->pos = Vector<3,float>(2,0,-4);
    left_tail->mass = 0.5;
    left_tail->forceIdx = 2;

    right_tail->pos = Vector<3,float>(-2,0,-4);
    right_tail->mass = 0.5;
    right_tail->forceIdx = 6;
    
    points.push_back(center);
    points.push_back(left_wing);
    points.push_back(right_wing);
    points.push_back(left_tail);
    points.push_back(right_tail);
    
    FaceSet *fs = new FaceSet();

    GenerateSimpleGeometry(fs);

    box = new RigidBox(Box(*fs));

    delete fs;

    box->SetTransformationNode(node);
    box->SetGravity(Vector<3,float>(0,-9.82,0));
    box->SetCenter(Vector<3,float>(0,5,0));
}

void Airplane::Handle(InitializeEventArg arg) {
    timer.Reset();
    timer.Start();
}

RigidBox* Airplane::GetRigidBody() {
    return box;
}

void Airplane::Handle(ProcessEventArg arg) {    
    Time t = timer.GetElapsedTimeAndReset();
    float dt = t.AsInt()/1000.0;

    Matrix<3,3,float> rotM = box->GetRotationMatrix();

    box->AddForce(rotM *  Vector<3,float>(0,0,1*dt*trottle));

    for (vector<Point*>::iterator itr = points.begin();
         itr != points.end();
         itr++) {
        Point* p = *itr;
        Vector<3,float> speedVec = box->GetSpeed();

        float speed = speedVec.GetLength();
        Vector<3,float> dragVec;


        if (speed > 0.1) 
            dragVec = -speedVec/speed;


        Vector<3,float> normal = Vector<3,float>(0,1,0);
        Vector<3,float> liftVector = (dragVec % normal) % dragVec;

        if (liftVector.IsZero())
            return;

        float liftLen = liftVector.GetLength();
        liftVector.Normalize();

        float dotP = dragVec * normal;
        dotP = fmin(fmax(dotP, -1),1);

        float attackAngel = asin(dotP);
        float rho = 1.2f;    
        float area = 5*5;
        float force = 0.5f * rho * speed * speed * area;
    
        Vector<3,float> forceVec = 
            (liftVector * LiftCoefficient(attackAngel,0) +
             dragVec * DragCoefficient(attackAngel,0)) * force;

        //logger.info << forceVec << logger.end;


        box->AddForce(rotM * forceVec, p->forceIdx );

    }
    


    //box->AddForce(Vector<3,float>(0,lift,0));


    return;

    // Matrix<3,3,float> rotM = box->GetRotationMatrix();

    // //logger.info << speed << logger.end;
    // //speed = rotM*speed;

    // Quaternion<float> q = Quaternion<float>(rotM);

    // Vector<3,float> angles = q.GetEulerAngles();

    // float pitch = -angles[0];

    // //logger.info << "pitch " << pitch << logger.end;

    // float cl = 1;
    
    // float lift = speedVec[2]*cl;

    

    //logger.info << "cl " << cl << logger.end;
    //logger.info << "lift " << lift << logger.end;

    
    // Vector<3,float> old_pos;
    // Quaternion<float> old_rot;    
    // node->GetAccumulatedTransformations(&old_pos, &old_rot);
    
    // node->Move(0,0,0.01*dt*trottle);
            
    // // calc lift
    
    // Vector<3,float> angles = old_rot.GetEulerAngles();
    
    // float pitch = -angles[0];
    
    // float cl = pitch*0.08+0.5;
    // float lift = speed.GetLengthSquared()*cl*.5*2000.0;
    
    // node->Move(0,(lift-9.82)*dt,0);
    // //logger.info << lift << logger.end;
    
    // logger.info << "speed: " << speed.GetLengthSquared()  <<
    // " cl: " << cl << 
    // " pitch: " << pitch << 
    // " lift: " << lift << logger.end;
    
    // // stuff
    
    // Vector<3,float> pos;
    // Quaternion<float> rot;    
    // node->GetAccumulatedTransformations(&pos, &rot);
    
    // // fix down.
    // if (pos[1] < 0) {
    //     node->SetPosition(Vector<3,float>(pos[0],0.0,pos[2]));
    //     node->GetAccumulatedTransformations(&pos, &rot);
    // }
    
    // Vector<3,float> dx = pos-old_pos;
    
    // logger.info << "pos: " << pos << " old_pos: " << old_pos << logger.end;
    
    // speed = dx/dt;
}

float Airplane::LiftCoefficient(float angle, int flaps) {
    float clf0[9] = { -.54,  -.2 ,  .2 ,  .57,   .92,  1.21,  1.43,  1.4 ,  1.0  };
    float clfd[9] = { 0   ,   .45,  .85, 1.02,  1.39,  1.65,  1.75,  1.38,  1.17 };
    float clfu[9] = { -.74,  -.4 , 0   ,  .27,   .63,   .92,  1.03,  1.1 ,   .78 };
    float    a[9] = {-8   , -4   , 0   , 4   ,  8   , 12   , 16   , 20   , 24    };

    float cl = 0;
    for (int i=0;i<8;i++) {
        if ((a[i] <= angle) && (a[i+1] > angle)) {
            switch (flaps) {
            case 0:
                cl = clf0[i] - (a[i] - angle) * (clf0[i] - clf0[i+1]) / (a[i] - a[i+1]);
                break;
            case -1:
                cl = clfd[i] - (a[i] - angle) * (clfd[i] - clfd[i+1]) / (a[i] - a[i+1]);
                break;

            case 1:
                cl = clfu[i] - (a[i] - angle) * (clfu[i] - clfu[i+1]) / (a[i] - a[i+1]);
                break;

            }
        }
            
    }
    return cl;
}

float Airplane::DragCoefficient(float angle, int flaps) {
     float cdf0[9] = { .01  , .0074, .004 , .009  , .013  , .02  , .05, .12 , .21 };
     float cdfd[9] = { .0065, .0043, .0055, .0153 , .0221 , .0391, .1 , .195, .3  };
     float cdfu[9] = { .005 , .0043, .0055, .02601, .03757, .6647, .13, .18 , .25 };
     float    a[9] = {-8   , -4   , 0   , 4   ,  8   , 12   , 16   , 20   , 24    };

    float cd = 0;
    for (int i=0;i<8;i++) {
        if ((a[i] <= angle) && (a[i+1] > angle)) {
            switch (flaps) {
            case 0:
                cd = cdf0[i] - (a[i] - angle) * (cdf0[i] - cdf0[i+1]) / (a[i] - a[i+1]);
                break;
            case -1:
                cd = cdfd[i] - (a[i] - angle) * (cdfd[i] - cdfd[i+1]) / (a[i] - a[i+1]);
                break;

            case 1:
                cd = cdfu[i] - (a[i] - angle) * (cdfu[i] - cdfu[i+1]) / (a[i] - a[i+1]);
                break;

            }
        }
            
    }
    return cd;
}

void Airplane::Pitch(float dp) {
    node->Rotate(dp,0,0);
    int flap = 0;
    if (dp > 0)
        flap = +1;
    else 
        flap = -1;

    

    left_tail->flap  += flap;

    if (left_tail->flap > 1)
        left_tail->flap = 1;
    else if (left_tail->flap < -1)
        left_tail->flap = -1;
    
    right_tail->flap += flap;

    if (right_tail->flap > 1)
        right_tail->flap = 1;
    else if (right_tail->flap < -1)
        right_tail->flap = -1;

    logger.info << right_tail->flap << logger.end;
        
}

void Airplane::SetTrottle(float t) { 
    trottle = fmax(0.0f,fmin(t,10.0f)); 
}

float Airplane::GetTrottle() {
    return trottle;
}

Vector<3,float> Airplane::GetSpeed() {
    return box->GetSpeed();
}

void Airplane::GenerateSimpleGeometry(FaceSet* fs) {
   
    FaceBuilder::FaceState state;
    state.color = Vector<4,float>(0,0,1,1);
    
    for (vector<Point*>::iterator itr = points.begin();
         itr != points.end();
         itr++) {
        Point* p = *itr;
        
        FaceBuilder::MakeABox(fs,
                              state,
                              p->pos,
                              Vector<3,float>(1,1,1)*p->mass);
    }        
}
