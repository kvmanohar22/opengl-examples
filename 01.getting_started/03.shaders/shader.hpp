#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <glad/glad.h>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

class Shader {
private:
   unsigned int _id;
public:
   Shader(const GLchar *v_path, const GLchar *f_path);
  
   void use();
};

#endif
