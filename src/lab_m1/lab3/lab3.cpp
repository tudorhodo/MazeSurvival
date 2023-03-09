#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}

void Lab3::MoveSquare(float deltaTimeSeconds) {
    if (up) {
        translateY += deltaTimeSeconds * 150;

        if (translateY >= 370) {
            up = false;
        }
    }
    else {
        translateY -= deltaTimeSeconds * 150;

        if (translateY <= -250) {
            up = true;
        }
    }

    if (right) {
        translateX += deltaTimeSeconds * 130;

        if (translateX >= 1030) {
            right = false;
        }
    }
    else {
        translateX -= deltaTimeSeconds * 130;

        if (translateX <= -150) {
            right = true;
        }
    }
}

void Lab3::ScaleSquare(float deltaTimeSeconds) {
    if (scaleX_bool) {
        scaleX += deltaTimeSeconds * 0.3;

        if (scaleX >= 2) {
            scaleX_bool = false;
        }
    }
    else {
        scaleX -= deltaTimeSeconds * 0.3;

        if (scaleX <= 1) {
            scaleX_bool = true;
        }
    }

    if (scaleY_bool) {
        scaleY += deltaTimeSeconds * 0.3;

        if (scaleY >= 3) {
            scaleY_bool = false;
        }
    }
    else {
        scaleY -= deltaTimeSeconds * 0.5;

        if (scaleY <= 1) {
            scaleY_bool = true;
        }
    }
}

void Lab3::RotateSquare(float deltaTimeSeconds) {
    angularStep += deltaTimeSeconds * rotateSpeed;
}

void Lab3::MoveCar(float deltaTimeSeconds) {
    if (directie) {
        translate += deltaTimeSeconds * rotateSpeed * 100;

        if (translate >= 1030) {
            directie = false;
        }
    }
    else {
        translate -= deltaTimeSeconds * rotateSpeed * 100;

        if (translate <= 150) {
            directie = true;
        }
    }
    int sens;
    if (directie) {
        sens = -1;
    }
    else {
        sens = 1;
    }

    Lab3::RotateSquare(sens * deltaTimeSeconds);
}

void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    translate = 400;

    up = true;
    right = true;

    directie = true;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    scaleX_car = 4;
    scaleY_car = 1;

    scaleX_bool = true;
    scaleY_bool = true;

    // Initialize angularStep
    angularStep = 0;
    rotateSpeed = 0.5;

    cx = -(squareSide / 2 + corner.x);
    cy = -(squareSide / 2 + corner.y);

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    // TODO(student): Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,
    // add them over there!

    // Exercitiul 3
    
    Lab3::MoveSquare(deltaTimeSeconds);

    Lab3::ScaleSquare(deltaTimeSeconds);

    Lab3::RotateSquare(deltaTimeSeconds);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(150, 250) * transform2D::Translate(translateX, translateY);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented.
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(400 - cx, 250 - cy) * transform2D::Scale(scaleX, scaleY) * transform2D::Translate(cx, cy);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650 - cx, 250 - cy) * transform2D::Rotate(angularStep) * transform2D::Translate(cx, cy);
    // press enter for surprize(3 times)
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
     

    // Bonus
    // press enter for surprize(3 times)
    /*Lab3::MoveCar(deltaTimeSeconds);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translate - cx, 250 - cy) * transform2D::Scale(scaleX_car, scaleY_car) * transform2D::Translate(cx, cy);

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translate - 150 - cx, 150 - cy) * transform2D::Rotate(angularStep) * transform2D::Translate(cx, cy);
    
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translate + 150 - cx, 150 - cy) * transform2D::Rotate(angularStep) * transform2D::Translate(cx, cy);

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);*/
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods){}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_ENTER) {
        if (rotateSpeed == 0.5) {
            rotateSpeed = 2;
        }
        else if (rotateSpeed == 2) {
            rotateSpeed = 5;
        }
        else if (rotateSpeed == 5) {
            rotateSpeed = 0.5;
        }
    }
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
