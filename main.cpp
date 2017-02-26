#include <list>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

class Node {
private:
    void remove_LeftSon(Node* node) {
        node->left->parent = node->parent;
        if (node->parent != nullptr) {
            if (node->parent->right == node)
                node->parent->right = node->left;
            else
                node->parent->left = node->left;
        }
        delete node;
    }

    void remove_Leaf(Node* node) {
        if (node->parent != nullptr) {
            if (node->parent->right == node)
                node->parent->right = nullptr;
            else
                node->parent->left = nullptr;
        }
        delete node;
    }

    void remove_RightSon(Node* node) {
        node->right->parent = node->parent;
        if (node->parent != nullptr) {
            if (node->parent->right == node)
                node->parent->right = node->right;
            else
                node->parent->left = node->right;
        }
        delete node;
    }

    void remove_BothSons(Node* node) {
        Node* nextNode = node->right;
        while (nextNode->left != nullptr)
            nextNode = nextNode->left;
        swap(node->key, nextNode->key);
        nextNode->removeRightWay();
    }

public:
    long long key, treeSize;
    bool flag;
    Node* left, * right, * parent;

    Node(long long value = 0) {
        key = value;
        left = right = parent = nullptr;
        flag = false;
        left_size = right_size = 0;
    }

    void removeRightWay() {
        if (this->left != nullptr && this->right != nullptr) {
            remove_BothSons(this);
        } else if (this->left != nullptr) {
            remove_LeftSon(this);
        } else if (this->right != nullptr) {
            remove_RightSon(this);
        } else {
            remove_Leaf(this);
        }
    }
};

class BinarySearchTree {
private:
    Node* root;
    function<void(Node*)> strategy;

    Node* find(long long value) const {
        Node* ans = root;
        while (ans != nullptr) {
            if (ans->key == value)
                return ans;
            else if (value < ans->key)
                ans = ans->left;
            else
                ans = ans->right;
        }
        return ans;
    }

    void rootLeftRightTraversalNodes(Node* node) {
        if (node == nullptr)
            return;
        if (strategy)
            strategy(node);
        rootLeftRightTraversalNodes(node->left);
        rootLeftRightTraversalNodes(node->right);
    }

    void leftRightRootTraversalNodes(Node* node) {
        if (node == nullptr)
            return;
        rootLeftRightTraversalNodes(node->left);
        rootLeftRightTraversalNodes(node->right);
        if (strategy)
            strategy(node);
    }

public:

    Node* getRoot() const {
        return root;
    }

    BinarySearchTree() {
        root = nullptr;
    }

    void add(long long value) {
        Node** curr = &root, ** prev = nullptr;
        while (*curr) {
            prev = curr;
            if (value < (*curr)->key)
                curr = &(*curr)->left;
            else if (value > (*curr)->key)
                curr = &(*curr)->right;
            else
                return;
        }
        *curr = new Node(value);
        if (prev != nullptr)
            (*curr)->parent = *prev;
    }

    void removeRightWay(long long value) {
        Node* node = find(value);
        if (node == nullptr)
            return;

        if (node == root) {
            Node* temp = new Node();
            temp->left = root;
            root->parent = temp;
            node->removeRightWay();
            root = temp->left;
            if (root != nullptr)
                root->parent = nullptr;
            delete temp;
        } else
            node->removeRightWay();
    }

    void rootLeftRightTraversal() {
        rootLeftRightTraversalNodes(root);
    }

    void leftRightRootTraversal() {
        leftRightRootTraversalNodes(root);
    }

    bool hasKey(long long value) const {
        return find(value) != nullptr;
    }

    void setStrategy(function<void(Node*)> func) {
        strategy = func;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    long long key;
    BinarySearchTree bst;
    while (cin >> key) {
        bst.add(key);
    }

    auto calcSubtreeSize = [](Node* node) {
        node->treeSize = 1;
        if (node->left != nullptr && node->right != nullptr)
            node->treeSize += node->left->treeSize + node->right->treeSize;
        else if (node->left != nullptr)
            node->treeSize += node->left->treeSize;
        else if (node->right != nullptr)
            node->treeSize += node->right->treeSize;
    };
    bst.setStrategy(calcSubtreeSize);
    bst.leftRightRootTraversal();


    list<Node> traverseNodes;
    auto pushNodesToList = [&traverseNodes](Node* node) { traverseNodes.push_back(*node); };
    bst.setStrategy(pushNodesToList);
    bst.rootLeftRightTraversal();
    for (auto node : traverseNodes)
        cout << node.key << "\n";

    return 0;
}
