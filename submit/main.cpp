#include <iostream>

struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class AVL {
private:
    Node* root;

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    int size(Node* node) {
        if (node == nullptr) return 0;
        return 1 + size(node->left) + size(node->right);
    }

    // 재귀적으로 높이를 계산하는 함수
    int height(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + max(height(node->left), height(node->right));
    }

    int getBalance(Node* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // 오른쪽 회전
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        return x;
    }

    // 왼쪽 회전
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        return y;
    }

    // 균형을 맞추기 위한 함수
    Node* balance(Node* node) {
        int balanceFactor = getBalance(node);

        // 왼쪽 불균형
        if (balanceFactor > 1) {
            // LL 케이스
            if (getBalance(node->left) >= 0) {
                return rotateRight(node);
            }
            // LR 케이스
            else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        // 오른쪽 불균형
        if (balanceFactor < -1) {
            // RR 케이스
            if (getBalance(node->right) < 0) {
                return rotateLeft(node);
            }
            // RL 케이스
            else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }
        return node;
    }

    // AVL 트리에 삽입
    Node* insertAVL(Node* node, int key, bool& success) { // 재귀이기 때문에 실제 삽입이 됐는지 체크하기 위한 매개변수 추가
        if (node == nullptr) {
            return new Node(key);
        }

        if (key < node->key) {
            node->left = insertAVL(node->left, key, success);
        }
        else if (key > node->key) {
            node->right = insertAVL(node->right, key, success);
        }
        else {
            std::cout << "i " << key << ": The key already exists" << std::endl; // 이미 존재하는 경우에 에러 메세지 출력
            success = false;
            return node;
        }

        return balance(node);
    }

    // AVL 트리에서 삭제
    Node* eraseAVL(Node* node, int deleteKey, bool& success) { // 재귀이기 때문에 실제 삽입이 됐는지 체크하기 위한 매개변수 추가
        if (node == nullptr) {
            // 노드가 없는 경우 에러 메시지 출력
            std::cout << "d " << deleteKey << ": The key does not exist" << std::endl;
            success = false;
            return nullptr;
        }

        if (deleteKey < node->key) {
            node->left = eraseAVL(node->left, deleteKey, success);
        }
        else if (deleteKey > node->key) {
            node->right = eraseAVL(node->right, deleteKey, success);
        }
        else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                success = true;
                return nullptr;
            }
            else if (node->left == nullptr) {
                Node* temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node;
                node = node->left;
                delete temp;
            }
            else {
                int leftHeight = height(node->left);
                int rightHeight = height(node->right);
                Node* temp = nullptr;

                if (leftHeight == rightHeight) {
                    if (size(node->left) >= size(node->right)) {
                        temp = maxInLeft(node->left);
                        node->key = temp->key;
                        node->left = eraseAVL(node->left, temp->key, success);
                    }
                    else {
                        temp = minInRight(node->right);
                        node->key = temp->key;
                        node->right = eraseAVL(node->right, temp->key, success);
                    }
                }
                else if (leftHeight < rightHeight) {
                    temp = minInRight(node->right); 
                    node->key = temp->key;
                    node->right = eraseAVL(node->right, temp->key, success);
                }
                else {
                    temp = maxInLeft(node->left);
                    node->key = temp->key;
                    node->left = eraseAVL(node->left, temp->key, success);
                }
            }
        }

        // 삭제 후 AVL 트리의 균형 유지
        return balance(node);
    }

    Node* minInRight(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    Node* maxInLeft(Node* node) {
        while (node && node->right) {
            node = node->right;
        }
        return node;
    }

    void inorderTree(Node* node, std::ostream& os) {
         if (node == nullptr) return;
        os << "<";
         inorderTree(node->left, os);

        os << " " << node->key << " ";
        inorderTree(node->right, os);
        os << ">";
    }

public:
    AVL() : root(nullptr) {}

    bool insert(int key) {
        bool success = true;
        root = insertAVL(root, key, success);
        return success;
    }

    bool erase(int key) {
        bool success = true;
        root = eraseAVL(root, key, success);
        return success;
    }

    friend std::ostream& operator<<(std::ostream& os, AVL& tree) {
        tree.inorderTree(tree.root, os);
        return os;
    }

    Node* getRoot() const {
        return root;
    }

    void clear() {
        clear(root);
    }

private:
    void clear(Node*& node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
        node = nullptr;
    }
};

int main() {
    AVL tree;
    char command;
    int key;

    while (std::cin >> command >> key) {
        switch (command) {
        case 'i': {
            if (tree.insert(key)) {
                std::cout << "<";
                std::cout << tree;
                std::cout << ">" << std::endl;
            }
            break;
        }

        case 'd': {
            if (tree.erase(key)) {
                if (tree.getRoot() != nullptr) {
                    std::cout << "<";
                    std::cout << tree;
                    std::cout << ">" << std::endl;
                }
                else {
                    std::cout << std::endl; // 마지막 노드 삭제시 개행
                }
            }
            break;
        }
        }
    }
    tree.clear();
    return 0;
}
