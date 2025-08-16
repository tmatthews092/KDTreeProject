#ifndef NODE_H__ 
#define NODE_H__ //check for dup declarations

#include <vector>

using namespace std;

class Node {
    private:
        Node* left;
        Node* right;
        int axis;
        vector<double> point;
    public:
        Node(vector<double>& point);
        ~Node(); //destructor
        void setLeftNode(Node* left);
        Node* getLeftNode();
        void setRightNode(Node* right);
        Node* getRightNode();
        void setPoint(vector<double> point);
        vector<double> getPoint();
        bool isLeaf();
};

#endif
