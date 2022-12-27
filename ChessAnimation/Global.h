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

struct vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct texture {
    unsigned int id;
    std::string type;
    std::string path;
};


const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1050;
const float PI = 3.1415926535f;
const float MOUSE_SPEED = 0.6f;
const float KEY_BATCH = 0.25f;
const float ROTATION_BATCH = 0.5f;
const float CAMERA_VIEWPORT = 2.0f;


inline float degrees_to_radians(float degrees) {
    return degrees *  PI / 180.0f;
}