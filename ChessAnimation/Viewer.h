#pragma once
#include "Model.h"

class Viewer
{
	glm::mat4 model;
	glm::mat4 projection;
	glm::mat4 view;

public:

	Viewer()
	{
		projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void Rotate(float radians, glm::vec3 direction)
	{
		model = glm::rotate(model, glm::radians(radians), direction);
	}

	void TransformView(glm::vec3 shift)
	{
		view = glm::translate(view, shift);
	}

	void BindUniforms(const Shader& shader)
	{
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
	}
};




