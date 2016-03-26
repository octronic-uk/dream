/*
* Node.cpp
*/
#include <iostream>
#include <cmath>
#include "Node.h"

namespace DreamScenegraph {
	Node::Node(std::string name) {
		mName = name;
		mParentIndex = -1;
		mVertexBufferIndex = -1;

		mTranslation[NODE_X] = 0.0f;
		mTranslation[NODE_Y] = 0.0f;
		mTranslation[NODE_Z] = 0.0f;

		mRotation[NODE_X] =  0.0f;
		mRotation[NODE_Y] =  0.0f;
		mRotation[NODE_Z] =  0.0f;
		return;
	}

	Node::~Node() {

	}

	int Node::hasValidPath() {
		return mPath.size() > 0;
	}

	int Node::hasValidName() {
		return mName.size() > 0;
	}

	void Node::print() {
		std::cout << mPath << std::endl;
		return;
	}

	void Node::setParentIndex(int parent) {
		mParentIndex = parent;
		return;
	}

	void Node::setTranslation(float x, float y, float z) {
		mTranslation[NODE_X] = x;
		mTranslation[NODE_Y] = y;
		mTranslation[NODE_Z] = z;
		return;
	}

	void Node::setRotation(float x, float y, float z) {
		mRotation[NODE_X] = x;
		mRotation[NODE_Y] = y;
		mRotation[NODE_Z] = z;
		return;
	}

	int Node::hasVertexBuffer() {
		return mVertexBufferIndex > -1;
	}

	void Node::setPolarTranslation(float radius, float theta) {
		return;
	}
}
