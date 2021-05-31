#pragma once
#include <Component/SimpleScene.h>
#include "Tema2Camera.h"
#include "Platform.h"
#include "GameObject.h"
#include "Player.h"
#include "Fuel.h"
#include "Life.h"



class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;
		void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

		Player* player;
		float startPlayerUpdate = 2;
		Fuel* fuel;
		Fuel* fuelBackground;
		Life* life;
		Life* lock;
		std::vector<Platform*> mainPath;

		



	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;


		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		void RenderGameObject(GameObject *object);



	protected:
		glm::mat4 projectionMatrix;
		int maxMainPathPlatforms = 10;

};
