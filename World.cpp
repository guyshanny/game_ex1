//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "World.h"
#include "Tree.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define SHADERS_DIR "shaders/"

#define _USE_MATH_DEFINES
#include <math.h>


World::World() :
    _vao(0), _vbo(0), _ibo(0), _yAngle(0.0f), _movingSpeed(0.1f), _world(1.0f)
{
	_projection = perspective(5.0f, 1.0f, 0.1f, 100.0f);
}

World::~World()
{
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);

	for (Object* object : _objects)
	{
		delete object;
	}
	_objects.clear();
} 

#pragma region Init
void World::init()
{
	programManager::sharedInstance()
		.createProgram("default",
			SHADERS_DIR "SimpleShader.vert",
			SHADERS_DIR "SimpleShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");

	_camera = Camera::instance();
	_camera->init((float)M_PI, _gridSize / 2.0);
	_createSceneObjects();

	// Obtain uniform variable handles:
	_fillColorUV = glGetUniformLocation(program, "fillColor");
	_gpuWVP = glGetUniformLocation(program, "wvp");

	// Initialize vertices buffer and transfer it to OpenGL
	{
		_createTrianglesGrid(_gridVertices, _gridIndices);

		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _gridVertices.size(), &_gridVertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * _gridIndices.size(), &_gridIndices[0], GL_STATIC_DRAW);

		// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
		// Obtain attribute handles:
		_posAttrib = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(_posAttrib);
		glVertexAttribPointer(_posAttrib, // attribute handle
			4,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE,
			0,
			0);

		// Unbind vertex array:
		glBindVertexArray(0);
	}

	// Drawing scene objects
	for (Object* object : _objects)
	{
		object->init();
	}
}

void World::_createTrianglesGrid(std::vector<glm::vec4>& vertices, std::vector<GLubyte>& indices)
{
	const size_t iCols = _gridSize / _triangleResolution;
	const size_t iRows = _gridSize / _triangleResolution;
	const size_t halfGridCols = _gridSize / 2;
	const size_t halfGridRows = _gridSize / 2;

	// Creating vertices
	for (size_t r = 0; r <= iRows; r++)
	{
		for (size_t c = 0; c <= iCols; c++)
		{
			//vertices.push_back(glm::vec4(c * _triangleResolution, r * _triangleResolution, 0.0f, 1.0f));
			float x = ((float(c) / (iCols - 1)) * 2 - 1) * halfGridCols;
			float y = ((float(r) / (iRows - 1)) * 2 - 1) * halfGridRows;
			vertices.push_back(glm::vec4(x, 0.0f, y, 1.0f));
		}
	}

	// Creating indices
	for (size_t r = 0; r < iRows; r++)
	{
		for (size_t c = 0; c < iCols; c++)
		{
			GLint index0 = r*(iCols + 1) + c;
			GLint index1 = index0 + 1;
			GLint index3 = index0 + (iCols + 1);
			GLint index4 = index3 + 1;
			if ((r + c) % 2 == 0)	// even
			{
				indices.push_back(index0);
				indices.push_back(index1);
				indices.push_back(index4);
				indices.push_back(index0);
				indices.push_back(index3);
				indices.push_back(index4);
			}
			else					// Odd
			{
				indices.push_back(index0);
				indices.push_back(index3);
				indices.push_back(index1);
				indices.push_back(index1);
				indices.push_back(index3);
				indices.push_back(index4);
			}
		}
	}
}

void World::_createSceneObjects()
{
	_objects.push_back((Object*)(new Tree(vec3(0.f), vec3(0.f, -1.f, 0.f))));
	_objects.push_back((Object*)(new Tree(vec3(5.f, 0.f, 5.f), vec3(0.f, -1.f, 0.f))));
	_objects.push_back((Object*)(new Tree(vec3(-7.f, 0.f, -10.f), vec3(0.f, -1.f, 0.f))));
	_objects.push_back((Object*)(new Tree(vec3(-8.f, 0.f, 9.f), vec3(0.f, -1.f, 0.f))));
}
#pragma endregion

#pragma region Draw
void World::draw()
{
	_camera->update();
	mat4 view = _camera->getViewMatrix();
	// Drawing the world
	_drawWorld(view);

	// Drawing scene objects
	for (Object* object : _objects)
	{
		object->update();
		object->draw(_projection, view);
	}
}

void World::_drawWorld(const mat4& view)
{
	// Set the program to be used in subsequent lines:
	glUseProgram(programManager::sharedInstance().programWithID("default"));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_FILL/GL_POINT/GL_LINE

	mat4 wvp = _projection * view * _world;

	// pass the wvp to vertex shader
	glUniformMatrix4fv(_gpuWVP, 1, GL_FALSE, value_ptr(wvp));

	// pass the model color to fragment shader   
	glUniform4f(_fillColorUV, 0.3f, 0.5f, 0.3f, 1.0);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);

	glDrawElements(GL_TRIANGLES, _gridIndices.size(), GL_UNSIGNED_BYTE, (GLvoid*)0);

	// Unbind the Vertex Array object
	glBindVertexArray(0);

	// Cleanup, not strictly necessary
	glUseProgram(0);
}
#pragma endregion

#pragma region KeysHandling
void World::moveForwardKeyPressed()
{
	_camera->moveForward();
}

void World::moveBackwardKeyPressed()
{
	_camera->moveBackward();
}

void World::turnLeftKeyPressed()
{
	_camera->turnLeft();
}

void World::turnRightKeyPressed()
{
	_camera->turnRight();
}

void World::strafeRightKeyPressed()
{
	_camera->strafeRight();
}

void World::strafeLeftKeyPressed()
{
	_camera->strafeLeft();
}

void World::resetKeyPressed()
{
	_world = mat4(1.0f);
	_projection = perspective(5.0f, 1.0f, 0.1f, 100.0f);
	_camera->reset();
}

void World::jumpKeyPressed(const bool& isJump)
{
	_camera->jump(isJump);
}

void World::zoomKeyPressed(const bool & isZoomIn)
{
	_camera->zoom(isZoomIn);
}

void World::teleportKeyPressed()
{
	_camera->teleport();
}

void World::shiftKeyPressed(const bool& isShiftPressed)
{
	_camera->fastMove(isShiftPressed);
}

void World::crouchKeyPressed(const bool& isCrouch)
{
	_camera->crouch(isCrouch);
}
#pragma endregion

void World::resize(int width, int height)
{
	_width = (float)width;
	_height = (float)height;
	_offsetX = 0;
	_offsetY = 0;
}
