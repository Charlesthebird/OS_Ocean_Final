#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath);
	// activate the shader
	// ------------------------------------------------------------------------
	void use();
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const;

	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const;
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const;
	// ------------------------------------------------------------------------
	void set4f(const std::string &name, float x, float y, float z, float q) const;
	// ------------------------------------------------------------------------
	void set3f(const std::string &name, float x, float y, float z) const;

	void set4m(const std::string & name, glm::mat4 m) const;

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type);
};
#endif