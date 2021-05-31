#pragma once
#include <include/glm.h>
#include "Tema2.h"

class GameObject {


public:
	
	GameObject(const glm::vec3& position, const glm::vec3& scale, Mesh* mesh, Shader* shader) {
		this->position = position;
		this->scale = scale;
		this->mesh = mesh;
		this->shader = shader;
		this->angle = 0;

		forward = glm::vec3(0, 0, -1);
		up = glm::vec3(0, 1, 0);
		right = glm::vec3(1, 0, 0);
		center = glm::vec3(0, 1, -3.5f);
	}

	GameObject(const glm::vec3& position, const glm::vec3& scale, Mesh* mesh, Shader* shader, const glm::vec3& color) {
		this->position = position;
		this->scale = scale;
		this->mesh = mesh;
		this->shader = shader;
		this->angle = 0;
		this->color = color;

		forward = glm::vec3(0, 0, -1);
		up = glm::vec3(0, 1, 0);
		right = glm::vec3(1, 0, 0);
		center = glm::vec3(0, 1, -3.5f);
	}

	GameObject(const glm::vec3& position, const glm::vec3& scale, float angle, Mesh* mesh, Shader* shader) {
		this->position = position;
		this->scale = scale;
		this->mesh = mesh;
		this->shader = shader;
		this->angle = angle;

		forward = glm::vec3(0, 0, -1);
		up = glm::vec3(0, 1, 0);
		right = glm::vec3(1, 0, 0);
		
	}

	GameObject(const glm::vec3& position, const glm::vec3& scale, float angle, Mesh* mesh, Shader* shader, const glm::vec3& color) {
		this->position = position;
		this->scale = scale;
		this->mesh = mesh;
		this->shader = shader;
		this->angle = angle;
		this->color = color;

		forward = glm::vec3(0, 0, -1);
		up = glm::vec3(0, 1, 0);
		right = glm::vec3(1, 0, 0);

	}

	GameObject(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& scale, Mesh* mesh, Shader* shader) {
		this->position = position;
		this->velocity = velocity;
		this->scale = scale;
		this->mesh = mesh;
		this->shader = shader;
		this->angle = 0;

		forward = glm::vec3(0, 0, -1);
		up = glm::vec3(0, 1, 0);
		right = glm::vec3(1, 0, 0);

		
	}

	GameObject(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& scale, Mesh* mesh, Shader* shader, const glm::vec3& color) {
		this->position = position;
		this->velocity = velocity;
		this->scale = scale;
		this->mesh = mesh;
		this->shader = shader;
		this->angle = 0;
		this->color = color;

		forward = glm::vec3(0, 0, -1);
		up = glm::vec3(0, 1, 0);
		right = glm::vec3(1, 0, 0);


	}

	bool checkBounds() {

		return (this->position.z < this->scale.z);

	}
	
	void TranslateForward(float distance)
	{
		this->position += glm::normalize(forward) * distance;
	}

	void TranslateUpword(float distance)
	{
		this->position += distance * glm::normalize(up);
	}

	void TranslateRight(float distance)
	{
		this->position += distance * glm::normalize(right);
	}

	glm::vec3 getScale() {
		return this->scale;
	}

	float getAngle() {
		return this->angle;
	}


	Mesh* getMesh(){
		return this->mesh;
	}
	
	Shader* getShader() {
		return this->shader;
	}

	glm::vec3 getVelocity() {
		return this->velocity;
	}

	void setVelocity(glm::vec3 velocity) {
		this->velocity = velocity;
	}
	
	glm::vec3 getPosition() {
		return this->position;
	}

	void setPosition(glm::vec3 position) {
		this->position = position;
	}

	void addVelocity(glm::vec3 velocity) {
		this->velocity += velocity;
	}

	glm::vec3 getAccleleration() {
		return this->acceleration;
	}

	void setAccleleration(glm::vec3 acceleration) {
		this->acceleration = acceleration;
	}

	virtual glm::mat4 getModelMatrix() = 0;
	virtual int specificRenderElements() = 0;

	glm::vec3 getColor() {
		return this->color;
	}

	void setColor(glm::vec3 color) {
		this->color = color;
	}

public:
	glm::vec3 position;
	glm::vec3 scale;
	float angle;
	Mesh* mesh;
	Shader* shader;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	glm::vec3 center;
	glm::vec3 color;


};