//
// Created by mateo on 13. 11. 2021..
//

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <initializer_list>
#include <vector>
#include <string>
#include <iterator>
#include <set>



class Shader
{
public:
    unsigned int ID;
    bool usingTess = false;

    Shader();
    unsigned int load_shaders(std::initializer_list<std::string> l);
    bool isUsingTess(){
        return usingTess;
    }

    // activate the shader
    // ------------------------------------------------------------------------
    void use(){glUseProgram(ID);}
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const{
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec3(const std::string &name,const glm::vec3& vec) const{
        glUniform3f(glGetUniformLocation(ID, name.c_str()), vec[0],vec[1],vec[2]);
    }
    void setMat4(const std::string &name,const GLfloat * value) const{
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE,value);
    }
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    static void checkCompileErrors(unsigned int shader, const std::string& type);

};


#endif //SHADER_H