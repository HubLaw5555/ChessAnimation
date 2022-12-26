#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include "stb_image.h"
#include "Shader.h" // this includes glm

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;
const float PI = 3.1415926;
const float MOUSE_SPEED = 0.3f;
