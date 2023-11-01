#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <vart/mesh.h>

#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    // ./create a camera (scene observer)
    VART::Camera camera(VART::Point4D(-5,1,-5),VART::Point4D::ORIGIN(),VART::Point4D::Y());
    // Create some objects
    VART::Arrow arrowX(2);
    VART::Arrow arrowY(VART::Point4D::ORIGIN(), VART::Point4D::Y()*2);
    VART::Arrow arrowZ(VART::Point4D::ORIGIN(), VART::Point4D::Z()*2);

    //criando uma piramide
    //definir 5 pontos
    vector<VART::Point4D> pontos;
    pontos.reserve(5);
    pontos.push_back(VART::Point4D(0,0,0,1));
    pontos.push_back(VART::Point4D(2,0,0,1));
    pontos.push_back(VART::Point4D(2,0,2,1));
    pontos.push_back(VART::Point4D(0,0,2,1));
    pontos.push_back(VART::Point4D(1,2,1,1));
    

    VART::MeshObject piramide;
    piramide.SetVertices(pontos);
    piramide.AddFace("0 1 2 3");
    piramide.AddFace("0 3 4");
    piramide.AddFace("1 0 4");
    piramide.AddFace("2 1 4");
    piramide.AddFace("3 2 4");
    piramide.AddFace("0 3 4");

    piramide.ComputeVertexNormals();
    piramide.SetMaterial(VART::Material::PLASTIC_RED());


    // Initialize scene objects
    arrowX.SetMaterial(VART::Material::PLASTIC_RED());
    arrowY.SetMaterial(VART::Material::PLASTIC_GREEN());
    arrowZ.SetMaterial(VART::Material::PLASTIC_BLUE());

    // Build the scene graph
    scene.AddObject(&arrowX);
    scene.AddObject(&arrowY);
    scene.AddObject(&arrowZ);
    scene.AddObject(&piramide);

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