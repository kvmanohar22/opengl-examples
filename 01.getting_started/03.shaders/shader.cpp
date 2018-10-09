#include "shader.hpp"

Shader::Shader(const GLchar *v_path, 
               const GLchar *f_path
              ) {
   std::string v_code, f_code;
   std::ifstream v_file, f_file;
   
   try {
      v_file.open(v_path);
      f_file.open(f_path);
      
      std::stringstream v_stream, f_stream;
      v_stream << v_file.rdbuf();
      f_stream << f_file.rdbuf();
      
      v_file.close();
      f_file.close();      
   } catch(std::ifstream::failure e) {
      std::cout << "Couldn't read the file"
                << std::endl;
   }
   
   const char *v_code_ = v_code.c_str();
   const char *f_code_ = f_code.c_str();
   
   int success;
   char info_log[512];
   unsigned int v_shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(v_shader, 1, &v_code_, NULL);
   glCompileShader(v_shader);
   glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(v_shader, 512, NULL, info_log);
      std::cout << "Couldn't compile the vertex shader"
                << info_log
                << std::endl;
   }

   unsigned int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(f_shader, 1, &f_code_, NULL);
   glCompileShader(f_shader);
   glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(f_shader, 512, NULL, info_log);
      std::cout << "Couldn't compile the vertex shader"
                << info_log
                << std::endl;
   }
    
   this->_id = glCreateProgram();
   glAttachShader(_id, v_shader);
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
}

void Shader::use() {
   glUseProgram(_id);
}

               
