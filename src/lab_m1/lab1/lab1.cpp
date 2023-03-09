#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    culoare_rosu = 0;
    culoare_verde = 0;
    culoare_albastru = 0;
    x = 2;
    y = 2;
    z = z_jos = 2;
    obiect = "teapot";
    jump = false;
    fall = false;
    z_sus = 2.25f;
}


Lab1::~Lab1()
{
}

void Lab1::Jump(float deltaTimeSeconds) {
    printf("%d %d - %f %f\n", jump, fall, z, z_sus);
    if (jump) {
        if (z >= z_sus) {
            jump = false;
            fall = true;
        }
        else {
            z += 0.75f * deltaTimeSeconds;
        }
    }

    if (fall) {
        if (z <= z_jos) {
            fall = false;
        }
        else {
            z -= 0.75f * deltaTimeSeconds;
        }
    }
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(culoare_rosu, culoare_verde, culoare_albastru, 0.2f);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    RenderMesh(meshes["sphere"], glm::vec3(2, 0.5f, 3));

    RenderMesh(meshes["sphere"], glm::vec3(-3, 0.6f, 2), glm::vec3(0.5f));

    Jump(deltaTimeSeconds);

    RenderMesh(meshes[obiect], glm::vec3(x, z, y));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
    if (window->KeyHold(GLFW_KEY_LEFT_CONTROL)) {
        if (window->KeyHold(GLFW_KEY_W) && window->KeyHold(GLFW_KEY_D)) {
            x += 1 * deltaTime;
            y += 1 * deltaTime;

        }
        else if (window->KeyHold(GLFW_KEY_W) && window->KeyHold(GLFW_KEY_A)) {
            x += 1 * deltaTime;
            y -= 1 * deltaTime;

        }
        else if (window->KeyHold(GLFW_KEY_S) && window->KeyHold(GLFW_KEY_D)) {
            x -= 1 * deltaTime;
            y += 1 * deltaTime;

        }
        else if (window->KeyHold(GLFW_KEY_S) && window->KeyHold(GLFW_KEY_A)) {
            x -= 1 * deltaTime;
            y -= 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_W) && window->KeyHold(GLFW_KEY_E)) {
            x += 1 * deltaTime;
            z += 1 * deltaTime;
            z_jos += 1 * deltaTime;
            z_sus += 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_W) && window->KeyHold(GLFW_KEY_Q)) {
            x += 1 * deltaTime;
            z -= 1 * deltaTime;
            z_jos -= 1 * deltaTime;
            z_sus -= 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_S) && window->KeyHold(GLFW_KEY_E)) {
            x -= 1 * deltaTime;
            z += 1 * deltaTime;
            z_jos += 1 * deltaTime;
            z_sus += 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_S) && window->KeyHold(GLFW_KEY_Q)) {
            x -= 1 * deltaTime;
            z -= 1 * deltaTime;
            z_jos -= 1 * deltaTime;
            z_sus -= 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_A) && window->KeyHold(GLFW_KEY_E)) {
            y -= 1 * deltaTime;
            z += 1 * deltaTime;
            z_jos += 1 * deltaTime;
            z_sus += 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_A) && window->KeyHold(GLFW_KEY_Q)) {
            y -= 1 * deltaTime;
            z -= 1 * deltaTime;
            z_jos -= 1 * deltaTime;
            z_sus -= 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_D) && window->KeyHold(GLFW_KEY_E)) {
            y += 1 * deltaTime;
            z += 1 * deltaTime;
            z_jos += 1 * deltaTime;
            z_sus += 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_D) && window->KeyHold(GLFW_KEY_Q)) {
            y += 1 * deltaTime;
            z -= 1 * deltaTime;
            z_jos -= 1 * deltaTime;
            z_sus -= 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_W)) {
            x += 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_S)) {
            x -= 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_A)) {
            y -= 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_D)) {
            y += 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_Q)) {
            z -= 1 * deltaTime;
            z_jos -= 1 * deltaTime;
            z_sus -= 1 * deltaTime;
        }
        else if (window->KeyHold(GLFW_KEY_E)) {
            z += 1 * deltaTime;
            z_jos += 1 * deltaTime;
            z_sus += 1 * deltaTime;
        }
    }

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        culoare_rosu = ((double)rand() / (RAND_MAX));
        culoare_verde = ((double)rand() / (RAND_MAX));
        culoare_albastru = ((double)rand() / (RAND_MAX));
    }

    if (key == GLFW_KEY_ENTER) {
        if (obiect.compare("teapot") == 0) {
            obiect = "box";
        }
        else if (obiect.compare("box") == 0) {
            obiect = "sphere";
        }
        else {
            obiect = "teapot";
        }
    }

    if (key == GLFW_KEY_SPACE) {
        jump = true;
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
