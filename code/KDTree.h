#ifndef KDTREE_H__ 
#define KDTREE_H__ //check for dup declarations

#include "./Node.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>
#include <limits>
#include <string>
#include <stdexcept>

using namespace std;

class KDTree {
public:
    /**
     * @brief Constructs an empty KDTree for points with the given number of dimensions.
     * 
     * @param dimensions (unsigned int) The number of dimensions for each point stored in the tree. 
     * Must be greater than zero.
     */
    KDTree(unsigned int dimensions);
    ~KDTree();

    /**
     * Bounds struct acts as a 2D, 3D container for a plane or cube's min and max x,y,z coordinates 
     */
    struct Bounds {
        double minX, minY, minZ, maxX, maxY, maxZ;
    };

    /**
     * @brief returns the root of the KDTree
     */
    Node *getRoot();

    /**
     * @brief sets the root of the KDTree
     */
    void setRoot(Node *node);

    /**
     * @brief get number of dimensions of KDTree
     */
    int getDimensions();

    /**
     * @brief inserts a node into the KDTree. The node is inserted recursively through recurseInsertion.
     * When traversing the tree a node will be inserted on the left or right of a node given it is less than or
     * greater than the dimension of the depth of the tree. 
     * 
     * @param point (vector<double>) point to insert into the kdtree
     * @return Node* root of the kdtree 
     */
    Node *insertNode(vector<double> point);

    /**
     * @brief removes a node from the KDTree. The node is removed by first identifying the node to remove, then
     * identifying the min value node in the branch and replacing the to remove node with the min node. Then
     * recurse down the right and left side setting the right node of the replaced node.
     * 
     * @param point (vector<double>) point to remove from the kdtree
     * @return Node* root of the kdtree
     */
    Node *removeNode(vector<double> point);

    /**
     * @brief get node from kdtree by traversing the tree and comparing the dimension of each level
     * 
     * @param point (vector<double>) point to find in tree
     * @return Node* found node or nullptr if not found 
     */
    Node *getNode(vector<double> point);

    /**
     * @brief determines the nearest node of a given target. This is done by traversing the kdtree 
     * finding the target through recursion then moving back up the kdtree as the recursive calls are
     * removed from the stack. Each time it encounters a node we check the distance from the node
     * to the target if its the smallest distance we've seen store that. If the node has another branch
     * we also need need to check it in case the closer value is in that branch.
     * 
     * @param target (Node*) the target node that we are determining the nearest neighbor for
     * @return Node* nearest neighbor by distance of the target
     */
    Node *nearestNeighborSearch(Node *target);

    /**
     * @brief find a range of points that are within a specified plane or cube. With a given point of
     * origin and height, width, and length (can be null) create a 2D plane or 3D. Traverse the entire tree,
     * as we traverse check if the point is within the plane/cube by comparing the min and max values of the
     * plane/cube. If the point lies within the object then it is in the range 
     * 
     * 
     * @param pointOfOrigin (vector<double>) origin of the plane/cube
     * @param height (double) height of the plane/cube
     * @param width (double) width of the plane/cube
     * @param length (double) length of the cube
     * @return vector<Node*> list of nodes within the plane/cube specified
     */
    vector<Node *> rangeSearch(vector<double> pointOfOrigin, double height, double width, double length);

    /**
     * @brief find the node with the minimum value given the dimension/axis specified from the root
     * 
     * @param axis (unsigned int) axis/dimension 
     * @return Node* minimum node for a given dimension
     */ 
    Node *findMinimumAxisValueFromNode(Node *node, unsigned int axis);

    void printKDTree(Node *node, string prefix, bool isLeft);

private:
    Node *root;
    unsigned int k;

    /**
     * @brief determines the distance between two points using squared distance rather than root distance
     * for 
     * 
     * @param a a point in the kdtree
     * @param b a point in the kdtree
     * @return double distance between two points
     */ 
    double squaredDistance(vector<double> a, vector<double> b);

    /**
     * @brief given a pointOfOrigin, create either a plane or cube given the number of coords
     * 
     * @param pointOfOrigin point of origin for the plane/cube
     * @param width width of the plane/cube
     * @param height height of the plane/cube
     * @param length length of the plane/cube
     * @return Bounds struct mins/max of object
     */
    Bounds makeRange(vector<double> pointOfOrigin, double width, double height, double length);
    Node *recurseInsertion(Node *node, vector<double> point, unsigned int depth);
    Node *recurseGetNode(Node *node, vector<double> point, unsigned int depth);
    Node *recurseFindMinimum(Node *node, unsigned int axis, unsigned int depth);
    Node *recurseRemoveNode(Node *node, vector<double> point, unsigned int depth);
    void recurseNN(Node *node, Node *target, Node *&currentBest, double &currentBestDist, unsigned int depth);
    void recurseGetNodesInRange(Node *node, Bounds b, vector<Node *> &nodesInRange);
    void addNodeToInRangeList(Node *node, Bounds b, vector<Node *> &nodesInRange);
    void printPoint(Node *node);
};

#endif