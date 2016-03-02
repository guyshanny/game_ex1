//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr
#include <vector>
#include "Camera.h"

using namespace glm;

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

class Model {
	
    GLuint _vao, _vbo, _ibo;

    // Attribute handle:
    GLint _posAttrib;
	
    // Uniform handle:
    GLint _fillColorUV, _gpuWVP;
    
    // View port frame:
    float _width, _height, _offsetX, _offsetY;
	
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
	glm::vec3 _cameraPosition;
	glm::vec3 _cameraDirection;
	float _yAngle;
	float _movingSpeed;

	void _createTrianglesGrid(std::vector<glm::vec4>& vertices, std::vector<GLubyte>& indices);

 public:
    Model();
    virtual ~Model();
    void init();
    void draw();
    void resize(int width, int height);	

	// Keys handling
	void moveForward() { _camera->moveForward(); }
	void moveBackward() { _camera->moveBackward(); }
	void turnLeft() { _camera->turnLeft(); }
	void turnRight() { _camera->turnRight(); }
};

#endif /* defined(__ex0__Model__) */
