#include <iostream>
#include "../code/Node.h"
#include "../code/KDTree.h"

using namespace std;

vector<double> enterPoint(KDTree* kdTree) {
    cout << "Enter point:" << endl;
    vector<double> point = vector<double>();
    double var;

    for (int i = 0; i < kdTree->getDimensions(); i++) {
        cin >> var;
        point.push_back(var);
    }

    return point;
}

int enterDimension(KDTree* kdTree) {
    int var;
    cout << "Enter dimension to find minimum; " << endl;
    bool isGettingDim = true;
    while (isGettingDim) {
        cout << "Dimension must be between 0 and " << kdTree->getDimensions() - 1 << endl;
        cin >> var;
        if (0 >= var && var <= kdTree->getDimensions() - 1) {
            break;
        }
    }
    return var;
}

void checkNodeFound(Node* node) {
    if (node != nullptr) {
        cout << "Node found: ";
        for (unsigned int i = 0; i < node->getPoint().size(); i++) {
            if (i + 1 != node->getPoint().size()) {
                cout << node->getPoint()[i] << ", ";
            } else {
                cout << node->getPoint()[i] << endl;
            }
        }
    } else {
        cout << "Node not found." << endl;
    }
}

int main(){
    cout << "====||| KDTree Application |||====" << endl << endl;

    bool isRunningInit = true;
    bool isRunningApp = true;
    int var;
    KDTree* kdTree;

    cout << "Initialize the KDTree. How many dimensions? Must be 2 or 3" << endl;
    while (isRunningInit) {
        cin >> var;
        if (var > 1 && var < 4) {
            cout << "Creating KDTree with " << var << " dimensions." << endl;
            kdTree = new KDTree(var);
            isRunningInit = false;
            break;
        }
    }

    while (isRunningApp) {
        cout << "**********************" << endl;
        cout << "[1]: Get Node" << endl;
        cout << "[2]: Insert" << endl;
        cout << "[3]: Find Min From Node, Given Axis" << endl;
        cout << "[4]: Remove" << endl;
        cout << "[5]: Find Nearest Neighbor to Node" << endl;
        cout << "[6]: Find Nodes in Range" << endl;
        cout << "[7]: Print Tree" << endl;
        cout << "[0]: Exit Application" << endl;
        cin >> var;
        cout << "**********************" << endl;

        if (kdTree->getRoot() != nullptr) {
            if (var == 1) {
                Node* node = kdTree->getNode(enterPoint(kdTree));
                checkNodeFound(node);
            } else if (var == 3) {
                //min
                Node* node = kdTree->findMinimumAxisValueFromNode(kdTree->getNode(enterPoint(kdTree)), enterDimension(kdTree));
                checkNodeFound(node);
            } else if (var == 4) {
                kdTree->setRoot(kdTree->removeNode(enterPoint(kdTree)));
                cout << "**********************" << endl;
                kdTree->printKDTree(kdTree->getRoot(), "", false);
            } else if (var == 5) {
                Node* nearestNode = kdTree->nearestNeighborSearch(kdTree->getNode(enterPoint(kdTree)));
                checkNodeFound(nearestNode);
            } else if (var == 6) {
                cout << "Enter point of origin for your range search." << endl;
                vector<double> point = enterPoint(kdTree);
                double height;
                double width;
                double length;
                cout << "Enter height of range" << endl;
                cin >> height;
                cout << "Enter width of range" << endl;
                cin >> width;
                if (kdTree->getDimensions() > 2) {
                    cout << "Enter length of range" << endl;
                    cin >> length;
                }
                vector<Node*> nodesInRange = kdTree->rangeSearch(point, height, width, length);
                for (Node* node : nodesInRange) {
                    checkNodeFound(node);
                }
            } else if (var == 7) {
                kdTree->printKDTree(kdTree->getRoot(), "", false);
            }
        }
        
        if (var == 2) {
            kdTree->setRoot(kdTree->insertNode(enterPoint(kdTree)));
            cout << "**********************" << endl;
            kdTree->printKDTree(kdTree->getRoot(), "", false);
        } else if (kdTree->getRoot() == nullptr) {
            cout << "Cannot make this operation, root is undefined." << endl;
        } else if (cin.fail()) {
            cout << "Invalid input." << endl;
        }

        if (!var) {
            cout << "Quitting Application..." << endl;
            break;
        }
        cin.clear();
    }

    return 0;
}
