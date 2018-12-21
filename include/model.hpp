#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <shader.hpp>
#include <mesh.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>                

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


unsigned int texture_from_file(
   const char *path,
   const std::string &directory,
   bool gamma = false
);

class Model {
private:
   std::vector<Mesh> meshes;
   std::string directory;
   std::vector<texture> textures_loaded;
   
   void load_model(std::string path);
   void pprint();
   void process_node(aiNode *node, const aiScene *scene);
   Mesh process_mesh(aiMesh *mesh, const aiScene *Scene);
   std::vector<texture> load_material_textures(
      aiMaterial *material,
      aiTextureType type,
      std::string type_name
   );

public:
   Model(const char *path) {
      load_model(path);
      pprint();
   }
   void draw(Shader shader) {
      for (int i = 0; i < meshes.size(); ++i) {
         meshes[i].draw(shader);
      }
   }
   
};

void Model::pprint() {
      std::cout << "Total number of meshes: "
                << meshes.size()
                << std::endl;
      for (int i = 0; i < meshes.size(); ++i)
            meshes[i].pprint(i+1);
}

void Model::load_model(std::string path) {
   Assimp::Importer importer;
   const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "Error Assimp"
                << importer.GetErrorString()
                << std::endl;
      return;
   }
   directory = path.substr(0, path.find_last_of('/'));
   
   process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode *node, const aiScene *scene) {
   // process all the current node's meshes (if any)
   for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(process_mesh(mesh, scene));
   }

   // process all the children of the current node
   for (int i = 0; i < node->mNumChildren; ++i) {
      process_node(node->mChildren[i], scene);
   }
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene) {
   std::vector<vertex> vertices;
   std::vector<unsigned int> indices;
   std::vector<texture> textures;

   // process all the vertices of the mesh
   for (int i = 0; i < mesh->mNumVertices; ++i) {
      vertex v;

      // Extract positions
      glm::vec3 pos;
      pos.x = mesh->mVertices[i].x;
      pos.y = mesh->mVertices[i].y;
      pos.z = mesh->mVertices[i].z;
      v.position = pos;

      // Extract normals
      glm::vec3 norm;
      norm.x = mesh->mNormals[i].x;
      norm.y = mesh->mNormals[i].y;
      norm.z = mesh->mNormals[i].z;
      v.normal = norm;

      // Extract texture positions
      glm::vec2 tex_pos;
      if (mesh->mTextureCoords[0]) {
         tex_pos.x = mesh->mTextureCoords[0][i].x;
         tex_pos.y = mesh->mTextureCoords[0][i].y;
      } else {
         tex_pos = glm::vec2(0.0f);   
      }
      v.tex_pos = tex_pos;
      vertices.push_back(v);
   }

   // process indices
   for (int i = 0; i < mesh->mNumFaces; ++i) {
      aiFace face = mesh->mFaces[i];
      for (int j = 0; j < face.mNumIndices; ++j)
         indices.push_back(face.mIndices[j]);
   }

   // process textures (diffuse and specular)
   if (mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

      // load diffuse maps
      std::vector<texture> diffuse_maps = load_material_textures(
         material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

      // load specular maps
      std::vector<texture> specular_maps = load_material_textures(
         material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
   }

   return Mesh(vertices, textures, indices);
}

std::vector<texture> Model::load_material_textures(
   aiMaterial *material,
   aiTextureType type,
   std::string type_name) {

   std::vector<texture> textures;
   for (int i = 0; i < material->GetTextureCount(type); ++i) {
      aiString str;
      material->GetTexture(type, i , &str);
      bool skip = false;
      for (int j = 0; j < textures_loaded.size(); ++j) {
         if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
            textures.push_back(textures_loaded[j]);
            skip = true;
            break;
         }
      }

      if (!skip) {
         texture t;
         t.id = texture_from_file(str.C_Str(), directory);
         t.type = type_name;
         t.path = str.C_Str();
         textures.push_back(t);
         textures_loaded.push_back(t);
      }
   }

   return textures;
}

unsigned int texture_from_file(
   const char *path,
   const std::string &directory,
   bool gamma) {
   
   std::string file_name = std::string(path);
   file_name = directory + '/' + file_name;
   
   unsigned int texture_id;
   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, n_channels;      
   unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &n_channels, 0);
   if (data) {
      GLenum format;
      if (n_channels == 1)
         format = GL_RED;
      else if (n_channels == 3)
         format = GL_RGB;
      else if (n_channels == 4)
         format = GL_RGBA;

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   } else {
      std::cout << "Couldn't load the texture image!"
                << std::endl;
   }
   stbi_image_free(data);
  
   return texture_id;
}

#endif

