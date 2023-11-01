#include <vart/scene.h>
#include <vart/light.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/mesh.h>
#include <vart/meshobject.h>
#include <vart/box.h>
#include <vart/cylinder.h>
#include <vart/cone.h>

#include <iostream>
#include <cmath>

using namespace std;
using namespace VART;

int main(int argc, char* argv[]){
    ViewerGlutOGL::Init(&argc, argv);
    ViewerGlutOGL viewer;
    Scene scene;
    Camera camera(Point4D(-5, 12, 15), Point4D::ORIGIN(), Point4D::Y());

    Box guarda;
    guarda.MakeBox(-3, 3, -0.5, 0.5, -1, 1);
    guarda.SetMaterial(Material::DARK_PLASTIC_GRAY());

    Transform rotacaoCabo;
    rotacaoCabo.MakeXRotation(M_PI/2);
    guarda.AddChild(rotacaoCabo);

    Cylinder cabo(5, 0.75);
    cabo.SetMaterial(Material::LIGHT_PLASTIC_GRAY());
    rotacaoCabo.AddChild(cabo);

    MeshObject lamina;
    vector<Point4D> pontos;
    pontos.push_back(Point4D(-2, 0, 0));
    pontos.push_back(Point4D(0, 0, 0.75));
    pontos.push_back(Point4D(2, 0, 0));
    pontos.push_back(Point4D(0, 0, -0.75));

    pontos.push_back(Point4D(-2, 20, 0));
    pontos.push_back(Point4D(0, 20, 0.75));
    pontos.push_back(Point4D(2, 20, 0));
    pontos.push_back(Point4D(0, 20, -0.75));
    pontos.push_back(Point4D(0, 25, 0));

    lamina.SetVertices(pontos);
    lamina.AddFace("0 1 5 4");
    lamina.AddFace("1 2 6 5");
    lamina.AddFace("2 3 7 6");
    lamina.AddFace("3 0 4 7");

    // Mesh face;
    // face.type = Mesh::TRIANGLES;
    lamina.AddFace("4 8 5");
    lamina.AddFace("5 8 6");
    lamina.AddFace("6 8 7");
    lamina.AddFace("7 8 4");

    lamina.ComputeVertexNormals();
    lamina.SetMaterial(Material::PLASTIC_WHITE());

    Transform translacaoCone;
    translacaoCone.MakeTranslation(0, 0, 5);

    Cone coneCabo(1, 1);
    coneCabo.SetMaterial(Material::DARK_PLASTIC_GRAY());
    translacaoCone.AddChild(coneCabo);

    cabo.AddChild(translacaoCone);

    guarda.AddChild(lamina);

    scene.AddObject(&guarda);

    scene.AddLight(Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    viewer.SetScene(scene);
    viewer.MainLoop();
    return 0;
}