#ifndef __TREE_H
#define __TREE_H

#include "Object.h"
#include "Globals.h"
#include <vector>


class Tree : Object
{
public:
	enum TreeKind
	{
		DEFAULT,
	};

private:
	enum L_SYSTEM
	{
		MOVE_FORWARD,
		ROTATE_ANTI_CLOCKWISE,
		ROTATE_CLOCKWISE,
	};

	const vec3 _treeBoundaries = vec3(WINDOW_BOUNDARY.x - 5.0f, WINDOW_BOUNDARY.y, WINDOW_BOUNDARY.z - 5.0f);
	vec3 _startPoint;
	const float _branchLengthDecreaseFactor;
	const float _childBranchAngle;				// Degree
	TreeKind _treeKind;

	std::vector<vec3> _vertices;
	
	
	void _buildTree();
	void _generateTreeBranches(const vec3& currentPosition, float length, float rotation, std::string lsystem);

public:
	Tree();
	Tree(const vec3& startPoint, 
		 const float& branchLengthDecreaseFactor, 
		 const float& childBranchAngle, 
		 const TreeKind& treeKind);

	virtual void draw(const mat4& projection, const mat4& view);

};

#endif
