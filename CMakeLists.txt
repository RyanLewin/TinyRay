CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(TinyRay)

FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp -std=gnu++0x")

SET(SRC_FILES
	Box.cpp
	Triangle.cpp
	Camera.cpp
	Material.cpp
	Ray.cpp
	Vector3.cpp
	Light.cpp
	Plane.cpp
	RayTracer.cpp
	Sphere.cpp
	Scene.cpp
	#ImageIO.cpp
	perlin.cpp
	Framebuffer.cpp
	)

INCLUDE_DIRECTORIES( 
	${GLUT_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	)
LINK_DIRECTORIES(
	/opt/local/lib
	)

ADD_EXECUTABLE(tinyray main.cpp 
	${SRC_FILES}
	)

TARGET_LINK_LIBRARIES(tinyray
	${GLUT_LIBRARY}
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	#glut
	)
