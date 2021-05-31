#pragma once
#include "GameObject.h"




class Life : public GameObject {

public:
	
	glm::mat4 getModelMatrix() {


		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, this->getPosition());
		modelMatrix = glm::scale(modelMatrix, this->getScale());

		return modelMatrix;
	}

	int specificRenderElements() {
		return 4;
	}

	Life(glm::vec3 position, glm::vec3 scale, Mesh* mesh, Shader* shader, glm::vec3 color) :GameObject(position, scale, mesh, shader, color) {

	}

};
