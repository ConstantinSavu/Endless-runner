#pragma once
#include "GameObject.h"


constexpr float PlatformMaxSpeed = 10;
constexpr float PlatformMinSpeed = 2;

class Platform : public GameObject{

public:



	Platform(const glm::vec3& position, const glm::vec3& scale, Mesh* mesh, Shader* shader, glm::vec3 color)
	:GameObject(position, scale, mesh, shader, color) {
		this->acceleration = glm::vec3(0, 0, 2);
	}

	Platform(const glm::vec3& position, const glm::vec3& scale, float angle, Mesh* mesh, Shader* shader, glm::vec3 color)
	:GameObject(position, scale, angle, mesh, shader, color) {
		
	}

	Platform(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& scale, Mesh* mesh, Shader* shader, glm::vec3 color)
		:GameObject(position, velocity, scale, mesh, shader, color) {
		this->acceleration = glm::vec3(0, 0, 2);
	}

	void Update(float deltaTimeSeconds) {
		this->TranslateForward(-deltaTimeSeconds * this->velocity.z);
	}

	int specificRenderElements() {
		return 2;
	}

	glm::mat4 getModelMatrix() {
		
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, this->getPosition());
		modelMatrix = glm::scale(modelMatrix, this->getScale());

		return modelMatrix;
	}

	//only called when colision is detected
	bool hasBeenTouched() {
		bool isTouched = this->touched;
		this->touched = true;
		return isTouched;
	}



private:
	glm::vec3 color;
	bool touched = false;

};
