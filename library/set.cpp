#include<algorithm>


template<typename T>
class Set {
public:
    Set() {
        End = new Node(true);
    }

    template<typename Iterator>
    Set(Iterator first, Iterator last) {
        for (Iterator it = first; it != last; ++it) {
            add(*it);
        }
        End = new Node(true);
    }

    Set(std::initializer_list<T> elems) {
        for (auto elem:elems) {
            add(elem);
        }
        End = new Node(true);
    }

    Set(const Set& s) {
        root = deepCopy(s.root);
        End = new Node(true);
        Size = s.Size;
    }

    ~Set() {
        eraseAll(root);
        delete End;
    }

    Set& operator=(const Set&right) {
        if (this == &right) {
            return *this;
        }
        eraseAll(root);
        delete End;
        root = deepCopy(right.root);
        Size = right.Size;
        End = new Node(true);
        return *this;
    }

    size_t size() const {
        return Size;
    }

    bool empty() const {
        return Size == 0;
    }

    void insert(const T& elem) {
        add(elem);
    }

    void erase(const T& x) {
        if (!contains(x)) {
            return;
        }
        --Size;
        root = dfs_erase(root, x);
    }
private:
    void add(T x) {
        Node *v = new Node(x);
        if (root == nullptr) {
            root = v;
            ++Size;
            return;
        }
        root = addElement(root, v);
    }

    bool contains(T x) {
        return dfs_search(root, x);
    }

    int Size = 0;

    struct Node {
        T key;
        T max;
        int height = 0;
        bool end = false;
        Node *parent = 0, *left = 0, *right = 0;

        Node(T x) {
            key = x;
            height = 0;
            max = key;
        }

        Node(bool _end) {
            end = _end;
        }
    };

public:

    struct iterator {
        Node* v;
        Node* setRoot;
        Node* setEnd;
        iterator() {
            v = nullptr;
        }

        iterator(Node* a, Node* _setRoot, Node* _End) {
            v = a;
            setRoot = _setRoot;
            setEnd = _End;
        }

        Node *SearchMax(Node *start) const {
            if (!start) {
                return setEnd;
            }
            if (!start->right) {
                return start;
            }
            return SearchMax(start->right);
        }

        Node *searchMin(Node *start) const {
            if (!start) {
                return setEnd;
            }
            if (!start->left) {
                return start;
            }
            return searchMin(start->left);
        }

        bool funcEquals(T a, T b) const {
            return (!(a < b) && !(b < a));
        }

        iterator operator++() {
            Node* cur = v;
            if (cur->right) {
                v = searchMin(cur->right);
                return iterator(v, setRoot, setEnd);
            }
            while (cur->parent && cur->parent->right == cur) {
                cur = cur->parent;
            }
            if (cur->parent) {
                v = cur->parent;
                return iterator(v, setRoot, setEnd);
            } else {
                v = setEnd;
                return iterator(v, setRoot, setEnd);
            }
        }

        iterator operator++(int) {
            Node* cur = v;
            if (cur->right) {
                v = searchMin(cur->right);
                return iterator(v, setRoot, setEnd);
            }
            while (cur->parent && cur->parent->right == cur) {
                cur = cur->parent;
            }
            if (cur->parent) {
                v = cur->parent;
                return iterator(v, setRoot, setEnd);
            } else {
                v = setEnd;
                return iterator(v, setRoot, setEnd);
            }
        }

        bool operator==(const iterator& another) const {
            if (v->end == true) {
                return another.v;
            }
            return (v->end == another.v->end && funcEquals(v->key, another.v->key));
        }

        iterator operator--() {
            if (v->end) {
                v = SearchMax(setRoot);
                return iterator(v, setRoot,setEnd);
            }
            Node* cur = v;
            if (cur->left) {
                v = SearchMax(cur->left);
                return iterator(v, setRoot, setEnd);
            }
            while (cur->parent && cur->parent->left == cur) {
                cur = cur->parent;
            }
            v = cur->parent;
            return iterator(v, setRoot, setEnd);
        }

        iterator operator--(int) {
            if (v->end) {
                v = SearchMax(setRoot);
                return iterator(v, setRoot, setEnd);
            }
            Node* cur = v;
            if (cur->left) {
                v = SearchMax(cur->left);
                return iterator(v, setRoot, setEnd);
            }
            while (cur->parent && cur->parent->left == cur) {
                cur = cur->parent;
            }
            v = cur->parent;
            return iterator(v, setRoot, setEnd);
        }

        T operator*() const{
            return v->key;
        }

        T* operator->() const {
            return &v->key;
        }

        bool operator!=(iterator another) const {
            bool answer = false;
            if (v->end == true) {
                answer = another.v;
            } else {
                answer = (v->end == another.v->end && funcEquals(v->key, another.v->key));
            }
            return !answer;
        }
    };

    iterator begin() const {
        return iterator(searchMin(root), root, End);
    }

    iterator end() const {
        return iterator(End, root, End);
    }

    iterator find(const T& elem) const {
        return iterator(findNode(elem), root, End);
    }

    iterator lower_bound(const T& elem) const {
        return iterator(dfs_lower_bound(root, elem), root, End);
    }

private:

    bool funcEquals(T a, T b) const {
        return (!(a < b) && !(b < a));
    }

    int getHeight(Node *v) {
        return (v ? v->height : -1);
    }

    int getDifference(Node *v) {
        if (!v) {
            return -10000;
        }
        return getHeight(v->right) - getHeight(v->left);
    }

    void updateMax(Node* v) {
        if (!v) {
            return;
        }
        v->max = v->key;
        if (v->right) {
            v->max = v->right->max;
        }
    }


    void updateHeight(Node *v) {
        if (!v) {
            return;
        }
        updateMax(v);
        v->height = std::max(getHeight(v->left), getHeight(v->right)) + 1;
    }



    Node *smallLeftRotate(Node *v) {
        Node *a = v;
        Node *P = a->parent;
        Node *p = a->left;
        Node *b = a->right;
        Node *q = b->left;
        Node *r = b->right;
        if (P) {
            if (P->left == a) {
                P->left = b;
            } else {
                P->right = b;
            }
        }
        b->left = a;
        b->right = r;
        b->parent = P;
        a->left = p;
        a->right = q;
        a->parent = b;
        if (p) p->parent = a;
        if (q) q->parent = a;
        if (r) r->parent = b;
        updateHeight(a);
        updateHeight(b);
        return b;
    }


    Node *smallRightRotate(Node *v) {
        Node *a = v;
        Node *P = a->parent;
        Node *b = a->left;
        Node *p = a->right;
        Node *q = b->left;
        Node *r = b->right;
        if (P) {
            if (P->left == a) {
                P->left = b;
            } else {
                P->right = b;
            }
        }
        b->parent = P;
        b->left = q;
        b->right = a;
        if (q) q->parent = b;
        a->parent = b;
        a->left = r;
        a->right = p;
        if (r) r->parent = a;
        if (p) p->parent = a;
        updateHeight(a);
        updateHeight(b);
        return b;
    }

    Node* End = nullptr;
    Node *root = nullptr;

    Node *balance(Node *v) {
        if (!v) return v;
        updateHeight(v);
        if (getDifference(v) == 2) {
            if (getDifference(v->right) < 0) {
                v->right = smallRightRotate(v->right);
                if (v->right) {
                    v->right->parent = v;
                }
            }
            return smallLeftRotate(v);
        }
        if (getDifference(v) == -2) {
            if (getDifference(v->left) > 0) {
                v->left = smallLeftRotate(v->left);
                if (v->left) {
                    v->left->parent = v;
                }
            }
            return smallRightRotate(v);
        }
        return v;
    }

    Node *addElement(Node *v, Node *addVertex) {
        if (funcEquals(v->key, addVertex->key)) {
            delete addVertex;
            return v;
        }
        if (v->key < addVertex->key) {
            if (!v->right) {
                v->right = addVertex;
                addVertex->parent = v;
                updateHeight(v);
                ++Size;
            } else {
                v->right = addElement(v->right, addVertex);
            }
            v = balance(v);
        } else {
            if (!v->left) {
                v->left = addVertex;
                addVertex->parent = v;
                updateHeight(v);
                ++Size;
            } else {
                v->left = addElement(v->left, addVertex);
            }
            v = balance(v);
        }
        return v;
    }

    bool dfs_search(Node *v, T x) {
        if (!v) {
            return false;
        }
        if (funcEquals(v->key, x)) {
            return true;
        }
        if (v->key > x) {
            return dfs_search(v->left, x);
        } else {
            return dfs_search(v->right, x);
        }
    }

    Node *removeMinVertex(Node *v) {
        if (!v) {
            return v;
        }
        if (v->left) {
            v->left = removeMinVertex(v->left);
            if (v->left) {
                v->left->parent = v;
            }
            return balance(v);
        } else {
            Node *P = v->parent;
            Node *u = v->right;
            if (!u) {
                return nullptr;
            }
            if (P) {
                if (P->left == v) {
                    P->left = u;
                }
                if (P->right == v) {
                    P->right = u;
                }
            }
            updateHeight(P);
            u->parent = P;
            return balance(u);
        }
    }

    Node *searchMin(Node *start) const {
        if (!start) {
            return End;
        }
        if (!start->left) {
            return start;
        }
        return searchMin(start->left);
    }

    Node *dfs_erase(Node *v, T x) {
        if (!v) {
            return nullptr;
        }
        if (v->key < x) {
            v->right = dfs_erase(v->right, x);
            if (v->right) {
                v->right->parent = v;
            }
            return balance(v);
        } else if (x < v->key) {
            v->left = dfs_erase(v->left, x);
            if (v->left) {
                v->left->parent = v;
            }
            return balance(v);
        } else {
            Node *P = v->parent;
            Node *left = v->left;
            Node *right = v->right;
            if (!right) {
                if (P) {
                    if (P->left == v) {
                        P->left = left;
                    } else {
                        P->right = left;
                    }
                }
                if (left) left->parent = P;
                updateHeight(P);
                delete v;
                return balance(left);
            } else {
                Node *changeV = searchMin(right);
                changeV->right = removeMinVertex(right);
                if (changeV->right) {
                    changeV->right->parent = changeV;
                }
                changeV->left = left;
                if (changeV->left) {
                    changeV->left->parent = changeV;
                }
                changeV->parent = P;
                if (P) {
                    if (P->left == v) {
                        P->left = changeV;
                    } else {
                        P->right = changeV;
                    }
                }
                updateHeight(P);
                delete v;
                return balance(changeV);
            }
        }
    }

    Node* dfs_lower_bound(Node* v, T x) const {
        if (!v) {
            return end().v;
        }
        if (v->left && (x < v->left->max || funcEquals(x, v->left->max))) {
            return dfs_lower_bound(v->left, x);
        }
        if ((x < v->key || funcEquals(x, v->key))) {
            return v;
        }
        return dfs_lower_bound(v->right, x);
    }

    Node* findNode(T x) const {
        return dfs_find(root, x);
    }

    Node* dfs_find(Node*v, T x) const {
        if (!v) {
            return end().v;
        }
        if (funcEquals(v->key, x)) {
            return v;
        }
        if (v->key < x) {
            return dfs_find(v->right, x);
        } else {
            return dfs_find(v->left, x);
        }
    }


    void eraseAll(Node*v) {
        if (!v) {
            return;
        }
        eraseAll(v->left);
        eraseAll(v->right);
        delete v;
    }

    Node* deepCopy(Node* v) {
        if (!v) {
            return nullptr;
        }
        Node* u = new Node(*v);
        u->left = deepCopy(v->left);
        if (u->left) u->left->parent = u;
        u->right = deepCopy(v->right);
        if (u->right) u->right->parent = u;
        return u;
    }
};