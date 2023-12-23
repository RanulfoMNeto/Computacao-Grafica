// main.cpp - from V-ART template application

// This example shows how to use the Arrow class.

// Changelog
// Oct 19, 2012 - Bruno de Oliveira Schneider
// - Removed the unused keyboard handler.
// - Added more arrows.
// Sep 25, 2008 - Bruno de Oliveira Schneider
// - Application created.

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>
#include <list>
#include <map>
#include <cmath>

#define RUAX 8
#define RUAZ 16

#define POSTEDISTANCIA 30

#define RUALATERALX 1.02

using namespace std;

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,1.80,10),VART::Point4D::ORIGIN(),VART::Point4D::Y());

    list<VART::MeshObject*> light_pole;
    VART::MeshObject::ReadFromOBJ("street/light-pole.obj", &light_pole);

    list<VART::MeshObject*> street;
    VART::MeshObject::ReadFromOBJ("street/street.obj", &street);

    map<string, VART::MeshObject*> mapMeshObjects;

    VART::Transform translationRight;
    VART::Transform translationLeft;
    VART::Transform translationBack;
    VART::Transform translationBackLeft;

    VART::Transform rotationUp;
    VART::Transform rotationRight;

    // light pole
    for(VART::MeshObject* i : light_pole) {
        rotationUp.MakeXRotation(M_PI/2);

        double sideDistance = (RUAX - RUALATERALX)/2;

        rotationRight.MakeZRotation(M_PI);
        translationRight.MakeTranslation(VART::Point4D(-sideDistance, RUAZ/2, 0));

        // Poste 1 Direito
        rotationRight.AddChild(translationRight);
        translationRight.AddChild(*i);

        // Poste 2 Direito
        translationBack.MakeTranslation(VART::Point4D(0, POSTEDISTANCIA, 0));
        translationRight.AddChild(translationBack);
        translationBack.AddChild(*i);

        rotationUp.AddChild(rotationRight);


        translationLeft.MakeTranslation(VART::Point4D(-sideDistance, -RUAZ/2, 0));

        // Poste 1 Esquerdo
        translationLeft.AddChild(*i);

        // Poste 2 Esquerdo
        translationBackLeft.MakeTranslation(VART::Point4D(0, -POSTEDISTANCIA, 0));
        translationLeft.AddChild(translationBackLeft);
        translationBackLeft.AddChild(*i);

        rotationUp.AddChild(translationLeft);

        scene.AddObject(&rotationUp);
    }

    VART::Transform translationStreet2;
    VART::Transform translationStreet3;

    // street
    for(VART::MeshObject* i : street) {
        cout << i->GetDescription() << endl;
        mapMeshObjects[i->GetDescription()] =  i;

        // street 1 
        scene.AddObject(i);

        // street 2
        translationStreet2.MakeTranslation(VART::Point4D(0, 0, -RUAZ));
        translationStreet2.AddChild(*i);
        scene.AddObject(&translationStreet2);

        // street 3
        translationStreet3.MakeTranslation(VART::Point4D(0, 0, -2*RUAZ));
        translationStreet3.AddChild(*i);
        scene.AddObject(&translationStreet3);
    }

    // Add lights and cameras to the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    // Set up the viewer
    viewer.SetTitle("V-ART arrow example");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    return 0;
}
