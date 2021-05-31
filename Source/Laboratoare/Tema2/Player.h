#pragma once
#include "GameObject.h"
#include "Tema2Camera.h"
#include <iostream>
#include "Fuel.h"

constexpr float gravityConstant = 5;
constexpr float MaxSpeed = 15;

class Player : public GameObject {

public:

	Player(const glm::vec3& position, const glm::vec3& center, float radius, const glm::vec3& scale, Mesh* mesh, Shader* shader, glm::vec3 color)
		:GameObject(position, scale, mesh, shader, color) {
		this->radius = radius;
		this->acceleration = glm::vec3(0, 0, 2);

		camera = new Laborator::Camera();
		camera->Set(position, center, glm::vec3(0, 1, 0));
	}

	Player(const glm::vec3& position, const glm::vec3& center, float radius, const glm::vec3& scale, float angle, Mesh* mesh, Shader* shader, glm::vec3 color)
		:GameObject(position, scale, angle, mesh, shader, color) {
		this->radius = radius;

		camera = new Laborator::Camera();
		camera->Set(position, center, glm::vec3(0, 1, 0));
	}

	Player(const glm::vec3& position, const glm::vec3& center, float radius, const glm::vec3& velocity, const glm::vec3& scale, Mesh* mesh, Shader* shader, glm::vec3 color)
		:GameObject(position, velocity, scale, mesh, shader, color) {
		this->acceleration = glm::vec3(0, 0, 2);
		this->radius = radius;

		camera = new Laborator::Camera();
		camera->Set(position, center, glm::vec3(0, 1, 0));
	}

	Player(const glm::vec3& position, const glm::vec3& center, float radius, const glm::vec3& velocity, const glm::vec3& scale, Mesh* mesh, Shader* shader)
		:GameObject(position, velocity, scale, mesh, shader) {
		this->acceleration = glm::vec3(0, 0, 2);
		this->radius = radius;

		camera = new Laborator::Camera();
		camera->Set(position, center, glm::vec3(0, 1, 0)); 
	}

	int specificRenderElements() {
		return 1;
	}

	void Update(float deltaTimeSeconds, Fuel *fuel, std::vector<Platform*> mainPath) {
		
		if (this->getLives() == 0) {
			return;
		}

		if (this->colided) {

			this->canJump = true;
			this->position.y = this -> colided -> getPosition().y + this->colided -> getScale().y/2 + this->radius/2;
			this->camera->position.y = this->colided->getPosition().y + this->colided->getScale().y/2 + this -> radius/2;
			this->velocity.y = 0;

			//touching platform
			if (this->colided->specificRenderElements() == 2) {
				if (((Platform*)this->colided)->hasBeenTouched() == false) {

					//red platform
					if (this->colided->color == glm::vec3(1, 0, 0)) {
						this->subtractLives();
					}
					//yellow
					else if (this->colided->color == glm::vec3(1, 1, 0)) {
						fuel->curentFuel -= 0.1 * fuel->MaxFuel;
					}
					//orange
					else if (this->colided->color == glm::vec3(1.000, 0.647, 0.000)) {
						this->lockedSpeed = true;
						this->lockedSpeedRemainingTime = this->lockedSpeedTime;

						for (int i = 0; i < mainPath.size(); i++) {
							mainPath[i]->setVelocity(glm::vec3(0, 0, MaxSpeed));
						}

						this->animationState = 1;

					}
					//green
					else if (this->colided->color == glm::vec3(0, 1, 0)) {
						fuel->curentFuel += 0.1 * fuel->MaxFuel;
					}
				}
			}

		}

		if (this->lockedSpeed) {
			this->lockedSpeedRemainingTime -= deltaTimeSeconds;
		}

		if (this->lockedSpeedRemainingTime < 0) {
			this->lockedSpeedRemainingTime = 0;
			this->lockedSpeed = false;
			this->animationState = 0;
		}

		if (this -> hasJumped == true && this->canJump == true) {
			this->addVelocity(jumpVelocity);
			
			this->canJump = false;
			this->hasJumped = false;
		}

		this->TranslateUpword(deltaTimeSeconds * this->getVelocity().y);
		this->addVelocity(glm::vec3(0, -gravityConstant * deltaTimeSeconds, 0));

		if (this->position.y < -1) {
			this->animationState = 1;
		}

		if (this->position.y < -10) {
			this->subtractLives();
			this->resetPlayerPosition();
		}

	}

	//Sphere VS AABB
	GameObject* colisionCheck(GameObject *object) {

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, object -> getPosition());
		modelMatrix = glm::scale(modelMatrix, object -> getScale());

		float middleX, middleY, middleZ;

		middleX = modelMatrix[3][0];
		middleY = modelMatrix[3][1];
		middleZ = modelMatrix[3][2];

		float minX, minY, minZ;
		float maxX, maxY, maxZ;
		
		minX = middleX - object->getScale().x/2;
		minY = middleY - object->getScale().y/2;
		minZ = middleZ - object->getScale().z/2;

		maxX = middleX + object->getScale().x/2;
		maxY = middleY + object->getScale().y/2;
		maxZ = middleZ + object->getScale().z/2;

		
		minY = maxY;

		float x, y, z;

		x = std::max(minX, std::min(this->getPosition().x, maxX));
		y = std::max(minY, std::min(this->getPosition().y, maxY));
		z = std::max(minZ, std::min(this->getPosition().z, maxZ));

		float distance = sqrt(	(x - this->getPosition().x) * (x - this->getPosition().x) +
								(y - this->getPosition().y) * (y - this->getPosition().y) +
								(z - this->getPosition().z) * (z - this->getPosition().z)
								);

		if (distance < this->radius) {
			return object;
		}

		return NULL;
	}

	void resetPlayerPosition() {
		this->position = glm::vec3(0, 6, 0);
		this->camera->position = glm::vec3(0, 6, 0);
		this->velocity.y = 0;
		this->animationState = 0;
		if (this->camera->firstPerson == false) {
			this->camera->position -= this->camera->forward * this->camera->distanceToTarget;
		}
	}

	void TranslateRight(float distance)
	{
		camera->TranslateRight(distance);
		__super::TranslateRight(distance);
		
	}

	void TranslateForward(float distance)
	{
		camera->TranslateForward(distance);
		__super::TranslateForward(distance);
		
	}

	void TranslateUpword(float distance)
	{
		camera->TranslateUpword(distance);
		__super::TranslateUpword(distance);
		
	}

	bool firstPerson() {
		return this->camera->firstPerson;
	}

	glm::vec3 GetTargetPosition()
	{
		return camera->GetTargetPosition();
	}

	glm::mat4 GetViewMatrix()
	{
		return camera -> GetViewMatrix();
	}

	void toggleFirstPerson() {
		camera->toggleFirstPerson();
	}



	void RotateFirstPerson_OX(float angle)
	{
		camera->RotateFirstPerson_OX(angle);
	}

	void RotateFirstPerson_OY(float angle)
	{
		camera->RotateFirstPerson_OY(angle);
	}

	void RotateFirstPerson_OZ(float angle)
	{
		camera->RotateFirstPerson_OZ(angle);
	}

	void RotateThirdPerson_OX(float angle)
	{
		camera->RotateThirdPerson_OX(angle);
	}

	void RotateThirdPerson_OY(float angle)
	{
		camera->RotateThirdPerson_OY(angle);
	}

	void RotateThirdPerson_OZ(float angle)
	{
		camera->RotateThirdPerson_OZ(angle);
	}

	void setPosition(glm::vec3 position) {
		this->position = position;
	}


	glm::vec3 getPosition() {
		return this->position;
	}

	glm::mat4 getModelMatrix() {

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, this->getPosition());
		modelMatrix = glm::scale(modelMatrix, this->getScale());

		return modelMatrix;
	}

	int getLives() {
		return this->lives;
	}

	void setLives(int lives) {
		this->lives = lives;
	}

	void subtractLives() {
		this->lives--;
	}

	void addLives() {
		this->lives++;
	}

public:

	bool canJump = false;
	bool hasJumped = false;
	int animationState = 0;
	GameObject* colided = NULL;
	bool lockedSpeed = false;
	float lockedSpeedTime = 3;
	float lockedSpeedRemainingTime = lockedSpeedTime;
	bool lockedControls = false;

	glm::vec3 jumpVelocity = glm::vec3(0, 5, 0);

private:
	glm::vec3 color;
	float radius;
	Laborator::Camera* camera;
	int lives = 3;
	

};
