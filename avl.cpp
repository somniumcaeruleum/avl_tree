#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;

class Node;
void update(Node* p);
void insert(Node* p, int data);
void rotate(Node* p);
void rotate_left(Node* p);
void rotate_right(Node* p);
void store(Node*, int, int*, bool*);
void print();
void print_testing(Node* p);
void kill_nodes(Node*);
void del_node(Node* p, int data);
Node* find_node(Node* p, int data);

Node* root{ nullptr };

int max(int a, int b) {
    if (a >= b) return a;
    else return b;
}

class Node {
public:
    int data;

    int leftNum{}; //the maximum height of left subtree
    int rightNum{}; //the maximun height of right subtree

    Node* left{};
    Node* right{};
    Node* upL{};
    Node* upR{};

    int varience() { //-1, 0, 1
        this->update_leftNum();
        this->update_rightNum();

        return leftNum - rightNum;
    }

    int height() {
        if (this == nullptr) return -1;
        else return max(leftNum, rightNum);
    }

    int sideofUp() { //-1: left, 0: it's root node, 1: right
        if (upL != nullptr) {
            return -1;
        }
        else if (upR != nullptr) {
            return 1;
        }
        else return 0;
    }

    Node* upper() {
        if (upL != nullptr) return upL;
        else return upR;
    }

    void update_leftNum() {
        if (left == nullptr) leftNum = 0;
        else leftNum = left->height() + 1;
    }

    void update_rightNum() {
        if (right == nullptr) rightNum = 0;
        else rightNum = right->height() + 1;
    }

    void replace(Node* object) {
        this->left = object->left;
        this->right = object->right;
        this->upL = object->upL;
        this->upR = object->upR;

        if (object->left != nullptr) {
            (object->left)->upR = this;
        }
        if (object->right != nullptr) {
            (object->right)->upL = this;
        }
        if (object->upR != nullptr) {
            (object->upR)->left = this;
        }
        if (object->upL != nullptr) {
            (object->upL)->right = this;
        }

        this->update_leftNum();
        this->update_rightNum();
    }

    Node(int d, Node* l, Node* r)
        :data(d), upL(l), upR(r) {
        left = right = nullptr;
    }
};

void update(Node* p) {
    if (p == nullptr) return;

    if (p->left != nullptr) {
        p->leftNum = (p->left)->height() + 1;
    }
    if (p->right != nullptr) {
        p->rightNum = (p->right)->height() + 1;
    }
    
    if (p->upL != nullptr) update(p->upL);
    else if (p->upR != nullptr) update(p->upR);
}

void insert(Node* p, int data) {
    if (p->data == data) {
        return;
    }

    if (p->data < data) {
        if (p->right == nullptr) {
            p->right = new Node(data, p, nullptr);

            //update
            update(p);

            //rotate
            rotate(p->upper());
        }
        else
            insert(p->right, data);
        return;
    }

    else if (p->data > data) {
        if (p->left == nullptr) {
            p->left = new Node(data, 0, p);

            //update
            update(p);

            //rotate
            rotate(p->upper());
        }
        else
            insert(p->left, data);
        return;
    }
}

void rotate_right(Node* p) {
    if (p == nullptr) return;
    cout << "rotate_right called on " << p->data << endl;

    Node* subroot = p->upper();
    int side = p->sideofUp(); //-1, 0, 1 | left, none, right (upperside from child)

    Node* c = p;
    Node* b = c->left;
    Node* x = b->right; //probably nullptr

    c->left = x;
    c->update_leftNum();

    if (x != nullptr) {
        x->upL = nullptr;
        x->upR = c;
    }

    b->right = c;
    b->update_rightNum();

    c->upR = nullptr;
    c->upL = b;

    b->upL = b->upR = nullptr;
    if (side == 0) {
        root = b;
    }
    else if (side == -1) { //right
        subroot->right = b;
        b->upL = subroot;
        subroot->update_rightNum();
    }
    else if (side == 1) { //left
        subroot->left = b;
        b->upR = subroot;
        subroot->update_leftNum();
    }
}

void rotate_left(Node* p) {
    if (p == nullptr) return;
    cout << "rotate_left called on " << p->data << endl;

    Node* subroot = p->upper();
    int side = p->sideofUp(); //-1, 0, 1 | left, none, right (upperside from child)

    Node* c = p;
    Node* b = c->right;
    Node* y = b->left; //probably nullptr

    if (y != nullptr) {
        y->upL = c;
        y->upR = nullptr;
    }

    c->right = y;
    c->update_rightNum();

    c->upL = nullptr;
    c->upR = b;

    b->left = c;
    b->update_leftNum();

    b->upL = b->upR = nullptr;
    if (side == 0) {
        root = b;
    }
    else if (side == -1) { //right
        subroot->right = b;
        b->upL = subroot;
        subroot->update_rightNum();
    }
    else if (side == 1) { //left
        subroot->left = b;
        b->upR = subroot;
        subroot->update_leftNum();
    }
}

void rotate(Node* p) {
    if (p == nullptr) return;

    //right || left-right
    if (p->varience() > 1) {
        if ((p->left) != nullptr) {
            //right
            if ((p->left)->varience() >= 0) {
                rotate_right(p);
            }
            //left-right
            else if ((p->left)->varience() < 0) {
                //left
                rotate_left(p->left);
                //right
                rotate_right(p);
            }
        }
    }
    //left || right-left
    else if (p->varience() < -1) {
        //right-left
        if (((p->right) != nullptr) && ((p->right)->varience() > 0)) {
            //right
            rotate_right(p->right);
            //left
            rotate_left(p);
        }
        //left
        else if (((p->right) != nullptr) && ((p->right)->varience() <= 0)) {
            rotate_left(p);
        }
    }

    if (p->upL != nullptr) rotate(p->upL);
    if (p->upR != nullptr) rotate(p->upR);

    return;
}

void store(Node* p, int i, int* data, bool* flag) {
    if (p == nullptr) return;

    data[i] = p->data;
    flag[i] = true;

    store(p->left, 2 * i + 1, data, flag);
    store(p->right, 2 * i + 2, data, flag);
}

void print() {
    if (root == nullptr) {
        cout << "The avl tree is empty now." << endl;
        return;
    }

    int height = root->height();
    int n = (1 << (height + 1)) - 1; //the number of the elements
    int m = 1 << height; //the number of children

    int* data = new int[n];
    bool* flag = new bool[n];

    for (int i{}; i < n; i++) flag[i] = false;

    store(root, 0, data, flag);

    for (int i{}; i <= height; i++) { //the number of lines
        for (int j{ (1 << i) - 1 }; j <= 2 * ((1 << i) - 1); j++) { //the index of elements in the data
            if (flag[j]) printf("%-5d", data[j]);
            else cout << "*    ";

            for (int k{}; k < (((1 << height) - (1 << i)) / (1 << i)); k++) {
                cout << "     ";
            }
        }
        cout << endl;
    }
}

void print_testing(Node* p) {
    if (p == nullptr) return;

    cout << p->data << endl;
    cout << "Left: ";
    if (p->left == nullptr) cout << "none ";
    else cout << (p->left)->data << " ";

    cout << "Right: ";
    if (p->right == nullptr) cout << "none ";
    else cout << (p->right)->data << " " << endl;

    cout << endl;

    print_testing(p->left);
    print_testing(p->right);

    return;
}

void kill_nodes(Node* p) {
    if (p == nullptr) return;

    kill_nodes(p->left);
    kill_nodes(p->right);

    //cout << p->data << endl;

    delete p;
}

Node* find_w(Node* p) {
    if (p->left == nullptr) return p;
    else return find_w(p->left);
}

void del_node(Node* p, int data) {
    if (p == nullptr) return;

    if (p->data > data) {
        del_node(p->left, data);
        return;
    }
    else if (p->data < data) {
        del_node(p->right, data);
        return;
    }
    else {
        //delete p
        cout << "find!" << endl;
        if ((p->left == nullptr) && (p->right == nullptr)) { //p is single
            Node* up = p->upper();
            if (p->upL != nullptr) {
                up->right = nullptr;
                delete p;
                update(up);
                rotate(up);
            }
            else if (p->upR != nullptr) {
                up->left = nullptr;
                delete p;
                update(up);
                rotate(up);
            }
            else {
                delete p;
                root = nullptr;
            }
        }

        else if ((p->left != nullptr) && (p->right == nullptr)) { //only left exist
            if (root == p) {
                root = p->left;
                root->upR = root->upL = root->left = root->right = nullptr;
                delete p;
            }
            else {
                Node* subroot = p->upper();
                if (p->sideofUp() == -1) { //upL
                    (p->left)->upL = subroot;
                    subroot->right = p->left;

                    update(subroot);
                    rotate(subroot);
                }
                else {
                    (p->left)->upR = subroot;
                    subroot->left = p->right;

                    update(subroot);
                    rotate(subroot);
                }
            }
        }

        else { //***
            Node* w = find_w(p->right);
            Node* upR = w->upR;

            if (upR == nullptr) { //right is only w
                Node* tmp = w->right;
                w->left = p->left;
                w->upR = p->upR;
                w->upL = p->upL;

                if (w->left != nullptr) {
                    (w->left)->upR = w;
                    w->update_leftNum();
                }
                if (w->upR != nullptr) {
                    (w->upR)->left = w;
                    (w->upR)->update_leftNum();
                }
                if (w->upL != nullptr) {
                    (w->upL)->right = w;
                    (w->upL)->update_rightNum();
                }

                delete p;

                if (w->sideofUp() == 0) root = w;

                update(w);
                rotate(w);
            }
            
            else { //there're other nodes on right. ******

                upR->left = w->right;
                if (w->right != nullptr) {
                    (w->right)->upL = nullptr;
                    (w->right)->upR = upR;
                }

                upR->update_leftNum();

                w->replace(p);
                delete p;

                if (w->sideofUp() == 0) root = w;

                update(upR);
                rotate(upR);
            }
        }
    }
}

Node* find_node(Node* p, int data) {
    if (p == nullptr) return nullptr;

    if (p->data < data) return find_node(p->right, data);
    else if (p->data > data) return find_node(p->left, data);
    else return p;
}

int main()
{
    char menu{};

    while (menu != 'q') {
        print();
        /*
        cout << "------------"<<endl;
        print_testing(root);
        cout << "------------"<<endl;
        */

        cout << "Enter menu: ";
        cin >> menu;

        int data;

        switch (menu) {
        case 'i':
        case 'I':
            //Insert
            cin >> data;
            if (root == nullptr)
                root = new Node(data, nullptr, nullptr);
            else
                insert(root, data);
            break;

        case 'f':
        case 'F':
            //Find
            cin >> data;
            if (find_node(root, data) != nullptr) {
                cout << "Find!" << endl;
            }
            else {
                cout << "Doesn't exist!" << endl;
            }
            break;

        case 'd':
        case 'D':
            //Delete
            cin >> data;

            del_node(root, data);
            break;
        
        case 'e':
        case 'E':
            cin >> data;

            Node* n = find_node(root, data);
            cout << n->varience() << endl;
        }
    }

    kill_nodes(root);
    return 0;
}