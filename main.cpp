// main
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

// OpenEngine stuff
#include <Meta/Config.h>
#include <Logging/Logger.h>
#include <Logging/StreamLogger.h>
#include <Core/Engine.h>
#include <Scene/SceneNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/PointLightNode.h>
#include <Scene/RenderStateNode.h>

#include <Geometry/FaceSet.h>
#include <Geometry/Face.h>
#include <Display/Camera.h>
#include <Display/FollowCamera.h>
#include <Display/PerspectiveViewingVolume.h>

#include <Physics/FixedTimeStepPhysics.h>
#include <Scene/CollectedGeometryTransformer.h>
#include <Scene/QuadTransformer.h>
#include <Scene/BSPTransformer.h>



// SimpleSetup
#include <Utils/SimpleSetup.h>

#include "FlightHandler.h"
#include "Airplane.h"
#include "AirplaneStats.h"
#include "GridNode.h"

// Game factory
//#include "GameFactory.h"

// name spaces that we will be using.
// this combined with the above imports is almost the same as
// fx. import OpenEngine.Logging.*; in Java.
using namespace OpenEngine::Logging;
using namespace OpenEngine::Core;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Display;
using namespace OpenEngine::Physics;

/**
 * Main method for the first quarter project of CGD.
 * Corresponds to the
 *   public static void main(String args[])
 * method in Java.
 */
int main(int argc, char** argv) {

    // Create simple setup
    SimpleSetup* setup = new SimpleSetup("Example Project Title");
    
    // Print usage info.
    logger.info << "========= Running OpenEngine Test Project =========" << logger.end;
    
    RenderStateNode* root = new RenderStateNode();
    
    root->EnableOption(RenderStateNode::LIGHTING);
    root->EnableOption(RenderStateNode::COLOR_MATERIAL);
    
    setup->GetScene()->AddNode(root);
    
    

    Vector<4,float> colr = Vector<4,float>(1,1,1,1);
    
    FaceSet* groundFs = new FaceSet();
    FacePtr f = FacePtr(new Face(Vector<3,float>(-100,0, 100),
                                 Vector<3,float>( 100,0, 100),
                                 Vector<3,float>( 100,0,-100)));
    f->colr[0] = colr;
    f->colr[1] = colr;
    f->colr[2] = colr;

    groundFs->Add(f);
    f = FacePtr(new Face(Vector<3,float>(-100,0, 100),
                         Vector<3,float>( 100,0,-100),
                         Vector<3,float>(-100,0,-100)));
    
    f->colr[0] = colr;
    f->colr[1] = colr;
    f->colr[2] = colr;

    groundFs->Add(f);
    
    GeometryNode* groundNode = new GeometryNode(groundFs);
    
    SceneNode *physicsNode = new SceneNode();
    physicsNode->AddNode(groundNode);

    CollectedGeometryTransformer collT;
    QuadTransformer quadT;
    BSPTransformer bspT;
    collT.Transform(*physicsNode);
    quadT.Transform(*physicsNode);
    bspT.Transform(*physicsNode);
        

    root->AddNode(physicsNode);
    
    TransformationNode* planeNode = new TransformationNode();

    

    
   
    Airplane *plane = new Airplane(planeNode);
    
    FaceSet* planeFs = new FaceSet();
    plane->GenerateSimpleGeometry(planeFs);
    
    GeometryNode* planeGeo = new GeometryNode(planeFs);
    planeNode->AddNode(planeGeo);
    

    FixedTimeStepPhysics *physics = new FixedTimeStepPhysics(physicsNode);

    RigidBox *box = plane->GetRigidBody();

    physics->AddRigidBody(box);
    root->AddNode(box->GetRigidBoxNode());

    FlightHandler* handler = new FlightHandler(plane);
        

    // Add to engine for processing time (with its timer)
    FixedTimeStepPhysicsTimer* ptimer = new FixedTimeStepPhysicsTimer(*physics);
    setup->GetEngine().InitializeEvent().Attach(*physics);
    setup->GetEngine().ProcessEvent().Attach(*ptimer);
    setup->GetEngine().DeinitializeEvent().Attach(*physics);


    setup->GetKeyboard().KeyEvent().Attach(*handler);
    setup->GetEngine().ProcessEvent().Attach(*plane);
    setup->GetEngine().InitializeEvent().Attach(*plane);

    AirplaneStats* stats = new AirplaneStats(plane);
    setup->GetEngine().ProcessEvent().Attach(*stats);
    setup->GetEngine().InitializeEvent().Attach(*stats);

    
    root->AddNode(planeNode);
    
    
    setup->GetScene()->AddNode(new GridNode());
    
    
    FollowCamera* cam = new FollowCamera(*(new PerspectiveViewingVolume()));
    cam->Follow(planeNode);
    
    cam->SetPosition(Vector<3,float>(0,4,-15));
    cam->LookAt(Vector<3,float>(0,0,0));
    
    
    setup->SetCamera(*cam);

    TransformationNode* lightTrans = new TransformationNode();
    lightTrans->AddNode(new PointLightNode());
    lightTrans->Move(0, 10, 0);
    root->AddNode(lightTrans);
        
    
    // Start the engine.
    setup->GetEngine().Start();


    // Return when the engine stops.
    return EXIT_SUCCESS;
}


