#include <iostream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include "gl.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui.h"
#include "glutils.h"
#include "models.h"
#include "map.h"
#include "character.h"
#include "mathutils.h"

static Character *character;

static void initGL(unsigned int width, unsigned int height)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST); 

    glDepthFunc(GL_LEQUAL);
    glClearColor(1, 0, 1, 1);
}

static void Draw(const glutils::ProgramWorld &program)
{
    static Map *map = Map::Generate();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    map->Draw(program);
    character->Draw(program);
}
 
int main(int argc, char *argv[])
{
    const unsigned int width = 800;
    const unsigned int height = 600;

    size_t seed;
    if(argc < 2)
        seed = std::time(nullptr);
    else
        seed = atoi(argv[1]);
    std::cout << "seed: " << std::dec << seed << std::endl;
    randutils::seed(seed);

    UI ui(width, height, "Test");

    initGL(width, height);
    glutils::ProgramWorld program("shaders/world.vert", "shaders/world.frag");
    glm::mat4 proj = glm::perspective(100.f, static_cast<float>(width)/height, .1f, 10.f);
    program.Apply();
    program.SetProj(proj);

    character = new Character();

    while(ui.PollEvent())
    {
        float theta, phi;
        character->GetThetaAndPhi(theta, phi);

        glm::vec3 eye = spheric(theta, phi, 2.f);
        glm::vec3 lamp = spheric(theta, phi, 1.f);

        glm::mat4 view = glm::lookAt(
            eye,
            glm::vec3(0, 0, 0),
            glm::vec3(0, 0, 1)
        );

        program.Apply();
        program.SetView(view);

        program.SetLightDir(glm::value_ptr(lamp));

        Draw(program);
        ui.Refresh();

        character->Update();
    }

    delete character;

    return EXIT_SUCCESS;
}

