#include "Scene.h"

Scene::Scene(std::string& currentSceneID)
{
	//Todo:
	emptyScene = true;
}

Scene::~Scene()
{
}

Camera* Scene::getCurrentCamera()
{
	return &mainCamera;
}

bool Scene::CheckIfEmpty()
{
	return emptyScene;
}
