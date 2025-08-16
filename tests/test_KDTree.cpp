// Chekout TEST_F functions bellow to learn what is being tested.
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime> // For time()

#include "../code/Node.h"
#include "../code/KDTree.h"

#include <gtest/gtest.h>


// in order to access private members, we have to defeat the safety
// mechanisms. This is acceptable for unit testing, but not much
// else. It is also not portable because it is not supported by the
// C++ spec, and therefore might not work with all compilers.
#define private public

using namespace std;

unsigned int k;

class test_KDTree : public ::testing::Test {
    protected:
        // This function runs only once before any TEST_F function
        static void SetUpTestCase() {}
        // This function runs after all TEST_F functions have been executed
        static void TearDownTestCase() {}
        // this function runs before every TEST_F function
        void SetUp() override {}
        void TearDown() override {}
};

double genDbl0_100() {
    // Random number generator
    return rand() % 101;
}

vector<Node *> generate2DSpaceNodes(int numNodes)
{
    srand(time(0));

    vector<Node*> nodes = vector<Node*>();

    for (int i = 0; i < numNodes; i++) {
        vector<double> point = vector<double>{genDbl0_100(), genDbl0_100()};
        Node* node = new Node(point);
        nodes.push_back(node);
    }
    return nodes;
}

vector<vector<double>> generate2DSpacePoints(int numNodes)
{
    srand(time(0));

    vector<vector<double>> points = vector<vector<double>>();
    for (int i = 0; i < numNodes; i++) {
        points.push_back(vector<double>{genDbl0_100(), genDbl0_100()});
    }
    return points;
}

vector<vector<double>> getSimplePresetPoints() {
    /**    (8,5)
           /   \
        (3,6) (10,2)
    */
    return vector<vector<double>>{
        {8.0, 5.0},
        {3.0, 6.0},
        {10.0, 2.0}
    };
}

vector<vector<double>> getComplexPresetPoints()
{
    /**    (8,5)
           /   \
        (3,6) (10,2)
    */
    return vector<vector<double>>{
        {8.0, 5.0},
        {3.0, 6.0},
        {10.0, 2.0},
        {5.0, 7.0},
        {9.0,1.0},
        {1.0, 1.0},
        {11.0, 0.0}
    };
}

vector<vector<double>> getSimplePreset3DPoints() {
    /**    (8,5)
           /   \
        (3,6) (10,2)
    */
    return vector<vector<double>>{
        {8.0, 5.0, 1.0},
        {3.0, 6.0, 10.0},
        {10.0, 2.0, 5.0}
    };
}

vector<vector<double>> getComplexPreset3DPoints()
{
    /**    (8,5)
           /   \
        (3,6) (10,2)
    */
    return vector<vector<double>>{
        {8.0, 5.0, 9.0},
        {3.0, 6.0, 2.0},
        {10.0, 2.0, 3.0},
        {5.0, 7.0, 1.0},
        {9.0, 1.0, 5.0},
        {1.0, 1.0, 4.0},
        {11.0, 0.0, 8.0},
        {4.0, 14.0, 4.0},
        {7.0, 15.0, 0.0}
    };
}

TEST_F(test_KDTree, KDTree_Constructor)
{
    bool seenError = false;
    try {
        new KDTree(0);
    } catch (const invalid_argument& e) {
        seenError = true;
    }
    ASSERT_TRUE(seenError);
}

TEST_F(test_KDTree, KDTree_NodesInit) {
    {
        int numNodes = 3;
        vector<Node *> nodes = generate2DSpaceNodes(numNodes);
        for (auto node : nodes) {
            ASSERT_TRUE(node->getPoint().size() == 2);
            ASSERT_TRUE(node->getLeftNode() == NULL);
            ASSERT_TRUE(node->getRightNode() == NULL);
        }
    }
}

TEST_F(test_KDTree, KDTree_GetNode)
{
    {
        vector<vector<double>> points = getSimplePresetPoints();

        KDTree *kdTree = new KDTree(points[0].size());
        Node* root = new Node(points[0]);
        Node* left = new Node(points[1]);
        Node* right = new Node(points[2]);
        root->setLeftNode(left);
        root->setRightNode(right);
        kdTree->setRoot(root);

        for (auto point : points)
        {
            ASSERT_TRUE(kdTree->getNode(point) != nullptr);
        }

        ASSERT_TRUE(kdTree->getNode(vector<double>{101, 200}) == nullptr);
    }
}

TEST_F(test_KDTree, KDTree_NodesInsert)
{
    {
        int numNodes = 3;
        vector<vector<double>> points = generate2DSpacePoints(numNodes);
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }

        for (auto point : points) {
            ASSERT_TRUE(kdTree->getNode(point) != nullptr);
        }
    }
}

TEST_F(test_KDTree, KDTree_NodesFindMin)
{
    {
        vector<vector<double>> points = getSimplePresetPoints();
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }

        ASSERT_TRUE(kdTree->getNode(points[1]) == kdTree->findMinimumAxisValueFromNode(kdTree->getRoot(), 0)); // find min x
        ASSERT_TRUE(kdTree->getNode(points[2]) == kdTree->findMinimumAxisValueFromNode(kdTree->getRoot(), 1)); // find min y
    }
}

TEST_F(test_KDTree, KDTree_NodesFindMinRoot)
{
    {
        vector<vector<double>> points = getSimplePresetPoints();
        points.erase(points.begin() + 2);
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }

        ASSERT_TRUE(kdTree->getNode(points[1]) == kdTree->findMinimumAxisValueFromNode(kdTree->getRoot(), 0)); // find min x
        ASSERT_TRUE(kdTree->getNode(points[0]) == kdTree->findMinimumAxisValueFromNode(kdTree->getRoot(), 1)); // find min y
    }
}

TEST_F(test_KDTree, KDTree_RemoveRootNode)
{
    {
        vector<vector<double>> points = getSimplePresetPoints();
        points.push_back({7.0,4.0});
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }

        ASSERT_TRUE(kdTree->getNode(points[0]) != nullptr);
        kdTree->setRoot(kdTree->removeNode(kdTree->getRoot()->getPoint()));
        ASSERT_TRUE(kdTree->getNode(points[0]) == nullptr);
    }
}

TEST_F(test_KDTree, KDTree_RemoveInternalNode)
{
    {
        vector<vector<double>> points = generate2DSpacePoints(8);
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }

        ASSERT_TRUE(kdTree->getNode(points[4]) != nullptr);
        kdTree->setRoot(kdTree->removeNode(points[4]));
        ASSERT_TRUE(kdTree->getNode(points[4]) == nullptr);
    }
}

TEST_F(test_KDTree, KDTree_RemoveInternalNodePreset)
{
    {
        vector<vector<double>> points = getComplexPresetPoints();
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }

        ASSERT_TRUE(kdTree->getNode(points[4]) != nullptr);
        ASSERT_TRUE(kdTree->getNode(points[2]) != nullptr);
        //remove 9,1
        kdTree->setRoot(kdTree->removeNode(points[4]));
        ASSERT_TRUE(kdTree->getNode(points[4]) == nullptr);
        ASSERT_TRUE(kdTree->getNode(points[6]) != nullptr);
        ASSERT_TRUE(kdTree->getNode(points[2])->getLeftNode()->getPoint() == points[6]);

        //remove 3,6
        kdTree->setRoot(kdTree->removeNode(points[1]));
        ASSERT_TRUE(kdTree->getNode(points[1]) == nullptr);
        ASSERT_TRUE(kdTree->getNode(points[5]) != nullptr);
        ASSERT_TRUE(kdTree->getNode(points[3]) != nullptr);
        ASSERT_TRUE(kdTree->getNode(points[3])->getLeftNode()->getPoint() == points[5]);

        //remove 10, 2
        kdTree->setRoot(kdTree->removeNode(points[2]));
        ASSERT_TRUE(kdTree->getNode(points[2]) == nullptr);
        ASSERT_TRUE(kdTree->getRoot()->getRightNode()->getPoint() == points[6]);
    }
}

TEST_F(test_KDTree, KDTree_NNPreset)
{
    {
        vector<vector<double>> points = getComplexPresetPoints();
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }

        Node* nnNode = kdTree->nearestNeighborSearch(kdTree->getNode(points[4]));

        ASSERT_TRUE(nnNode->getPoint() == points[2]);
        kdTree->setRoot(kdTree->removeNode(points[2]));

        ASSERT_TRUE(kdTree->getNode(points[4]) != nullptr);

        nnNode = kdTree->nearestNeighborSearch(kdTree->getNode(points[4]));
        ASSERT_TRUE(nnNode->getPoint() == points[6]);
    }
}

TEST_F(test_KDTree, KDTree_RangeSearch)
{
    {
        vector<vector<double>> points = getComplexPresetPoints();
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }
        vector<double> pointOfOrigin = vector<double>{9.0,0.0};
        vector<Node*> nodesInRange = kdTree->rangeSearch(pointOfOrigin, 3, 3, 0);
        ASSERT_TRUE(nodesInRange[0]->getPoint() == points[4]);
        ASSERT_TRUE(nodesInRange[1]->getPoint() == points[6]);
        ASSERT_TRUE(nodesInRange[2]->getPoint() == points[2]);
    }
}

TEST_F(test_KDTree, KDTree_3DGetNodeComplex)
{
    {
        vector<vector<double>> points = getComplexPreset3DPoints();
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }
        ASSERT_TRUE(kdTree->getNode(points[0]) != nullptr);
        ASSERT_TRUE(kdTree->getNode(points[1]) != nullptr);
        ASSERT_TRUE(kdTree->getNode(points[3]) != nullptr);
        ASSERT_TRUE(kdTree->getNode(points[5]) != nullptr);
        ASSERT_TRUE(kdTree->getNode(vector<double>{100.0,1.0,1.0}) == nullptr);
    }
}

TEST_F(test_KDTree, KDTree_3DInsert)
{
    {
        vector<vector<double>> points = getSimplePreset3DPoints();
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }
        ASSERT_TRUE(kdTree->getRoot()->getPoint() == points[0]);
        ASSERT_TRUE(kdTree->getRoot()->getLeftNode()->getPoint() == points[1]);
        ASSERT_TRUE(kdTree->getRoot()->getRightNode()->getPoint() == points[2]);
    }
}

TEST_F(test_KDTree, KDTree_3DInsertComplex)
{
    {
        vector<vector<double>> points = getComplexPreset3DPoints();
        KDTree *kdTree = new KDTree(points[0].size());
        for (auto point : points)
        {
            kdTree->setRoot(kdTree->insertNode(point));
        }
        ASSERT_TRUE(kdTree->getNode(points[3]) != nullptr);
        ASSERT_TRUE(kdTree->getNode(points[3])->getLeftNode()->getPoint() == points[8]);
        ASSERT_TRUE(kdTree->getNode(points[3])->getRightNode()->getPoint() == points[7]);
    }
}
