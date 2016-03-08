#ifndef __TREE_H
#define __TREE_H

#include <string>
#include <vector>

#include "Object.h"

#pragma once
class Tree : Object
{
private:
	int _numOfVertices;
	const std::string _branch = "F";
	static const std::string rules[];
	void createVertices(std::vector<glm::vec4>& vertices, int numIterations, std::string base, std::vector<std::string> rules);
public:
	Tree(const vec3 & pos, const vec3 & dir) : Object(pos, dir, mat4(0.f), 0, 0) {}
	~Tree() {}

	virtual void init();
	virtual void update() {}
	virtual void draw(const mat4& projection, const mat4& view);

	
	
};

#endif