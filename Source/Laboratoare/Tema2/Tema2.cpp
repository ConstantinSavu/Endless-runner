  
#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Shader* shader = new Shader("PlatformShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/PlatformVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/PlatformFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("FuelShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FuelVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FuelFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("PlayerShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/PlayerVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/PlayerFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	srand(1);

	
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	player = new Player(glm::vec3(0, 1, 0), glm::vec3(0, 1, -3.5f), 0.1, glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1), meshes["sphere"], shaders["PlayerShader"], glm::vec3(1, 1, 1));
	player->setLives(3);

	fuel = new Fuel(glm::vec3(0.8, 0, 0), glm::vec3(0.1, 1, 0), meshes["box"], shaders["FuelShader"], glm::vec3(0.957, 0.643, 0.376));
	fuelBackground = new Fuel(glm::vec3(0.8, 0, 0), glm::vec3(0.1, 1, 0), meshes["box"], shaders["FuelShader"], glm::vec3(1, 1, 1));

	life = new Life(glm::vec3(-1 + 0.1, 1 - 0.1, 0), glm::vec3(0.1, 0.1, 0), meshes["box"], shaders["FuelShader"], glm::vec3(1.000, 0.412, 0.706));
	
	lock = new Life(glm::vec3(-1 + 0.1, -1 + 0.1, 0), glm::vec3(0.1, 0.1, 0), meshes["box"], shaders["FuelShader"], glm::vec3(1, 1, 1));

	Platform *newPlatform = new Platform(glm::vec3(0,0,0), glm::vec3(0, 0, PlatformMinSpeed) ,glm::vec3(1.0f, 0.2f, 5.0f), meshes["box"], shaders["PlatformShader"], glm::vec3(0, 0, 1));
	mainPath.push_back(newPlatform);
	for (int i = 1; i < maxMainPathPlatforms; i++) {
		Platform* newPlatform = new Platform(glm::vec3(0, 0, i * (-4.8)), glm::vec3(0, 0, PlatformMinSpeed), glm::vec3(1.0f, 0.2f, 5.0f), meshes["box"], shaders["PlatformShader"], glm::vec3(0, 0, 1));
		mainPath.push_back(newPlatform);
	}

	

}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	// Render player if in third person
	if (!player -> firstPerson())
	{
		RenderGameObject(player);
	}

	//fuel calculation
	fuel->curentFuel -= deltaTimeSeconds * mainPath[0] -> getVelocity().z * 0.5;

	
	if (fuel->curentFuel < 0) {
		fuel->curentFuel = 0;
		player->subtractLives();
		fuel->curentFuel = fuel->MaxFuel;
	}
	
	if (player->lockedSpeed) {
		RenderGameObject(lock);
	}

	RenderGameObject(fuel);
	RenderGameObject(fuelBackground);

	life->setPosition(glm::vec3(-1 + 0.1, 1 - 0.1, 0));
	for (int i = 0; i < player->getLives(); i++) {
		RenderGameObject(life);
		life->setPosition(life->getPosition() + glm::vec3(0.2, 0, 0));
	}


	if (!mainPath[0] -> checkBounds()) {
		
		int var = rand() % 3;
		int xOffset = var / 2;

		if (var % 2 == 1) {
			xOffset = -((var + 1) / 2);
		}

		Platform* newPlatform = new Platform(glm::vec3(xOffset, 0, mainPath[mainPath.size() - 1]->getPosition().z - mainPath[mainPath.size() - 1]->getScale().z),
								mainPath[mainPath.size() - 1]->getVelocity(),
								mainPath[mainPath.size() - 1]->getScale(), meshes["box"], shaders["PlatformShader"],
								mainPath[mainPath.size() - 1]->getColor());
		
		var = rand() % 100;

		if (0 <= var && var < 60) {
			//blue platform
			newPlatform->setColor(glm::vec3(0, 0, 1));
		}
		else if (60 <= var && var < 70) {
			//red platform
			newPlatform->setColor(glm::vec3(1, 0, 0));
			//no 2 consecutive red platforms, but blue instead
			if (mainPath[mainPath.size() - 1]->getColor() == glm::vec3(1, 0, 0)) {
				newPlatform->setColor(glm::vec3(0, 0, 1));
			}
		}
		else if (70 <= var && var < 80) {
			//yellow platform
			newPlatform->setColor(glm::vec3(1, 1, 0));
		}
		else if (80 <= var && var < 90) {
			//orange platform
			newPlatform->setColor(glm::vec3(1.000, 0.647, 0.000));
		}
		else if (90 <= var && var < 100) {
			//green platform
			newPlatform->setColor(glm::vec3(0, 1, 0));
		}


		mainPath.push_back(newPlatform);

		mainPath.erase(mainPath.begin());
	}

	
	
	player->colided = NULL;
	for (int i = 0; i < mainPath.size(); i++) {
		mainPath[i]->Update(deltaTimeSeconds);
		if (player->colided == NULL) {
			player->colided = player->colisionCheck(mainPath[i]);
		}
		
	}


	for (int i = 0; i < mainPath.size(); i++) {
		RenderGameObject(mainPath[i]);
	}




	if (startPlayerUpdate < 0) {
		player->Update(deltaTimeSeconds, fuel, mainPath);
	}
	else {
		startPlayerUpdate -= deltaTimeSeconds;
	}
	
}

void Tema2::FrameEnd()
{
	if (player->getLives() == 0) {
		
		player->lockedControls = true;
		player->animationState = 1;
		player->setVelocity(glm::vec3(0, 0, 0));

		for (int i = 0; i < mainPath.size(); i++) {
			mainPath[i]->setVelocity(glm::vec3(0, 0, 0));
		}
	}
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(player->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderGameObject(GameObject *object)
{
	Mesh* mesh = object -> getMesh();
	Shader* shader = object -> getShader();

	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glm::mat4 modelMatrix = object->getModelMatrix();

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	int location1 = glGetUniformLocation(shader->program, "Model");

	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// TODO : get shader location for uniform mat4 "View"
	int location2 = glGetUniformLocation(shader->program, "View");

	// TODO : set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = player->GetViewMatrix();
	glUniformMatrix4fv(location2, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// TODO : get shader location for uniform mat4 "Projection"
	int location3 = glGetUniformLocation(shader->program, "Projection");
	// TODO : set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(location3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int location4 = glGetUniformLocation(object->getShader()->program, "Color");
	glUniform3f(location4, object->getColor().x, object->getColor().y, object->getColor().z);

	int type = object->specificRenderElements();
	//Player
	if (type == 1) {


		if (((Player*)object)->colided) {
			int location5 = glGetUniformLocation(object->getShader()->program, "colidedColor");
			glUniform3f(location5, ((Player*)object)->colided->getColor().x, 
									((Player*)object)->colided->getColor().y, 
									((Player*)object)->colided->getColor().z);
		}

		int location6 = glGetUniformLocation(object->getShader()->program, "animationState");
		glUniform1f(location6, ((Player*)object)->animationState);

		float time = float(Engine::GetElapsedTime());
		int location7 = glGetUniformLocation(shader->program, "elapsedTime");
		glUniform1f(location7, time);

	}
	else if (type == 2) {

	}
	else if (type == 3) {
		
	}
	
	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W) && player->lockedSpeed == false) {
			for (int i = 0; i < mainPath.size(); i++) {

				if (mainPath[i]->getVelocity().z < PlatformMaxSpeed) {
					glm::vec3 velocity = mainPath[i]->getVelocity();
					glm::vec3 acceleration = mainPath[i]->getAccleleration();

					velocity += acceleration * deltaTime;

					mainPath[i]->setVelocity(velocity);

				}
			}

			
			
		}

		if (window->KeyHold(GLFW_KEY_A) && !player->lockedControls) {
			// TODO : translate the camera to the left
			player->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S) && player->lockedSpeed == false) {
			// TODO : translate the camera backwards
			for (int i = 0; i < mainPath.size(); i++) {

				if (mainPath[i]->getVelocity().z > PlatformMinSpeed) {
					glm::vec3 velocity = mainPath[i]->getVelocity();
					glm::vec3 acceleration = mainPath[i]->getAccleleration();

					velocity -= acceleration * deltaTime;

					mainPath[i]->setVelocity(velocity);

				}
			}
		}

		if (window->KeyHold(GLFW_KEY_D) && !player->lockedControls) {
			// TODO : translate the camera to the right
			player->TranslateRight(deltaTime * cameraSpeed);
		}

		
	
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_C) {
		
		player->toggleFirstPerson();

	}
	
	if (key == GLFW_KEY_SPACE && player -> canJump == true) {

		player->hasJumped = true;
		
	}

}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	/*
	//Used for fun
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (player -> firstPerson()) {
			player->RotateFirstPerson_OX(-2 * sensivityOX * deltaY);
			player->RotateFirstPerson_OY(-2 * sensivityOY * deltaX);
		}
		else {
			player->RotateThirdPerson_OX(-2 * sensivityOX * deltaY);
			player->RotateThirdPerson_OY(-2 * sensivityOY * deltaX);
		}

	}
	*/
	
	
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}