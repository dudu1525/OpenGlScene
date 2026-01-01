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
void gps::LightSources::setPointLightParameters(vec3 position, vec3 ambientD, vec3 diffuseD, vec3 specularD, float linear, float quadratic)
{
	this->pointlight.position = position;
	this->pointlight.ambient = ambientD;
	this->pointlight.diffuse = diffuseD;
	this->pointlight.specular = specularD;
	this->pointlight.linear = linear;
	this->pointlight.quadratic = quadratic;
}
void gps::LightSources::setDirectionalUniforms(GLuint programId)
{

	glUniform3fv(glGetUniformLocation(programId,  "dirLight.direction"), 1, glm::value_ptr(dirrlight.direction));
	glUniform3fv(glGetUniformLocation(programId, "dirLight.ambient"), 1, glm::value_ptr(dirrlight.ambient));
	glUniform3fv(glGetUniformLocation(programId, "dirLight.diffuse"), 1, glm::value_ptr(dirrlight.diffuse));
	glUniform3fv(glGetUniformLocation(programId, "dirLight.specular"), 1, glm::value_ptr(dirrlight.specular));

}
void gps::LightSources::setPointUniforms(GLuint programId)
{
	glUniform3fv(glGetUniformLocation(programId, "pointlight.position"), 1, glm::value_ptr(pointlight.position));
	glUniform3fv(glGetUniformLocation(programId, "pointlight.ambient"), 1, glm::value_ptr(pointlight.ambient));
	glUniform3fv(glGetUniformLocation(programId, "pointlight.diffuse"), 1, glm::value_ptr(pointlight.diffuse));
	glUniform3fv(glGetUniformLocation(programId, "pointlight.specular"), 1, glm::value_ptr(pointlight.specular));
	glUniform1f(glGetUniformLocation(programId, "pointlight.constant"), pointlight.constant);
	glUniform1f(glGetUniformLocation(programId, "pointlight.linear"), pointlight.linear);
	glUniform1f(glGetUniformLocation(programId, "pointlight.quadratic"), pointlight.quadratic);


}
void gps::LightSources::setLightUniforms(GLuint programId)
{
	setDirectionalUniforms(programId);
	setPointUniforms(programId);

}
