#include "KDTree.h"



KDTree::KDTree()
{
}


KDTree::~KDTree()
{
	if(root!=nullptr)
		root->Clear();
}

//Returns a newly allocated node based on parameters
Node * KDTree::GenNode(glm::vec2 point, Node *&parent)
{
	Node* newNode = new Node;

	newNode->point = point;
	newNode->left = newNode->right = nullptr;
	newNode->parent = parent;
	return newNode;
}


//Overloaded Insert method
//
void KDTree::Insert(glm::vec2 point, std::vector<glm::vec2>& linePoints)
{
	level = Insert(root, point, 0);
	
	
	AddLinePoints(linePoints);

	
}

//Overloaded Insert method
//Recursive method to insert a point into our KDtree
//Returns depth of added node
int KDTree::Insert(Node *&node, glm::vec2 point, int depth)
{
	//If this node is empty, fill it with the added point
	if (node == nullptr )
	{	
		node = GenNode(point, prevNode);
		prevNode = node;
		return depth;
	}

	//Node axis alignment based on depth
	int axis = depth % 2;

	//If inserted point is left/down compared to current node point
	if (point[axis] < node->point[axis])
	{
		prevNode = node;
		Insert(node->left, point, depth + 1);
	}
	//If inserted point is right/up compared to current node point
	else
	{
		prevNode = node;
		Insert(node->right, point, depth + 1);
	}

	
}

#pragma region Tree_Rendering
//Adds two points of a line segment to be the render list
void KDTree::AddLinePoints(std::vector<glm::vec2>& linePoints)
{
	glm::vec2 point = prevNode->point;
	glm::vec2 parentPoint;
	float axisValue;
	if (level == 0)
	{
		linePoint1 = glm::vec2(point.x, 1);
		linePoint2 = glm::vec2(point.x, -1);
	}
	else
	{
		//In this case prevnode == current node 
		parentPoint = prevNode->parent->point;
		if (level % 2 == 0)
		{
			linePoint1 = glm::vec2(point.x, parentPoint.y);

			if (point.y > parentPoint.y)
				axisValue = ClosestSegment(point, linePoints, 1);
			else
				axisValue = ClosestSegment(point, linePoints, -1);

			linePoint2 = glm::vec2(point.x, axisValue);
		}
		else
		{
			linePoint1 = glm::vec2(parentPoint.x, point.y);

			if (point.x > parentPoint.x)
				axisValue = ClosestSegment(point, linePoints, 1);
			else
				axisValue = ClosestSegment(point, linePoints, -1);

			linePoint2 = glm::vec2(axisValue, point.y);
		}
	}



	linePoints.push_back(linePoint1);
	linePoints.push_back(linePoint2);


}


//Purely for rendering the tree
//Finds closest segment in the same direction as the parameter
//Glitches out occasionaly, feel free to improve
GLfloat KDTree::ClosestSegment(glm::vec2 point, std::vector<glm::vec2> linePoints, int direction)
{
	int axis = (level - 1) % 2;
	GLfloat closestAxis = direction;
	glm::vec2 temp;
	glm::vec2 lp1;
	glm::vec2 lp2;
	float minDist = 100;
	float dist;

	for (int i = 0; i < linePoints.size(); i += 2)
	{
		lp1 = linePoints[i];
		lp2 = linePoints[i + 1];

		//if the two axis points are not the same, go to the next pair
		if (lp2[axis] != lp1[axis])
			continue;

		//Check if the line segment would interesect our line
		if (axis == 0)
		{
			if (!((glm::max(lp2.y, lp1.y) > point.y && (glm::min(lp2.y, lp1.y) < point.y))))
				continue;
		}
		else if (axis == 1)
		{
			if (!((glm::max(lp2.x, lp1.x) > point.x && (glm::min(lp2.x, lp1.x) < point.x))))
				continue;
		}
		//Create temp point on the same axis as the point
		if (axis == 0)
			temp = glm::vec2(lp1.x, point.y);
		else
			temp = glm::vec2(point.x, lp1.y);

		//Calc distance between the 2 points on this axis
		dist = temp[axis] - point[axis];

		//If dist is negative when direction is positive, go to the next pair
		if (dist < 0)
		{
			if (direction == 1)
				continue;

			dist *= -1;
		}
		//If dist is positive when direction is negative, go to the next pair
		else if (dist > 0)
		{
			if (direction == -1)
				continue;
		}

		if (dist <= minDist)
		{
			minDist = dist;
			closestAxis = temp[axis];
		}

	}

	return closestAxis;
}
#pragma endregion





