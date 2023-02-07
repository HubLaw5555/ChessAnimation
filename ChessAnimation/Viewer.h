//#pragma once
//#include "model.h"
//
//class viewer
//{
//	glm::mat4 model;
//	glm::mat4 projection;
//	glm::mat4 view;
//
//public:
//
//	viewer()
//	{
//		projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		view = glm::mat4(1.0f);
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//	}
//
//	void rotate(float radians, glm::vec3 direction)
//	{
//		model = glm::rotate(model, glm::radians(radians), direction);
//	}
//
//	void transformView(glm::vec3 shift)
//	{
//		view = glm::translate(view, shift);
//	}
//
//	void bindUniforms(const shader& shader)
//	{
//		shader.setMat4("model", model);
//		shader.setMat4("view", view);
//		shader.setMat4("projection", projection);
//	}
//
//	void setView(glm::mat4 v)
//	{
//		view = v;
//	}
//};
//
//
//
//
