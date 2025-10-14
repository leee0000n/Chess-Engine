#pragma once
class Tree
{
public:
	
	Tree() : m_val(0), m_left(nullptr), m_right(nullptr) {}
	Tree(int val) : m_val(val), m_left(nullptr), m_right(nullptr) {}
	Tree(int val, Tree* left, Tree* right) : m_val(val), m_left(left), m_right(right) {}

	int getValue();
	Tree* getLeft();
	Tree* getRight();

	void setValue(int val);
	void setLeft(Tree* left);
	void setRight(Tree* right);

	void addNewNode(Tree* node);
private:
	int m_val;

	Tree* m_left;
	Tree* m_right;
};

