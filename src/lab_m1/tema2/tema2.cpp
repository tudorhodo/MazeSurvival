#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


bool Tema2::verifyNeighbors(vector<vector<int>> grid, int x, int y) {
    int number = 0;

    if (x <= 0 || y <= 0 || x >= 14 || y >= 14)
        return false;

    if (grid[x - 1][y] == 0)
        number++;

    if (grid[x + 1][y] == 0)
        number++;

    if (grid[x][y - 1] == 0)
        number++;

    if (grid[x][y + 1] == 0) {
        number++;
    }

    if (number == 1)
        return true;
    
    return false;
}


char Tema2::directionChoice(vector<vector<int>> grid, int x, int y) {
    char positionsAvailable[4];
    int i = 0;

    if (verifyNeighbors(grid, x - 1, y))
        positionsAvailable[i++] = 'N';

    if (verifyNeighbors(grid, x + 1, y))
        positionsAvailable[i++] = 'S';

    if (verifyNeighbors(grid, x, y - 1))
        positionsAvailable[i++] = 'W';

    if (verifyNeighbors(grid, x, y + 1))
        positionsAvailable[i++] = 'E';

    if (i == 0)
        return 0;

    return positionsAvailable[rand() % i];
}


bool Tema2::nextHop(vector<vector<int>> grid, int& x, int& y) {
    char dir = directionChoice(grid, x, y);

    switch (dir) {
    case 'N':
        x--;
        return true;
    case 'S':
        x++;
        return true;
    case 'W':
        y--;
        return true;
    case 'E':
        y++;
        return true;
    default:
        break;
    }
    
    return false;
}


vector<vector<int>> Tema2::generateGrid() {
    vector<vector<int>> grid(15, vector<int>(15, 1));
    vector<tuple<int, int>> zeroes;

    bool exitFound = false;
    int x_exit = -1;
    int y_exit = -1;

    int x_start;
    int y_start;

    if (rand() % 2 == 0) {
        x_start = 0;
        y_start = rand() % 15;
        grid[x_start][y_start] = 3;
        x_start++;
        grid[x_start][y_start] = 0;
    }
    else {
        x_start = rand() % 15;
        y_start = 0;
        grid[x_start][y_start] = 3;
        y_start++;
        grid[x_start][y_start] = 0;
    }


    bool change;
    bool fullExits = false;

    do {
       change = nextHop(grid, x_start, y_start);

       if (change) {
           grid[x_start][y_start] = 0;
           zeroes.push_back(make_tuple(x_start, y_start));
           continue;
       }
       else if (fullExits == false) {
           if (rand() % 2 == 0) {
               x_start = 0;
               y_start = rand() % 15;

               grid[x_start][y_start] = 3;
               x_start++;

               grid[x_start][y_start] = 0;
           }
           else {
               x_start = rand() % 15;
               y_start = 0;

               grid[x_start][y_start] = 3;
               y_start++;

               grid[x_start][y_start] = 0;
           }


           change = true;
           fullExits = true;
       }
       else {
           int step = 0;
           while (step < zeroes.size() / 2) {
               step++;
               int nextLocation = rand() % zeroes.size();
               x_start = get<0>(zeroes[nextLocation]);
               y_start = get<1>(zeroes[nextLocation]);

               change = nextHop(grid, x_start, y_start);

               if (change) {
                   grid[x_start][y_start] = 0;
                   zeroes.push_back(make_tuple(x_start, y_start));
                   break;
               }
           }
       }
    } while (zeroes.size() < 90 && change);

    int total = zeroes.size() % 50;

    for (int i = 0; i < total; i++) {
        int r = rand() % zeroes.size();

        auto it = zeroes.begin() + r;
        int x_enemy = get<0>(*it);
        int z_enemy = get<1>(*it);
        zeroes.erase(it);

        inamici.push_back(Enemy(x_enemy * 8, z_enemy * 8));
        grid[x_enemy][z_enemy] = 2;
    }

    int locationStart = rand() % zeroes.size();

    x_body = get<0>(zeroes[locationStart]) * 8;
    z_body = get<1>(zeroes[locationStart]) * 8;

    return grid;
}


void Tema2::Init()
{
    renderCameraTarget = true;

    userHP = 5;
    escapeTime = 200;

    grid = generateGrid();

    y_body = 1.15f;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(x_body, y_body + 2.5f, z_body - 4), glm::vec3(x_body, y_body, z_body), glm::vec3(0, 1, 0));
    camera->distanceToTarget = 4.71f;

    x_body = camera->GetTargetPosition().x;
    y_body = camera->GetTargetPosition().y;
    z_body = camera->GetTargetPosition().z;


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
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("MyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    anglePerspektiva = 60;
    left = -2;
    right = 2;
    bottom = -2;
    top = 2;
    zNear = 3.4f;
    zFar = 20;

    projectionMatrix = glm::perspective(RADIANS(anglePerspektiva), window->props.aspectRatio, 0.01f, 200.0f);

    //wireframe
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0), glm::vec3(0, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, -0.5f,  0), glm::vec3(0, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 0.5f,  0), glm::vec3(0, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0.5f,  0), glm::vec3(0, 0, 0), glm::vec3(0.2, 0.8, 0.6))
        };

        vector<unsigned int> indices =
        {
            0, 1, 3, 2
        };

        meshes["wireframe"] = new Mesh("wireframe");
        meshes["wireframe"]->SetDrawMode(GL_LINE_LOOP);
        meshes["wireframe"]->InitFromData(vertices, indices);
    }

    //yellow square
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0), glm::vec3(1, 0.9, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, -0.5f,  0), glm::vec3(1, 0.9, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 0.5f,  0), glm::vec3(1, 0.9, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0.5f,  0), glm::vec3(1, 0.9, 0), glm::vec3(0.2, 0.8, 0.6))
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            2, 1, 3
        };

        meshes["yellow_square"] = new Mesh("yellow_square");
        meshes["yellow_square"]->SetDrawMode(GL_TRIANGLES);
        meshes["yellow_square"]->InitFromData(vertices, indices);
    }

    //blue square
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, -0.5f,  0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 0.5f,  0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0.5f,  0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6))
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            2, 1, 3
        };

        meshes["blue_square"] = new Mesh("blue_square");
        meshes["blue_square"]->SetDrawMode(GL_TRIANGLES);
        meshes["blue_square"]->InitFromData(vertices, indices);
    }
}


void Tema2::FrameStart()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{

    GenerateMap();

    escapeTime -= deltaTimeSeconds;

    if (escapeTime <= 0) {
        cout << "Game Over!\n";
        return;
    }

    if (userHP == 0) {
        cout << "Game Over!\n";
        return;
    }

    float x = camera->GetTargetPosition().x;
    float z = camera->GetTargetPosition().z;
    if (Win(x, z)) {
        cout << "WIN!!! BOSS NUMBER ONE!\n";
        return;
    }

    EnemyProjectileContact();

    EnemyCharacterContact();

    MoveBullets(deltaTimeSeconds);

    MoveEnemies(deltaTimeSeconds);

    DrawBullets();

    DrawCharacter();

    DrawEnemies();

    DrawDead(deltaTimeSeconds);

    DrawUI();
}


void Tema2::FrameEnd(){}


void Tema2::DrawUI() {
    //hp
    glm::mat3 modelMatrix_2d = glm::mat3(1);
    modelMatrix_2d *= transform2D::Translate(-0.65, 0.8);
    modelMatrix_2d *= transform2D::Scale(0.5f, 0.25f);
    RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], modelMatrix_2d);

    modelMatrix_2d = glm::mat3(1);
    modelMatrix_2d *= transform2D::Translate(-0.9f + 0.05f * userHP, 0.8);
    modelMatrix_2d *= transform2D::Scale(0.1f * userHP, 0.25f);
    RenderMesh2D(meshes["yellow_square"], shaders["VertexColor"], modelMatrix_2d);

    //time
    modelMatrix_2d = glm::mat3(1);
    modelMatrix_2d *= transform2D::Translate(0.65, 0.8);
    modelMatrix_2d *= transform2D::Scale(0.5f, 0.25f);
    RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], modelMatrix_2d);

    modelMatrix_2d = glm::mat3(1);
    modelMatrix_2d *= transform2D::Translate(0.4f + 0.00125f * escapeTime, 0.8);
    modelMatrix_2d *= transform2D::Scale(0.0025f * escapeTime, 0.25f);
    RenderMesh2D(meshes["blue_square"], shaders["VertexColor"], modelMatrix_2d);
}


bool Tema2::RestrictedArea(float x, float z) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (grid[i][j] == 1) {
                if (x + 1 >= i * 8 - 4 && x - 1 <= i * 8 + 4 && z + 1 >= j * 8 - 4 && z - 1 <= j * 8 + 4) {
                    return true;
                }
            }
        }
    }

    return false;
}


bool Tema2::Win(float x, float z) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (grid[i][j] == 3) {
                if (x + 1 >= i * 8 - 4 && x - 1 <= i * 8 + 4 && z + 1 >= j * 8 - 4 && z - 1 <= j * 8 + 4) {
                    return true;
                }
            }
        }
    }

    return false;
}


void Tema2::EnemyCharacterContact() {
    for (auto it = inamici.begin(); it < inamici.end();) {
        if (glm::distance(glm::vec3(it->x, 1, it->z), camera->GetTargetPosition()) <= 2) {
            it = inamici.erase(it);
            cout << "lovit, au\n";
            userHP--;
            if (userHP == 0) {
                cout << "Game Over!\n";
            }
        }
        else {
            it++;
        }
    }
}


void Tema2::MoveEnemies(float deltaTimeSeconds) {
    for (auto it = inamici.begin(); it < inamici.end(); it++) {
        
        switch (it->dir)
        {
        case 1:
            it->x += deltaTimeSeconds * 5;
            it->toTraverse -= deltaTimeSeconds * 5;
            break;
        case 2:
            it->z += deltaTimeSeconds * 5;
            it->toTraverse -= deltaTimeSeconds * 5;
            break;
        case 3:
            it->x -= deltaTimeSeconds * 5;
            it->toTraverse -= deltaTimeSeconds * 5;
            break;
        case 4:
            it->z -= deltaTimeSeconds * 5;
            it->toTraverse -= deltaTimeSeconds * 5;
            break;
        default:
            break;
        }

        if (it->toTraverse <= 0) {
            if (it->dir == 4) {
                it->dir = 1;
            }
            else {
                it->dir++;
            }

            it->toTraverse = 4;
        }
    }
}


void Tema2::DrawEnemies() {
    for (auto it = inamici.begin(); it < inamici.end(); it++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(it->x, 1, it->z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 3, 3));

        RenderSimpleMesh(meshes["sphere"], shaders["MyShader"], modelMatrix, glm::vec3(0.5, 0, 1));
    }
}


void Tema2::EnemyProjectileContact() {
    for (auto inamic = inamici.begin(); inamic < inamici.end();) {
        bool eliminat = false;
        for (auto proiectil = proiectile.begin(); proiectil < proiectile.end();) {
            if (glm::distance(proiectil->position, glm::vec3(inamic->x, 1, inamic->z)) <= 3) {
                morti.push_back(BoomEnemy(inamic->x, inamic->z, 0));
                inamic = inamici.erase(inamic);
                eliminat = true;
                proiectile.erase(proiectil);
                break;
            }
            
            proiectil++;
        }

        if (!eliminat) {
            inamic++;
        }
    }
}


void Tema2::DrawDead(float deltaTimeSeconds) {
    for (auto it = morti.begin(); it < morti.end();) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(it->x, 1, it->z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
        it->ora_deces += deltaTimeSeconds;

        RenderSimpleMesh(meshes["sphere"], shaders["MyShader"], modelMatrix, glm::vec3(1, 0, 0), true, it->ora_deces);

        if (it->ora_deces >= 2.5f) {
            it = morti.erase(it);
        }
        else {
            it++;
        }
    }
}


void Tema2::DrawCharacter() {
    if (!renderCameraTarget) return;

    float angularStep;
    double dx = camera->position.x - x_body;
    double dz = camera->position.z - z_body;

    if (dz < 0) {
        angularStep = atan(dx / dz);
    }
    else {
        angularStep = M_PI + atan(dx / dz);
    }

    x_body = camera->GetTargetPosition().x;
    y_body = camera->GetTargetPosition().y;
    z_body = camera->GetTargetPosition().z;

    //head
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body, y_body, z_body));
    modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));

    RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, glm::vec3(1, 0.85, 0.67));

    //body
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body, y_body - 0.6f, z_body));
    modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f, 0.75f, 0.3f));

    RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0.74, 1));

    //shoulders
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body, y_body - 0.6f, z_body));
    modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-x_body, -y_body + 0.6f, -z_body));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body - 0.5f, y_body - 0.35f, z_body));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.25f, 0.3f));

    RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0.74, 1));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body, y_body - 0.6f, z_body));
    modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-x_body, -y_body + 0.6f, -z_body));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body + 0.5f, y_body - 0.35f, z_body));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.25f, 0.3f));

    RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0.74, 1));

    //arms
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body, y_body - 0.6f, z_body));
    modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-x_body, -y_body + 0.6f, -z_body));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body - 0.5f, y_body - 0.8f, z_body));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.6f, 0.3f));

    RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, glm::vec3(1, 0.85, 0.67));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body, y_body - 0.6f, z_body));
    modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-x_body, -y_body + 0.6f, -z_body));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body + 0.5f, y_body - 0.8f, z_body));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.6f, 0.3f));

    RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, glm::vec3(1, 0.85, 0.67));

    //legs
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body, y_body - 0.6f, z_body));
    modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-x_body, -y_body + 0.6f, -z_body));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body - 0.25f, y_body - 1.3f, z_body));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.6f, 0.3f));

    RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0, 0.8));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body, y_body - 0.6f, z_body));
    modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-x_body, -y_body + 0.6f, -z_body));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(x_body + 0.25f, y_body - 1.3f, z_body));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.6f, 0.3f));

    RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0, 0.8));
}


void Tema2::GenerateMap() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (grid[i][j] == 1) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i * 8, 1, j * 8));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(8, 2, 8));

                RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0.58, 0.18));
            }
        }
    }

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(56, -0.4f, 56));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.4, 1, 2.4));

    RenderSimpleMesh(meshes["plane"], shaders["MyShader"], modelMatrix, glm::vec3(0, 0, 0));
}


void Tema2::MoveBullets(float deltaTime) {
    for (auto it = proiectile.begin(); it < proiectile.end();) {
        it->position += glm::normalize(it->forward) * deltaTime * 2.f;

        if (glm::distance(it->start, it->position) >= 3) {
            it = proiectile.erase(it);
        }
        else {
            it++;
        }
    }
}


void Tema2::DrawBullets() {
    for (auto it = proiectile.begin(); it < proiectile.end(); it++) {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, it->position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
        RenderSimpleMesh(meshes["sphere"], shaders["MyShader"], modelMatrix, glm::vec3(1, 0, 1));
    }
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color, bool explode, float timp)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    int locationModel = glGetUniformLocation(shader->program, "Model");

    glUniformMatrix4fv(locationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int locationView = glGetUniformLocation(shader->program, "View");

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(locationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int locationColor = glGetUniformLocation(shader->program, "MyColor");
    glUniform3fv(locationColor, 1, glm::value_ptr(color));

    int locationExplode = glGetUniformLocation(shader->program, "Explode");
    glUniform1i(locationExplode, explode);


    int locationTime = glGetUniformLocation(shader->program, "Time");
    glUniform1f(locationTime, timp);
    int locationProjection = glGetUniformLocation(shader->program, "Projection");

    glUniformMatrix4fv(locationProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0); 
}


void Tema2::RenderMesh2D(Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

    glm::mat3 mm = modelMatrix;
    glm::mat4 model = glm::mat4(
        mm[0][0], mm[0][1], mm[0][2], 0.f,
        mm[1][0], mm[1][1], mm[1][2], 0.f,
        0.f, 0.f, mm[2][2], 0.f,
        mm[2][0], mm[2][1], 0.f, 1.f);

    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 3.5f;
    if (userHP == 0) {
        return;
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(cameraSpeed * deltaTime);
        float x = camera->GetTargetPosition().x;
        float z = camera->GetTargetPosition().z;
        if (RestrictedArea(x, z)) {
            camera->MoveForward(-cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        camera->TranslateRight(-cameraSpeed * deltaTime);
        float x = camera->GetTargetPosition().x;
        float z = camera->GetTargetPosition().z;
        if (RestrictedArea(x, z)) {
            camera->TranslateRight(cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        camera->MoveForward(-cameraSpeed * deltaTime);
        float x = camera->GetTargetPosition().x;
        float z = camera->GetTargetPosition().z;
        if (RestrictedArea(x, z)) {
            camera->MoveForward(cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        camera->TranslateRight(cameraSpeed * deltaTime);
        float x = camera->GetTargetPosition().x;
        float z = camera->GetTargetPosition().z;
        if (RestrictedArea(x, z)) {
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        camera->TranslateUpward(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        camera->TranslateUpward(-cameraSpeed * deltaTime);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_LEFT_CONTROL) {
        if (renderCameraTarget)
        {
            renderCameraTarget = false;
            if (camera->distanceToTarget == 4.71f) {
                camera->TranslateForward(4.71f);
                camera->distanceToTarget = 0;
            }
        }
        else {
            renderCameraTarget = true;
            if (camera->distanceToTarget == 0) {
                camera->TranslateForward(-4.71f);
                camera->distanceToTarget = 4.71f;
            }
        }
    }

    if (!renderCameraTarget && key == GLFW_KEY_SPACE) {
        proiectile.push_back(Projectile(camera->position, camera->forward));
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.01f;
    float sensivityOY = 0.01f;

    
    if (renderCameraTarget) {
        camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
    }
    else {
        camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
        camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_3) {
        if (renderCameraTarget)
        {
            renderCameraTarget = false;
            if (camera->distanceToTarget == 4.71f) {
                camera->TranslateForward(4.71f);
                camera->distanceToTarget = 0;
            }
        }
        else {
            renderCameraTarget = true;
            if (camera->distanceToTarget == 0) {
                camera->TranslateForward(-4.71f);
                camera->distanceToTarget = 4.71f;
            }
        }
    }

    if (!renderCameraTarget && button == GLFW_MOUSE_BUTTON_2) {
        proiectile.push_back(Projectile(camera->position, camera->forward));
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
