#include "Node.h"

Node::Node(vector<double>& point) {
    this->point = point;
    left = nullptr;
    right = nullptr;
}

Node::~Node() {}

vector<double> Node::getPoint() {
    return point;
}

void Node::setPoint(vector<double> point) {
    this->point = point;
}

Node* Node::getLeftNode() {
    return left;
}

void Node::setLeftNode(Node* left) {
    this->left = left;
}

Node* Node::getRightNode() {
    return right;
}

void Node::setRightNode(Node* right) {
    this->right = right;
}

bool Node::isLeaf() {
    return getLeftNode() == nullptr && getRightNode() == nullptr;
}
