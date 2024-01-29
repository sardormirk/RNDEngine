#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{

public:
    GLuint shaderProgram = NULL;
    enum ErrCode
    {
        VERTEX,
        FRAGMENT,
        PROGRAM

    };

    Shader(const char *vShaderPath, const char *fShaderPath)
    {

        std::string vShaderCode = loadShader(vShaderPath);
        std::string fShaderCode = loadShader(fShaderPath);
        const char *vShaderSource = vShaderCode.c_str();
        const char *fShaderSource = fShaderCode.c_str();

        GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vShader, 1, &vShaderSource, NULL);
        glShaderSource(fShader, 1, &fShaderSource, NULL);

        glCompileShader(vShader);
        checkCompileError(vShader, VERTEX);

        glCompileShader(fShader);
        checkCompileError(fShader, FRAGMENT);

        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vShader);
        glAttachShader(shaderProgram, fShader);
        glLinkProgram(shaderProgram);

        checkCompileError(shaderProgram, PROGRAM);

        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }

    void use()
    {
        glUseProgram(shaderProgram);
    }

    void setMat4(const std::string &name, glm::mat4 matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void setVec3(const std::string &name, const glm::vec3 &vec)
    {
        glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vec[0]);
    }

    void setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z)
    {
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }

    std::string loadShader(const char *shaderPath)
    {
        std::string shaderCode;
        std::ifstream shaderFile;
        std::stringstream sFileBuf;

        sFileBuf.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            shaderFile.open(shaderPath);
            sFileBuf << shaderFile.rdbuf();
            shaderCode = sFileBuf.str();
        }
        catch (std::ifstream::failure err)
        {
            std::cout << "ERROR READING FILE" << std::endl;
        }

        return shaderCode;
    }

private:
    void checkCompileError(GLuint type, ErrCode err)
    {
        GLchar infoLog[1024];
        GLint success;

        if (err == PROGRAM)
        {
            glGetProgramiv(type, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(type, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM ERROR of type: \n"
                          << infoLog << "\n------------------------" << std::endl;
            }
        }
        else if (err == VERTEX)
        {
            glGetShaderiv(type, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(type, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: \n"
                          << infoLog << "\n------------------------" << std::endl;
            }
        }
        else if (err == FRAGMENT)
        {
            glGetShaderiv(type, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(type, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: \n"
                          << infoLog << "\n------------------------" << std::endl;
            }
        }
    }
};