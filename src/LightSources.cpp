#include "LightSources.h"


gps::LightSources::LightSources()
{


		
}
void gps::LightSources::setDirecLightParameters(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD)
{
	this->dirrlight.direction = direction;
	this->dirrlight.ambient = ambientD;
	this->dirrlight.diffuse = diffuseD;
	this->dirrlight.specular = specularD;

}
void gps::LightSources::setDirectionalUniforms(GLuint programId)
{

	glUniform3fv(glGetUniformLocation(programId,  "dirLight.direction"), 1, glm::value_ptr(dirrlight.direction));
	glUniform3fv(glGetUniformLocation(programId, "dirLight.ambient"), 1, glm::value_ptr(dirrlight.ambient));
	glUniform3fv(glGetUniformLocation(programId, "dirLight.diffuse"), 1, glm::value_ptr(dirrlight.diffuse));
	glUniform3fv(glGetUniformLocation(programId, "dirLight.specular"), 1, glm::value_ptr(dirrlight.specular));

}
void gps::LightSources::setLightUniforms(GLuint programId)
{
	setDirectionalUniforms(programId);
}
