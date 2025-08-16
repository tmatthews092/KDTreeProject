#include "KDTree.h"

KDTree::KDTree(unsigned int k) {
    if (k > 0) {
        this->k = k;
    } else {
        throw invalid_argument("Incorrect number of dimensions provided. Please enter a dimension greater than 0.");
    }
    root = nullptr;
}

KDTree::~KDTree() {}

Node* KDTree::recurseInsertion(Node* node, vector<double> point, unsigned int depth) {
    if (node == nullptr) {
        return new Node(point);
    }
    unsigned int d = depth % k;

    if (point[d] < node->getPoint()[d]) {
        node->setLeftNode(recurseInsertion(node->getLeftNode(), point, depth + 1));
    } else {
        node->setRightNode(recurseInsertion(node->getRightNode(), point, depth + 1));
    }

    return node;
}

Node *KDTree::recurseGetNode(Node *node, vector<double> point, unsigned int depth) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->getPoint() == point) {
        return node;
    }
    unsigned int d = depth % k;

    if (point[d] < node->getPoint()[d]) {
        return recurseGetNode(node->getLeftNode(), point, depth + 1);
    } else {
        return recurseGetNode(node->getRightNode(), point, depth + 1);
    }
}

Node* fminNode(Node* currentNode, Node* left, Node* right, int axis) {
    if (left != nullptr && left->getPoint()[axis] < currentNode->getPoint()[axis]) {
        return left;
    } else if (right != nullptr && right->getPoint()[axis] < currentNode->getPoint()[axis]) {
        return right;
    }
    return currentNode;
}

Node* KDTree::recurseFindMinimum(Node *node, unsigned int axis, unsigned int depth) {
    if (node == nullptr) {
        return nullptr;
    }

    unsigned int d = depth % k;
    if (d == axis) {
        if (node->getLeftNode() == nullptr) {
            return node;
        }

        return recurseFindMinimum(node->getLeftNode(), axis, depth + 1);
    }

    return fminNode(
        node,
        recurseFindMinimum(node->getLeftNode(), axis, depth + 1),
        recurseFindMinimum(node->getRightNode(), axis, depth + 1),
        axis
    );
}

Node* KDTree::recurseRemoveNode(Node *node, vector<double> point, unsigned int depth) {
    if (node == nullptr) {
        return nullptr;
    }

    unsigned int d = depth % k;

    if (node->getPoint() == point) {
        if (node->getRightNode() != nullptr) {
            Node* rSubMinNode = findMinimumAxisValueFromNode(node->getRightNode(), d);
            node->setPoint(rSubMinNode->getPoint());
            node->setRightNode(recurseRemoveNode(node->getRightNode(), rSubMinNode->getPoint(), depth + 1));
        } else if (node->getLeftNode() != nullptr) {
            Node* lSubMinNode = findMinimumAxisValueFromNode(node->getLeftNode(), d);
            node->setPoint(lSubMinNode->getPoint());
            node->setRightNode(recurseRemoveNode(node->getLeftNode(), lSubMinNode->getPoint(), depth + 1));
            node->setLeftNode(nullptr);
        } else {
            delete node;
            return nullptr;
        }
        return node;
    }

    if (point[d] < node->getPoint()[d]) {
        node->setLeftNode(recurseRemoveNode(node->getLeftNode(), point, depth + 1));
    } else {
        node->setRightNode(recurseRemoveNode(node->getRightNode(), point, depth + 1));
    }

    return node;
}

double KDTree::squaredDistance(vector<double> a, vector<double> b) {
    double distance = 0.0;
    for (unsigned int i = 0; i < k; i++) {
        double diff = a[i] - b[i];
        distance += diff * diff;
    }
    return distance;
}

void KDTree::recurseNN(Node *node, Node *target, Node*& currentBest, double& currentBestDist, unsigned int depth)
{
    if (node == nullptr) {
        return;
    }

    unsigned int d = depth % k;
    double currentDist = squaredDistance(node->getPoint(), target->getPoint());
    if (currentBest == nullptr || (currentDist < currentBestDist && currentDist > 0))
    {
        currentBest = node;
        currentBestDist = currentDist;
    }

    Node* nextBranch = nullptr;
    Node* otherBranch = nullptr;

    if (target->getPoint()[d] < node->getPoint()[d]) {
        nextBranch = node->getLeftNode();
        otherBranch = node->getRightNode();
    } else {
        nextBranch = node->getRightNode();
        otherBranch = node->getLeftNode();
    }

    recurseNN(nextBranch, target, currentBest, currentBestDist, depth + 1);

    double currentDistToPlane = target->getPoint()[d] - node->getPoint()[d];
    if ((currentDistToPlane * currentDistToPlane) < currentBestDist) {
        recurseNN(otherBranch, target, currentBest, currentBestDist, depth + 1);
    }
}

void KDTree::addNodeToInRangeList(Node *node, KDTree::Bounds b, vector<Node*>& nodesInRange) {
    bool inX = true;
    bool inY = true;
    bool inZ = true;
    if (k > 0) {
        inX = b.minX <= node->getPoint()[0] && node->getPoint()[0] <= b.maxX;
    }

    if (k > 1) {
        inY = b.minY <= node->getPoint()[1] && node->getPoint()[1] <= b.maxY;
    }

    if (k > 2) {
        inZ = b.minZ <= node->getPoint()[2] && node->getPoint()[2] <= b.maxZ;
    }

    if (inX && inY && inZ) {
        nodesInRange.push_back(node);
    }
}

void KDTree::recurseGetNodesInRange(Node *node, KDTree::Bounds b, vector<Node*>& nodesInRange) {
    if (node == nullptr) {
        return;
    }

    recurseGetNodesInRange(node->getLeftNode(), b, nodesInRange);
    addNodeToInRangeList(node, b, nodesInRange);
    recurseGetNodesInRange(node->getRightNode(), b, nodesInRange);
}

Node* KDTree::nearestNeighborSearch(Node* target) {
    Node *best = nullptr;
    double bestDist = numeric_limits<double>::infinity();

    recurseNN(root, target, best, bestDist, 0);
    return best;
}

KDTree::Bounds KDTree::makeRange(vector<double> pointOfOrigin, double width, double height, double length) {
    Bounds b;
    if (k > 0) {
        double ox = pointOfOrigin[0];
        b.minX = ox;
        b.maxX = ox + width;
    }

    if (k > 1) {
        double oy = pointOfOrigin[1];
        b.minY = oy;
        b.maxY = oy + height;
    }
    
    if (k > 2) {
        double oz = pointOfOrigin[2];
        b.minZ = oz;
        b.maxZ = oz + length;
    }

    return b;
}

vector<Node *> KDTree::rangeSearch(vector<double> pointOfOrigin, double height, double width, double length) {
    vector<Node*> nodesInRange;
    Bounds b = makeRange(pointOfOrigin, height, width, length);
    recurseGetNodesInRange(root, b, nodesInRange);
    return nodesInRange;
}

Node* KDTree::removeNode(vector<double> point) {
    return recurseRemoveNode(root, point, 0);
}

Node *KDTree::findMinimumAxisValueFromNode(Node *node, unsigned int axis) {
    return recurseFindMinimum(node == nullptr ? root : node, axis, 0);
}

Node* KDTree::insertNode(vector<double> point) {
    return recurseInsertion(root, point, 0);
}

Node* KDTree::getNode(vector<double> point) {
    return recurseGetNode(root, point, 0);
}

Node* KDTree::getRoot() {
    return root;
}

void KDTree::setRoot(Node *node) {
    root = node;
}

int KDTree::getDimensions() {
    return k;
}

void KDTree::printPoint(Node* node) {
    for (unsigned int i = 0; i < node->getPoint().size(); i++) {
        if (i + 1 != node->getPoint().size()) {
            cout << node->getPoint()[i] << ", ";
        } else {
            cout << node->getPoint()[i] << endl;
        }
    }
}
/**
 * @brief Recursively prints the KDTree in a structured tree format.
 * 
 * This method outputs the tree structure to the console as branches. 
 * Each node is displayed with its point, and child nodes are printed 
 * recursively with indentation to indicate tree hierarchy.
 * 
 * @param node poin to the current Node to print. If nullptr, nothing is printed.
 * @param prefix String used for indentation and branch lines (default is empty).
 * @param isLeft Boolean flag indicating whether the current node is a left child 
 *               (true) or right child (false) of its parent; used for branch symbols.
 */
void KDTree::printKDTree(Node *node, string prefix = "", bool isLeft = true) {
    if (!node) {
        return;
    }

    cout << prefix;
    cout << (isLeft ? "├──" : "└──");

    printPoint(node);

    // Extend the prefix for child branches
    string newPrefix = prefix + (isLeft ? "│   " : "    ");
    if (node->getLeftNode() || node->getRightNode()) {
        if (node->getLeftNode()) {
            printKDTree(node->getLeftNode(), newPrefix, true);
        } else {
            cout << newPrefix << "├──(null)\n";
        }

        if (node->getRightNode()) {
            printKDTree(node->getRightNode(), newPrefix, false);
        } else {
            cout << newPrefix << "└──(null)\n";
        }
    }
}
