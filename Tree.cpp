#include "Tree.h"

#include "ShaderIO.h"
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

const std::string _treeType[] = { "default" };
const char _L_SYSTEM_MAP[] = { 'F', '+', '-', '[', ']' };

Tree::Tree() : Tree(vec3(0.0f), 0.75f, 45.0f, TreeKind::DEFAULT) { }

Tree::Tree(const vec3 & startPosition, 
		   const float& branchLengthDecreaseFactor, 
		   const float& childBranchAngle, 
		   const TreeKind& treeKind) :
										Object(), 
										_startPoint(startPosition), 
										_branchLengthDecreaseFactor(branchLengthDecreaseFactor),
										_childBranchAngle(childBranchAngle),
										_treeKind(treeKind)
{
	_buildTree();
}

void Tree::_buildTree()
{
	_vertices.push_back(_startPoint);
	_generateTreeBranches(_startPoint, 1.0f, 90.0f, 0);
}

void Tree::_generateTreeBranches(const vec3 & currentPosition, float length, float rotation, std::string lsystem)
{
	if (NULL == lsystem.size())
	{
		return;
	}

	vec3 nextPosition;

	char move = lsystem.front();
	switch (move)
	{
		case L_SYSTEM::MOVE_FORWARD:
		{
			nextPosition = currentPosition + vec3(0.0f, -length, 0.0f);
			length = length * _branchLengthDecreaseFactor;
			_vertices.push_back(nextPosition);
		}
		break;

		case L_SYSTEM::ROTATE_ANTI_CLOCKWISE:
		{
			nextPosition.x = 
		}
		break;
	
		case L_SYSTEM::ROTATE_CLOCKWISE:
		{

		}
		break;

		default:
			break;
	}

	_generateTreeBranches(nextPosition, length, rotation, lsystem.erase(0, 1));
}


void Tree::draw(const mat4 & projection, const mat4 & view)
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


}
