// main.cpp - Test application for JointAction

// This application shows how to make simple animation using the class JointAction.

// Changelog
// Oct 19, 2012 - Bruno de Oliveira Schneider
// - Adapted to new keyboard handler methods.
// Jul 15, 2008 - Bruno Schneider
// - Application created.

#include <vart/scene.h>
#include <vart/transform.h>
#include <vart/joint.h>
#include <vart/jointmover.h>
#include <vart/uniaxialjoint.h>
#include <vart/baseaction.h>
#include <vart/jointaction.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/callback.h>
#include <vart/meshobject.h>
#include <vart/box.h>
#include <vart/linearinterpolator.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/cylinder.h>

#include <iostream>
#define PI 3.141592654

using namespace std;

class KeyboardHandler : public VART::ViewerGlutOGL::KbHandler
{
    public:
        KeyboardHandler() {
        }
        virtual void OnKeyDown(int key) {
            switch (key)
            {
                case ' ':
                    actionPtr1->Activate();
                    break;
                case '1':
                    selectedDofPtr = dofPtrVec[0];
                    break;
                case '2':
                    selectedDofPtr = dofPtrVec[1];
                    break;
                case '3':
                    selectedDofPtr = dofPtrVec[2];
                    break;
                case ',':
                    selectedDofPtr->Move(-0.02);
                    break;
                case '.':
                    selectedDofPtr->Move(0.02);
                    break;
                case VART::KEY_UP :
                    tanque->GetVectorX(&ponto);
                    transformacao.MakeTranslation(-ponto);
                    tanque->Apply(transformacao);
                    break;
                case VART::KEY_DOWN:
                    tanque->GetVectorX(&ponto);
                    transformacao.MakeTranslation(ponto);
                    tanque->Apply(transformacao);
                    break;
                case VART::KEY_LEFT:
                    tanque->GetTranslation(&ponto);
                    transformacao.MakeRotation(ponto, VART::Point4D::Y(), 0.2);
                    tanque->Apply(transformacao);
                    break;
                case VART::KEY_RIGHT:
                    tanque->GetTranslation(&ponto);
                    transformacao.MakeRotation(ponto, VART::Point4D::Y(), -0.2);
                    tanque->Apply(transformacao);
                    break; 

            }
            viewerPtr->PostRedisplay();
        }
        VART::Dof* dofPtrVec[3];
        VART::Dof* selectedDofPtr;
        VART::BaseAction* actionPtr1;
        VART::Transform* tanque;
        VART::Point4D* frente;
    private:
        VART::Point4D ponto;
        VART::Transform transformacao;
};

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass(){}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            VART::BaseAction::MoveAllActive();
            viewerPtr->PostRedisplay();
        }
    protected:
    private:
};

using VART::Point4D;

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    cout << "This application shows hot to use JointAction.\n"
         << "Keyboard commands:\n"
         << "1) Selects the base joint\n"
         << "2) Selects the middle joint\n"
         << "3) Selects the top joint\n"
         << ",) Moves selected joint towards minimal flexion\n"
         << ".) Moves selected joint towards maximal flexion\n"
         << " ) The [SPACE] key activates the action that moves the robotic arm to its "
         << "'rest position' within four seconds\n"
         << "q) Quits\n\n";
    // create a camera (scene observer)
    VART::Camera camera(Point4D(-5,8,-10), Point4D::ORIGIN(), Point4D::Y());
    MyIHClass idleHandler;

    // Build up the scene -- begin =======================================================
    // scene -> base
    VART::Box terreno;
    terreno.MakeBox(-20, 20, -0.2, 0, -20, 20);  
    terreno.SetMaterial(VART::Material::DARK_PLASTIC_GRAY());
    scene.AddObject(&terreno);

    //n sera usado
    VART::UniaxialJoint eixoTanque;
    VART::Dof* dofPtr1 = eixoTanque.AddDof(Point4D::Y(), Point4D::ORIGIN(), -PI, PI);
    terreno.AddChild(eixoTanque);
    //.............

    VART::Transform centroTanque;
    centroTanque.MakeIdentity();
    scene.AddObject(&centroTanque);

    VART::Point4D frenteTanque(Point4D::X());

    // base -> tanque
    VART::Box tanque;
    tanque.MakeBox(-4.5,4.5,0,3.5,-2.5,2.5);
    tanque.SetMaterial(VART::Material::PLASTIC_GREEN());
    centroTanque.AddChild(tanque);
    
    VART::UniaxialJoint eixoCabine; // joint from tanque to arm2
    VART::Dof* dofPtr2 = eixoCabine.AddDof(Point4D::Y(), Point4D(0,3.5,0), -PI/2, PI/2);
    tanque.AddChild(eixoCabine);
    
    // eixoCabine -> arm2
    VART::Box cabine;
    cabine.MakeBox(-1.5,1.5, 3.5,5.5, -1.5,1.5);
    cabine.SetMaterial(VART::Material::PLASTIC_GREEN());
    eixoCabine.AddChild(cabine);
    
    VART::UniaxialJoint eixoCanhao; // joint from cabine to arm3
    VART::Dof* dofPtr3 = eixoCanhao.AddDof(Point4D::Z(), Point4D(0,4.5,0), -PI/16, 0.1);
    cabine.AddChild(eixoCanhao);
    
    // eixoCanhao -> arm3
    VART::Transform tr1;
    tr1.MakeYRotation(-PI/2);
    eixoCanhao.AddChild(tr1);

    VART::Transform tr2;
    tr2.MakeTranslation(VART::Point4D(0,4.5,0));
    tr1.AddChild(tr2);

    VART::Cylinder canhao(6, 0.5);
    canhao.SetMaterial(VART::Material::PLASTIC_GREEN());
    tr2.AddChild(canhao);

    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    // Build up the scene -- end =========================================================


    // Set up actions -- begin
    VART::JointAction mover;
    VART::LinearInterpolator interpolator;
    VART::JointMover* baseMoverPtr = mover.AddJointMover(&eixoTanque, 4, interpolator);
    baseMoverPtr->AddDofMover(VART::Joint::FLEXION, 0, 1, 1);
    VART::JointMover* middleMoverPtr = mover.AddJointMover(&eixoCabine, 2, interpolator);
    middleMoverPtr->AddDofMover(VART::Joint::FLEXION, 0, 1, 1);
    VART::JointMover* topMoverPtr = mover.AddJointMover(&eixoCanhao, 2, interpolator);
    topMoverPtr->AddDofMover(VART::Joint::FLEXION, 0, 1, 1);
    // Set up actions -- end


    // Set up handlers, managers ,etc.
    KeyboardHandler kbh;
    kbh.selectedDofPtr = dofPtr1;
    kbh.dofPtrVec[0] = dofPtr1;
    kbh.dofPtrVec[1] = dofPtr2;
    kbh.dofPtrVec[2] = dofPtr3;
    kbh.actionPtr1 = &mover;
    kbh.tanque = &centroTanque;
    kbh.frente = &frenteTanque;

    // Set up the viewer
    viewer.SetScene(scene); // attach the scene
    viewer.SetKbHandler(&kbh);
    viewer.SetIdleHandler(&idleHandler); // attach the idle handler to the viewer
    viewer.SetTitle("JointAction Example");

    //mover.Activate();

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
