#pragma once
#include "GameObject.h"




class Fuel : public GameObject {

public:
	float MaxFuel = 100;
	float MinFuel = 0;
	float curentFuel = MaxFuel;
	
	glm::mat4 getModelMatrix() {

		this->scale.y = this->curentFuel / this->MaxFuel;

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, this->getPosition());

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
		modelMatrix = glm::scale(modelMatrix, this->getScale());
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));

		return modelMatrix;
	}

	int specificRenderElements(){
		return 3;
	}

	Fuel(glm::vec3 position, glm::vec3 scale, Mesh* mesh, Shader* shader, glm::vec3 color):GameObject(position, scale, mesh, shader, color) {

	}

};