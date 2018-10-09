LINKERS := -ldl -lglfw3 -lm -lX11 -lGL -lGLU -lpthread
INCLUDE_DIRS := include 

render_window: render_window.cpp
	@g++ render_window.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/render_window
	@echo "Built rendering window"

render_triangle: render_triangle.cpp
	@g++ render_triangle.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/render_triangle
	@echo "Built rendering single triangle"

render_rectangle: render_rectangle.cpp
	@g++ render_rectangle.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/render_rectangle
	@echo "Built rendering rectangle"

shaders: shaders.cpp
	@g++ shaders.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/shaders
	@echo "Built rendering shaders"

more_attributes: more_attributes.cpp
	@g++ more_attributes.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/more_attributes
	@echo "Built more attributes"

shader_class: triangle_using_shader.cpp
	@g++ triangle_using_shader.cpp src/glad.c shader.cpp -I ${INCLUDE_DIRS} ${LINKERS} -o bin/shader_class
	@echo "Built rendering triangle using shader class"

texture: texture.cpp
	@g++ texture.cpp src/glad.c shader.cpp -I ${INCLUDE_DIRS} ${LINKERS} -o bin/texture
	@echo "Built texture generator"

texture_units: texture_units.cpp
	@g++ texture_units.cpp src/glad.c shader.cpp -I ${INCLUDE_DIRS} ${LINKERS} -o bin/texture_units
	@echo "Built texture units generator"

transformations: transformations.cpp
	@g++ transformations.cpp src/glad.c shader.cpp -pedantic -I ${INCLUDE_DIRS} ${LINKERS} -o bin/transformations
	@echo "Built tranformations renderer"

going_3d: going_3d.cpp
	@g++ going_3d.cpp src/glad.c shader.cpp -pedantic -I ${INCLUDE_DIRS} ${LINKERS} -o bin/going_3d
	@echo "Built going 3d transformations!"

more_3d: more_3d.cpp
	@g++ more_3d.cpp src/glad.c shader.cpp -pedantic -I ${INCLUDE_DIRS} ${LINKERS} -o bin/more_3d
	@echo "Built more 3d transformations!"

camera: camera.cpp
	@g++ camera.cpp src/glad.c shader.cpp -pedantic -I ${INCLUDE_DIRS} ${LINKERS} -o bin/camera
	@echo "Built camera transformations!"

camera2: camera2.cpp
	@g++ camera2.cpp src/glad.c shader.cpp -pedantic -I ${INCLUDE_DIRS} ${LINKERS} -o bin/camera2
	@echo "Built camera dynamics!"

camera3: camera3.cpp
	@g++ camera3.cpp src/glad.c shader.cpp -pedantic -I ${INCLUDE_DIRS} ${LINKERS} -o bin/camera3
	@echo "Built camera mouse dynamics!"

colors: colors.cpp
	@g++ colors.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/colors
	@echo "Built colors rendering!"

colors2: colors2.cpp
	@g++ colors2.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/colors2
	@echo "Built simulating different lighting conditions!"

rotate_light_source: rotate_light_source.cpp
	@g++ rotate_light_source.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/rotate_light_source
	@echo "Built simulating rotation of light source"

ex1: ex/ex1.cpp
	@g++ ex/ex1.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/ex1
	@echo "Built ex 1"

ex2: ex/ex2.cpp
	@g++ ex/ex2.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/ex2
	@echo "Built ex 2"

ex3: ex/ex3.cpp
	@g++ ex/ex3.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/ex3
	@echo "Built ex 3"


clean:
	@echo "Cleaning..."
	@rm -rf ./bin/*

all: render_window \
	  render_triangle \
	  render_rectangle \
	  ex1 \
	  ex2 \
	  ex3 \
	  shaders \
	  more_attributes \
	  shader_class \
	  texture \
	  texture_units \
	  going_3d \
	  more_3d \
	  camera
