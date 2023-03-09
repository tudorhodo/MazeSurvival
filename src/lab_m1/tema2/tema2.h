#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
         Tema2();
        ~Tema2();

        void Init() override;

        struct Projectile {
            Projectile(glm::vec3 position, glm::vec3 forward)
                : position(position), start(position), forward(forward) {}
            glm::vec3 position;
            glm::vec3 start;
            glm::vec3 forward;
        };

        struct Enemy {
            Enemy(float x, float z)
                : x(x - 2), z(z - 2), toTraverse(4), dir(1) {}
            float toTraverse;
            float x;
            float z;
            int dir;
        };

        struct BoomEnemy {
            BoomEnemy(float x, float z, float deces)
                : x(x), z(z), ora_deces(deces) {}
            float x;
            float z;
            float ora_deces;
        };

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void MoveBullets(float deltaTime);
        void DrawBullets();

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color, bool explode = false, float timp = 0);
        void RenderMesh2D(Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        vector<vector<int>> generateGrid();
        char directionChoice(vector<vector<int>> grid, int x, int y);
        bool verifyNeighbors(vector<vector<int>> grid, int x, int y);
        bool nextHop(vector<vector<int>> grid, int& x, int& y);
        void GenerateMap();
        void DrawCharacter();

        void DrawEnemies();
        void MoveEnemies(float deltaTimeSeconds);
        void EnemyProjectileContact();
        void DrawDead(float deltaTimeSeconds);

        bool RestrictedArea(float x, float z);
        void EnemyCharacterContact();
        bool Win(float x, float z);

        void DrawUI();

     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        float anglePerspektiva;
        bool perspektiva = true;
        float left, right, bottom, top, zNear, zFar;
        float x_body, y_body, z_body;
        int userHP;
        float escapeTime;

        vector<vector<int>> grid;
        vector<Projectile> proiectile;
        vector<Enemy> inamici;
        vector<BoomEnemy> morti;
    };
}   // namespace m1
