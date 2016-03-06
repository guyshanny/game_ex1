#include "Tree.h"
#include "ShaderIO.h"
#include <stack>
#include <iostream>

#define ANGLE 25.0f

void Tree::draw(const mat4& projection, const mat4& view)
{
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);

	glDrawArrays(GL_LINES, 0, _numOfVertices);

	// Unbind the Vertex Array object
	glBindVertexArray(0);
	glUseProgram(0);
}

std::string createString(int numIterations, std::string base, std::vector<std::string> rules)
{
	std::string str = base;
	size_t curBranch = 0;
	size_t rule = 0;
	for (int i = 0; i < numIterations; i++)
	{
		curBranch = 0;
		while ((curBranch = str.find(base, curBranch)) != std::string::npos)
		{
			curBranch = str.find(base, curBranch);
			if (rules.size() > 1)
			{
				rule = rand() % rules.size();
			}
			str.replace(curBranch, base.length(), rules.at(rule));
			curBranch += rules.at(rule).length();
		}
	}
	return str;
}

void Tree::createVertices(std::vector<glm::vec4>& vertices, int numIterations, std::string base, std::vector<std::string> rules)
{
	std::string str = createString(numIterations, base, rules);
	//std::cout << str << std::endl;
	std::stack<vec3> posStack;
	std::stack<vec3> dirStack;
	vec3 curPos = _position;
	vec3 curDir = _direction;
	mat4 eye;
	
	vertices.push_back(glm::vec4(curPos, 1.f));
	for (int i = 0; i < str.length(); i ++)
	{
		if (str.at(i) == base.at(0))
		{
			curPos = curPos + curDir;
			vertices.push_back(glm::vec4(curPos, 1.f));
		}
		else if (str.at(i) == '[')
		{
			posStack.push(curPos);
			dirStack.push(curDir);
			vertices.push_back(glm::vec4(curPos, 1.f));
		}
		else if (str.at(i) == ']')
		{
			curPos = posStack.top();
			posStack.pop();
			curDir = dirStack.top();
			dirStack.pop();
		}
		else if (str.at(i) == '&')
		{
			//pitch
			mat4 rot = rotate(eye, radians(ANGLE), vec3(0.f, 0.f, 1.f));
			curDir = vec3(rot * vec4(curDir, 1.f));
		}
		else if (str.at(i) == '^')
		{
			//pitch
			mat4 rot = rotate(eye, -radians(ANGLE), vec3(0.f, 0.f, 1.f));
			curDir = vec3(rot * vec4(curDir, 1.f));
		}
		else if (str.at(i) == '\\')
		{
			//roll
			mat4 rot = rotate(eye, radians(ANGLE), vec3(1.f, 0.f, 0.f));
			curDir = vec3(rot * vec4(curDir, 1.f));

		}
		else if (str.at(i) == '/')
		{
			//roll
			mat4 rot = rotate(eye, -radians(ANGLE), vec3(1.f, 0.f, 0.f));
			curDir = vec3(rot * vec4(curDir, 1.f));
		}
		else if (str.at(i) == '+')
		{
			//yaw
			mat4 rot = rotate(eye, radians(ANGLE), vec3(0.f, 1.f, 0.f));
			curDir = vec3(rot * vec4(curDir, 1.f));
		}
		else if (str.at(i) == '-')
		{
			//yaw
			mat4 rot = rotate(eye, -radians(ANGLE), vec3(0.f, 1.f, 0.f));
			curDir = vec3(rot * vec4(curDir, 1.f));
		}
	}
}

void Tree::init()
{
	GLuint program = programManager::sharedInstance().programWithID("default");

	std::vector<glm::vec4> vertices;
	std::vector<std::string> rules;
	rules.push_back("F[&F][&/F][&\\F]");
	rules.push_back("F[^F][^+F][^-F]");
	rules.push_back("F[^\\F][^/F][&-F]");
	createVertices(vertices, 4, "F", rules);
	_numOfVertices = vertices.size();

	// Create and bind the object's Vertex Array Object:
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Create and load vertex data into a Vertex Buffer Object:
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _numOfVertices, &vertices[0], GL_STATIC_DRAW);

	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	// Obtain attribute handles:
	GLint _posAttrib;
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
