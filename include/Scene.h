#pragma once

#include <string>
#include "Camera.h"

class Scene
{
public:
	Scene(std::string& currentSceneID);
	~Scene();

	Camera* getCurrentCamera();

	bool CheckIfEmpty();

private:

	Camera mainCamera;

	bool emptyScene;
};

