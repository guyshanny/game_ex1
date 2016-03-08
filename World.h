//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __WORLD__
#define __WORLD__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr
#include <vector>
#include "Camera.h"
#include "Object.h"

using namespace glm;

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

class World {
	
    GLuint _vao, _vbo, _ibo;

    // Attribute handle:
    GLint _posAttrib;
	
    // Uniform handle:
    GLint _fillColorUV, _gpuWVP;
    
    // View port frame:
    float _width, _height, _offsetX, _offsetY;
	
	// WVP matrices
	mat4 _projection;
	mat4 _world;

	// Mesh grid
	static const size_t _gridSize = 30;
	static const size_t _triangleResolution = 2;
	std::vector<glm::vec4> _gridVertices;
	std::vector<GLubyte> _gridIndices;

	// Zoom
	bool _isZoomActive;
	float _zoomBegin;
	float _zoomAddition;
	float _zoomFactor;

	// Camera variables
	Camera* _camera;
	float _yAngle;
	float _movingSpeed;

	void _createTrianglesGrid(std::vector<glm::vec4>& vertices, std::vector<GLubyte>& indices);

	// Scene objects
	std::vector<Object*> _objects;
	void _createSceneObjects();

	void _drawWorld(const mat4& view);


 public:
    World();
    virtual ~World();
    void init();
    void draw();
    void resize(int width, int height);	

	// Keys handling
	void moveForwardKeyPressed();
	void moveBackwardKeyPressed();
	void turnLeftKeyPressed();
	void turnRightKeyPressed();
	void strafeRightKeyPressed();
	void strafeLeftKeyPressed();
	void crouchKeyPressed(const bool& isCrouch);
	void resetKeyPressed();
	void jumpKeyPressed(const bool& isJump);
	void zoomKeyPressed(const bool& isZoomIn);
	void teleportKeyPressed();
	void shiftKeyPressed(const bool& isShiftPressed);
};

#endif /* defined(__ex0__Model__) */
