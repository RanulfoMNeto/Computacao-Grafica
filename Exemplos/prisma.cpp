#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/mesh.h>

#include <iostream>
#include <cmath>

using namespace std;
using namespace VART;

class Prisma : public MeshObject{
    public:
        Prisma(unsigned int, float, float);
    private:
        unsigned int nLados;
        float r;
        float h;
        void definirPontos();
        void definirFacesLaterais();
        void definirBases();
        Material setMaterial();
        void construirPrisma();
};

Prisma::Prisma(unsigned int nLados, float raio, float altura){
    this->nLados = nLados;
    r = raio;
    h = altura;
    construirPrisma();
}

void Prisma::construirPrisma(){
    definirPontos();
    definirFacesLaterais();
    definirBases();

    this->ComputeVertexNormals();
}

void Prisma::definirPontos(){
    vector<Point4D> pontos;
    for(unsigned int i=0; i<nLados; i++){
        double angulo = i*(2*M_PI/nLados);
        pontos.push_back(Point4D(r*cos(angulo), 0, r*sin(angulo)));
    }
    for(unsigned int i=0; i<nLados; i++){
        double angulo = i*(2*M_PI/nLados);
        pontos.push_back(Point4D(r*cos(angulo), h, r*sin(angulo)));
    }

    this->SetVertices(pontos);
}

void Prisma::definirFacesLaterais(){
    for(unsigned int i=0; i<nLados; i++){
        Mesh faceL;
        faceL.type = Mesh::QUADS;

        faceL.indexVec = {(i+1)%nLados, i, i+nLados, (((i+1)%nLados)+nLados)};

        faceL.material = setMaterial();

        this->AddMesh(faceL);
    }
}

void Prisma::definirBases(){
    Mesh baseBaixo;
    baseBaixo.type = Mesh::POLYGON;
    for(unsigned int i=0; i<nLados; i++)
        baseBaixo.indexVec.push_back(i);
    Mesh baseCima;
    baseCima.type = Mesh::POLYGON;
    for(unsigned int i=0; i<nLados; i++)
        baseCima.indexVec.push_back((nLados-i-1)+nLados);
    
    baseBaixo.material = setMaterial();
    baseCima.material = setMaterial();

    this->AddMesh(baseBaixo);
    this->AddMesh(baseCima);
}

Material Prisma::setMaterial(){
    Material material;
    Color cor;
    cor.SetR(rand()%256);
    cor.SetG(rand()%256);
    cor.SetB(rand()%256);
    cor.SetA(rand()%256);
    material.SetEmissiveColor(cor);
    return material;
}

int main(int argc, char* argv[]){
    ViewerGlutOGL::Init(&argc, argv);
    Scene scene;
    ViewerGlutOGL viewer;
    Camera camera(Point4D(-8, 5, 8), Point4D(0,3,0), Point4D::Y());

    Transform cisalha;
    
    srand(time(0));

    Prisma prisma(7, 2, 6);

    cisalha.AddChild(prisma);

    cisalha.MakeShear(0, 0.5);

    scene.AddObject(&prisma);

    scene.AddLight(Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    viewer.SetTitle("Prisma");
    viewer.SetScene(scene);

    scene.DrawLightsOGL();
    ViewerGlutOGL::MainLoop();

    return 0;
}