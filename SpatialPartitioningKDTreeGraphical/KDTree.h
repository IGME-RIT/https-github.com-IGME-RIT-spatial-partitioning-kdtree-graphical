#pragma once
#define GLEW_STATIC
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>


//A simple node struct that our tree is built with
struct Node {
	glm::vec2 point;
	Node *left = nullptr;
	Node *right = nullptr;
	Node *parent = nullptr;

	//Recursive clear;
	void Clear()
	{
		if(left!=nullptr)
			left->Clear();
		if (right != nullptr)
			right->Clear();

		delete this;
	}
};


class KDTree
{
	//Root node
	Node* root = nullptr;
	
	Node* prevNode;

	int level = 0;
	//Max depth
	int maxLevel = 10;
	glm::vec2 linePoint1;
	glm::vec2 linePoint2;
	
public:
	KDTree();
	~KDTree();

	GLfloat ClosestSegment(glm::vec2 point, std::vector<glm::vec2>linePoints, int direction);
	Node* GenNode(glm::vec2 point, Node *&parent);
	void Insert(glm::vec2 point, std::vector<glm::vec2> &linePoints);
	int Insert(Node *&node, glm::vec2 point, int depth);
	void AddLinePoints(std::vector<glm::vec2>& linePoint);
};

