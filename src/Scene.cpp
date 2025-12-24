#include "Scene.hpp"

gps::Scene::Scene()
{



}

void gps::Scene::initializeLights(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD)
{

	this->lightSources.setDirecLightParameters(direction, ambientD, diffuseD, specularD);
}
