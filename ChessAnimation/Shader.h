#ifndef SHADER_H
#define SHADER_H

//opengl libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <glfw3.h>
//

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>




class shader
{

public:

	unsigned int ID;
	shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;

	void setVec3(const std::string& name, glm::vec3 value) const;

	void setVec2(const std::string& name, glm::vec2 value) const;

	void setMat4(const std::string& name, glm::mat4 value) const;
};

#endif