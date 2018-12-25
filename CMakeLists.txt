cmake_minimum_required(VERSION 3.0)

project(GL)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(ASSIMP REQUIRED)

set(HEADERS "include")
include_directories(${HEADERS})

set(GLAD_SRC src/glad.c)
set(LIBS glfw ${ASSIMP_LIBRARIES})

function(builder src bin)
  add_executable(${bin} ${src} ${GLAD_SRC}) 
  target_link_libraries(${bin} ${LIBS})
endfunction()

# chapter 1
builder(01.getting_started/02.triangle/render_rectangle.cpp rectangle)
builder(01.getting_started/03.shaders/shaders.cpp shader)
builder(01.getting_started/04.textures/texture.cpp texture)
builder(01.getting_started/04.textures/texture_units.cpp texture_units)
builder(01.getting_started/05.transformations/transformations.cpp transform)
builder(01.getting_started/06.co-systems/more_3d.cpp more_3d)
builder(01.getting_started/07.camera/camera.cpp camera)
builder(01.getting_started/07.camera/camera2.cpp camera2)
builder(01.getting_started/07.camera/camera3.cpp camera3)

# chapter 2
builder(02.lighting/01.colors/colors.cpp colors1)
builder(02.lighting/01.colors/colors2.cpp colors2)
builder(02.lighting/04.lighting_maps/diffuse_maps.cpp diffuse)
builder(02.lighting/04.lighting_maps/specular_maps.cpp specular)
builder(02.lighting/05.light_casters/directional.cpp directional)
builder(02.lighting/05.light_casters/point_lights.cpp pointlight)
builder(02.lighting/05.light_casters/spotlight.cpp spotlight)
builder(02.lighting/06.multiple_lights/multiple_lights.cpp multiple)

# chapter 3
builder(03.models/nanosuit.cpp nanosuit)

# chapter 4
builder(04.advanced/01_depth_testing.cpp adv_depth)
builder(04.advanced/02_stencil_testing.cpp stencil)
builder(04.advanced/03_blending.cpp blend)
builder(04.advanced/04_face_culling.cpp cull)
builder(04.advanced/05_framebuffers.cpp buffer)
builder(04.advanced/06_cubemaps.cpp cube)
builder(04.advanced/08_advanced_glsl.cpp glsl)
builder(04.advanced/09_geometry_shader.cpp geo)
builder(04.advanced/09_explode.cpp explode)
builder(04.advanced/10_normals.cpp normals)
builder(04.advanced/11_instancing1.cpp instance1)
builder(04.advanced/11_instancing2.cpp instance2)
builder(04.advanced/11_instancing3.cpp instance3)
builder(04.advanced/11_instancing4.cpp instance4)
