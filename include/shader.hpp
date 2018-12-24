#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

class Shader {
private:
    unsigned int _id;
public:
    Shader(const char *v_path, 
           const char *f_path,
           const char *g_path=NULL) {

        std::string v_code_, f_code_, g_code_;
        std::ifstream v_file, f_file, g_file;
        std::stringstream v_stream, f_stream, g_stream;

        try {
            v_file.open(v_path);
            f_file.open(f_path);

            v_stream << v_file.rdbuf();
            f_stream << f_file.rdbuf();

            v_file.close();
            f_file.close();      

            v_code_ = v_stream.str();
            f_code_ = f_stream.str();

            if (g_path != NULL) {
                g_file.open(g_path);
                g_stream << g_file.rdbuf();
                g_file.close();
                g_code_ = g_stream.str();
            }

        } catch(std::ifstream::failure e) {
            std::cout << "Couldn't read the file"
                      << std::endl;
        }

        const char *v_code = v_code_.c_str();
        const char *f_code = f_code_.c_str();
        const char *g_code;
        if (g_path != NULL)
            g_code = g_code_.c_str();

        int success;
        char info_log[512];

        // Vertex Shader
        unsigned int v_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(v_shader, 1, &v_code, NULL);
        glCompileShader(v_shader);
        glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
         glGetShaderInfoLog(v_shader, 512, NULL, info_log);
         std::cout << "Couldn't compile the vertex shader"
                   << info_log
                   << std::endl;
        }

        // Geometry Shader
        unsigned int g_shader;
        if (g_path != NULL) {
            g_shader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(g_shader, 1, &g_code, NULL);
            glCompileShader(g_shader);
            glGetShaderiv(g_shader, GL_COMPILE_STATUS, &success);
            if (!success) {
             glGetShaderInfoLog(g_shader, 512, NULL, info_log);
             std::cout << "Couldn't compile the geometry shader"
                       << info_log
                       << std::endl;
            }
        }

        // Fragment Shader
        unsigned int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(f_shader, 1, &f_code, NULL);
        glCompileShader(f_shader);
        glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
         glGetShaderInfoLog(f_shader, 512, NULL, info_log);
         std::cout << "Couldn't compile the fragment shader"
                   << info_log
                   << std::endl;
        }

        // Shader program
        this->_id = glCreateProgram();
        glAttachShader(_id, v_shader);
        if (g_path != NULL)
            glAttachShader(_id, g_shader);
        glAttachShader(_id, f_shader);
        glLinkProgram(_id);

        glGetProgramiv(_id, GL_LINK_STATUS, &success);
        if (!success) {
         glGetProgramInfoLog(_id, 512, NULL, info_log);
         std::cout << "Couldn't link the shaders"
                   << info_log
                   << std::endl;  
        }
        glDeleteShader(v_shader);
        glDeleteShader(f_shader);
        if (g_path != NULL)
            glDeleteShader(g_shader);
    }

    void use() {
        glUseProgram(_id);
    }

    void seti(const char *name, const int val) {
        glUniform1i(glGetUniformLocation(this->_id, name), val);
    }

    void setf(const char *name, const float val) {
        glUniform1f(glGetUniformLocation(this->_id, name), val);
    }

    void setmat4(const char *name, const glm::mat4 val) {
        glUniformMatrix4fv(glGetUniformLocation(
         this->_id, name), 1, GL_FALSE, glm::value_ptr(val));
    }

    void setvec3(const char *name, const glm::vec3 val) const {
        glUniform3fv(glGetUniformLocation(
         this->_id, name), 1, glm::value_ptr(val));
    }

    void setuniform(const char *name, const int binding_point) const {
        glUniformBlockBinding(this->_id, 
                            glGetUniformBlockIndex(this->_id, 
                                                   name),
                            binding_point
                            );
    }
};

#endif
