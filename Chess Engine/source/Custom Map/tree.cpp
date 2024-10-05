#include "tree.h"

int Tree::getValue() { return m_val; }
Tree* Tree::getLeft() { return m_left; }
Tree* Tree::getRight() { return m_right; }

void Tree::setValue(int val) {
	m_val = val;
}

void Tree::setLeft(Tree* left) {
	if (m_left != nullptr) delete m_left;

	m_left = left;
}

void Tree::setRight(Tree* right) {
	if (m_right != nullptr) delete m_right;

	m_right = right;
}

void Tree::addNewNode(Tree* node) {
	// Check if node becomes left
	if (node->getValue() <= m_val) {
		// If left is empty, set left
		if (m_left == nullptr) {
			m_left = node;;
			return;
		}
	}
	// Else becomes right
	else {
		// If right is empty, set right
		if (m_right == nullptr) {
			m_right = node;
			return;
		}
	}

	addNewNode(node);
}