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
#include <vector>

using namespace std;

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D::ORIGIN(),VART::Point4D::Y());

    vector<VART::Point4D> points;

    // Vértices piramide inferior
    points.push_back(VART::Point4D(0,0,0));
    points.push_back(VART::Point4D(1,0,0));
    points.push_back(VART::Point4D(1,0,1));
    points.push_back(VART::Point4D(0,0,1));
    points.push_back(VART::Point4D(0.5,1,0.5));

    // Vértices piramide superior
    points.push_back(VART::Point4D(0,2,0));
    points.push_back(VART::Point4D(1,2,0));
    points.push_back(VART::Point4D(1,2,1));
    points.push_back(VART::Point4D(0,2,1));

    VART::MeshObject piramides;

    piramides.SetVertices(points);

    // Faces piramide inferior
    piramides.AddFace("0 3 4");
    piramides.AddFace("3 2 4");
    piramides.AddFace("2 1 4");
    piramides.AddFace("1 0 4");
    piramides.AddFace("0 1 2 3");

    // Faces piramide superior
    piramides.AddFace("8 5 4");
    piramides.AddFace("7 8 4");
    piramides.AddFace("6 7 4");
    piramides.AddFace("5 6 4");
    piramides.AddFace("5 8 7 6");

    // Initialize scene objects
    piramides.SetMaterial(VART::Material::PLASTIC_GREEN());

    // Build the scene graph
    scene.AddObject(&piramides);

    // Add lights and cameras to the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    // Set up the viewer
    viewer.SetTitle("V-ART pirades");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    return 0;
}
