#include <vart/scene.h>
#include <vart/light.h>
#include <vart/arrow.h>
#include <vart/meshobject.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/mesh.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define TAM 20

using namespace std;

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : radians(0) {}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            radians += 0.01;
            rotation->MakeYRotation(radians);
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform* rotation;
    private:
        float radians;
};

// The application itself:
int main(int argc, char* argv[]) {
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)
    // ./create a camera (scene observer)
    VART::Camera camera(VART::Point4D(-8,5,-8),VART::Point4D(0,5,0),VART::Point4D::Y());

    //piramide de base triangular
    const unsigned int n = 8;

    cout << argc << endl;

    vector<VART::Point4D> pPrisma;
    pPrisma.reserve(2+n);
    for(unsigned short i=0; i<n; i++){
        float angulo = i*(2*M_PI/n);
        pPrisma.push_back(VART::Point4D(2*sin(angulo), 5, 2*cos(angulo)));
    }
    pPrisma.push_back(VART::Point4D(0, 0, 0));
    pPrisma.push_back(VART::Point4D(0, 10, 0));

    VART::MeshObject prisma;
    prisma.SetVertices(pPrisma);
    for(unsigned int i=0; i<n; i++){
        VART::Mesh faceCima;
        faceCima.type = VART::Mesh::TRIANGLES;
        faceCima.indexVec = {i, (i+1)%n, n+1};
        VART::Mesh faceBaixo;
        faceBaixo.type = VART::Mesh::TRIANGLES;
        faceBaixo.indexVec = {(i+1)%n, i, n};
        prisma.AddMesh(faceCima);
        prisma.AddMesh(faceBaixo);
    }

    // VART::Mesh base;
    // base.type = VART::Mesh::POLYGON;
    // for(unsigned int i=n; i>0; i--){
    //     base.indexVec.push_back(i-1);
    // }
    // prisma.AddMesh(base);


    prisma.ComputeVertexNormals();
    VART::Material material;
    material.SetPlasticColor(VART::Color::CYAN());
    material.SetShininess(100);
    prisma.SetMaterial(material);

    VART::Transform prismaRotation;
    prismaRotation.AddChild(prisma);

    // Build the scene graph
    scene.AddObject(&prismaRotation);

    // Add lights and cameras to the scene
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    // Set Idle Handler
    MyIHClass idleHandler;
    idleHandler.rotation = &prismaRotation;

    // Set up the viewer
    viewer.SetTitle("V-ART arrow example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler);

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop) and never return
    return 0;
}
