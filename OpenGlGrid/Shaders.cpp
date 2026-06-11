#include "Shaders.h"

#include<GLFW/glfw3.h>
#include <glm/glm.hpp>

//class Shader
//{
//public:
//	unsigned int ID;
//	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
//	{
//		// 1. Получаем исходный код шейдера из filePath
//		std::string vertexCode;
//		std::string fragmentCode;
//		std::ifstream vShaderFile;
//		std::ifstream fShaderFile;
//		// Удостоверимся, что ifstream объекты могут выкидывать исключения
//		vShaderFile.exceptions(std::ifstream::badbit);
//		fShaderFile.exceptions(std::ifstream::badbit);
//		try
//		{
//			// Открываем файлы
//			vShaderFile.open(vertexPath);
//			fShaderFile.open(fragmentPath);
//			std::stringstream fShaderStream, vShaderStream;
//			// Считываем данные в потоки
//			vShaderStream << vShaderFile.rdbuf();
//			fShaderStream << fShaderFile.rdbuf();
//			// Закрываем файлы
//		   // vShaderFile.close();
//			fShaderFile.close();
//			// Преобразовываем потоки в массив GLchar
//			vertexCode = vShaderStream.str();
//			fragmentCode = fShaderStream.str();
//		}
//		catch (std::ifstream::failure e)
//		{
//			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
//		}
//		const GLchar* vShaderCode = vertexCode.c_str();
//		const GLchar* fShaderCode = fragmentCode.c_str();
//
//		GLuint vertex, fragment;
//		GLint success;
//		GLchar infoLog[512];
//
//		// Вершинный шейдер
//		vertex = glCreateShader(GL_VERTEX_SHADER);
//		glShaderSource(vertex, 1, &vShaderCode, NULL);
//		glCompileShader(vertex);
//		// Если есть ошибки - вывести их
//		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
//		if (!success)
//		{
//			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
//			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//		};
//
//		// Фрагментный шейдер
//		fragment = glCreateShader(GL_FRAGMENT_SHADER);
//		glShaderSource(fragment, 1, &fShaderCode, NULL);
//		glCompileShader(fragment);
//		// Если есть ошибки - вывести их
//		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
//		if (!success)
//		{
//			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
//			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//		};
//
//		// Шейдерная программа
//		ID = glCreateProgram();
//		glAttachShader(ID, vertex);
//		glAttachShader(ID, fragment);
//		glLinkProgram(ID);
//		//Если есть ошибки - вывести их
//		glGetProgramiv(ID, GL_LINK_STATUS, &success);
//		if (!success)
//		{
//			glGetProgramInfoLog(ID, 512, NULL, infoLog);
//			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//		}
//
//		// Удаляем шейдеры, поскольку они уже в программу и нам больше не нужны.
//		glDeleteShader(vertex);
//		glDeleteShader(fragment);
//		GLint nrAttributes;
//		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//		std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
//	
//	}
//
//	void Use()
//	{
//		glUseProgram(ID);
//
//	}
//	void setMat4(const std::string& name, const glm::mat4& mat) const
//	{
//		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//	}
//};
