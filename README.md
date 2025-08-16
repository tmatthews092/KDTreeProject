# KDTree Implementation

## Overview
This project implements a KDTree, a binary search tree for organizing points in *k* dimensions.  
It is used in artificial intelligence, machine learning, robotics, computer graphics, geospatial systems, and 3D modeling. A KDTree stores spatial data, where each node contains coordinates (e.g., `x, y, z`). Each internal node can have up to two children. Points are stored in a `std::vector<double>` instead of a raw array to support an arbitrary number of dimensions without manual resizing.

---

## Structure and Rules

### Node Organization
- At each level, nodes are compared on a specific dimension.  
- The dimension alternates as you go deeper in the tree.  
  - In 2D: root compares `x`, the next level compares `y`, the next `x` again, and so on.  
- **Left subtree**: points with a smaller value in the current dimension.  
- **Right subtree**: points with a greater value in the current dimension.  

### Example (2D)
- Root: `(1,1)`  
- Insert `(0,1)` → compared on `x`: `0 < 1`, goes left.  
- Insert `(-1,2)` → compared on `x`: `-1 < 1`, goes left → compared on `y`: `2 > 1`, goes right child of `(0,1)`.  

This alternating comparison effectively splits space by hyperplanes.  
- Points on the left are in one halfspace, points on the right are in the other.  
- This rule must always hold, otherwise the tree is invalid.  

---

## Implemented Methods

### `getNode`
- Recursively traverses the tree, comparing on the dimension of the current level.  
- Goes left if the point’s value is smaller, right otherwise.  
- Implemented first since other methods rely on this logic.

**Complexity**:  
- Time: `O(log n)` average, `O(n)` worst case  
- Space: `O(log n)` (recursion stack), `O(1)` iterative  

---

### `insertNode`
- Traverses the tree using alternating dimension comparisons.  
- Inserts at the first null branch encountered.  

**Complexity**:  
- Time: `O(log n)` average, `O(n)` worst case  
- Space: `O(log n)` (recursion stack), `O(1)` iterative  

---

### `findMinimumAxisValueFromNode`
- Finds the minimum node along a given axis, starting from a subtree root.  
- Recursively compares candidates where the target axis matches the current level.
- Used in `removeNode` to find a minimum node that will replace the to be removed node. 

**Complexity**:  
- Time: `O(log n)` average, `O(n)` worst case  
- Space: `O(log n)` recursion  

---

### `removeNode`
- Locate the target node.  
- Find the minimum in the relevant dimension from its subtree using `findMinimumAxisValueFromNode`.  
- Replace the target with that node, then recursively update left/right subtrees.  

**Complexity**:  
- Time: `O(log n)` average, `O(n)` worst case  
- Space: `O(log n)` recursion  

---

### `nearestNeighbor`
- Traverses down to the target region, then backtracks.  
- Updates the nearest point seen so far.  
- Explores the opposite branch if it might contain a closer point, when backtracking.  

**Complexity**:  
- Time: `O(log n)` average, `O(n)` worst case  
- Space: `O(log n)` recursion  

---

### `rangeSearch`
- Finds all points inside a bounding box (rectangle in 2D, cube in 3D).  
- Requires an origin, height, width, and (to be ignored in 2D) length.  
- Checks if each point lies within the `[min, max]` of the bounding box for each dimension.  

**Complexity**:  
- Time: `O(n^(1 - 1/k) * m)`, where *m* = number of results  
- Space: `O(m)` for storing results + `O(log n)` recursion  

---

## Running the Project

### Option 1: Run Tests
1. `cd build`  
2. `cmake ..`  
3. `make && ./run_tests`  

This will run:
- All methods with **2D points**  
- `getNode` and `insert` with **3D points**  

### Option 2: Run Application
- In VSCode Juypter Ctrl + Shift + D
- F5 or click Run App in Run and Debug Tab
- Choose tree dimensionality (**2D or 3D**).
- Use the console menu:
[1] Get Node <br>
[2] Insert <br>
[3] Find Min From Node (Given Axis) <br>
[4] Remove <br>
[5] Find Nearest Neighbor <br>
[6] Range Search <br>
[7] Print Tree <br>
[0] Exit
