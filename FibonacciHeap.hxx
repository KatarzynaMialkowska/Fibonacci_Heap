#include<iostream>
#include<cmath>
#include <climits>
#include <ctime>
#include <random>

template <class T> 
class FibonacciHeap {
    struct Node {
        T key;
        Node* right; 	// pointer to previous brother (or pointer to self if no brother)
        Node* left; 	// pointer to the next brother (or pointer to yourself if there is no brother)
        Node* child; 	// pointer to any descendant (nullptr in leaves)
        Node* parent; 	// pointer to parent (nullptr in root)
        int degree; 	// vertex degree (number of descendants)
        bool mark; 		// marker, telling if a given node has lost a child since the last moment it became a descendant of another node 
    };
 
	Node* Hmin; 	// pointer to the root of the tree containing the smallest key (minimum node of the heap)
	int Hn; 		// number of nodes

/**
 * Heap H is empty <=> Hmin = nullptr,
 * tree roots are linked by the left and right pointers
 * into two-way circular list
 * the order of the trees on the list can be arbitrary,
 * in the H structure we also remember the number of nodes in the heap (field H.n)
**/

public:
	FibonacciHeap(): Hmin(nullptr), Hn(0) {}
	~FibonacciHeap() { if(Hmin)deleteAll(Hmin);  }

	
	void deleteAll(Node* node) {
		if(node != nullptr) {
			Node* Iterator1 = node;
			do {
				Node* Iterator2 = Iterator1;
				Iterator1 = Iterator2->right;
				deleteAll(Iterator2->child);
				delete Iterator2;
			} while(Iterator1 != node);
		}
	}	
	void deleteAll() {
		while(!empty()){
			pop();
		}
	}
	// insert element into heap and concatenate it into a two-way circular list
	Node* push(T key) {
		Node* node = new Node;
		node->key = key;
		node->parent = nullptr;
		node->child = nullptr;
		node->left = node;
		node->right = node;
		node->degree = 0;
		node->mark = false;
		// node 'node' becomes a single node tree with a heap property (disordered binomial tree in H.F.) 
		Hmin = unionToCircularList(Hmin, node); // connecting a one-element node list with a two-way circular list, root Hmin
		++Hn;
		return node;
	}
	// merge into two-way circular list
	Node* unionToCircularList(Node* H1,Node* H2) {
		if(H1 == nullptr) return H2;
		if(H2 == nullptr) return H1;
		if(H2->key < H1->key) {
			Node* t = H1;
			H1 = H2;
			H2 = t;
		}
		// cyclicality
		Node* t1 = H1->right;
		Node* t2 = H2->left; 
		H1->right = H2;
		H2->left = H1;
		t1->left = t2;
		t2->right = t1;
		return H1;
	}

	// include another heap
	// void unionToCircularList(FibonacciHeap& joining) {
	// Hmin = unionToCircularList(Hmin, joining.Hmin);
	// Hn += joining.Hn;
	// joining.Hmin = nullptr; 
	// }
	
	// if heap H is empty, then Hmin = nullptr
	bool empty() {
		return (Hmin == nullptr);
	}
	// return minimum value
	T top() {
		return Hmin->key;
	}
	// return the number of elements in the heap 
	T size() {
		return Hn;
	}
	// unpinning a node from a two-way circular list 
	void removeFromCircularList(Node* node) {
		if (node->right == node) return; // out if there is one node
		// pinning pointers 
		Node* leftNode = node->left; 
		Node* rightNode = node->right;	
		leftNode->right = rightNode;
		rightNode->left = leftNode;	
	}
	// unpinning nodes in a leaf from parents
	void unparent(Node* node) {
		if(node == nullptr) return;
		Node* Current = node;
		do {
			node->parent = nullptr;
			node = node->right;
		}while(Current != node);
	}
	// remove the node with the smallest key and order the Fibonacci heap
	Node* extractMin() {
		Node* min = Hmin;
		if (min != nullptr) {
			unparent(min->child); // unpinning list from parents
			unionToCircularList(min, min->child); // merge children into a circular list of roots
			removeFromCircularList(min); // remove minimum from circular list
			if (min == min->right) 
				Hmin = nullptr;
			else {
				Hmin = min->right; // the minimum pointer to the right side
				consolidate();
			}
			Hn--;
		}
		return min;
	}

/**
 * Each node has a degree not greater than O (log n)
 * and the size of the subtree rooted in the node of degree k
 * is at least Fk + 2,
 * where Fk is the kth Fibonacci number.
 * 
 * Golden ratio, i.e. the quotient of adjacent Fibonacci sequence terms
 **/

	void consolidate() {
		bool DiffDegree = false;
		int Dn = (int)( log2(Hn) / log2(1.618) ) ;// log b a = log a / log b // the maximum degree of node
		// Golden ratio 1,61803...
		Node ** A;
		A = new Node* [Dn+1]; // an auxiliary pointer  array of the size of the maximum degree of the tree in the Fib heap
		for (int i = 0; i < Dn+1; i++) A[i] = nullptr; // array points to nullptr
		Node* x = Hmin;
		while(true) { // until degrees are the same
			int d = x->degree;
			while( A[d] != nullptr) { // if exist a tree of a given degree
				Node* y = A[d]; 
				if (y == x) {
					DiffDegree = true;  // if y == x, then all the nodes in the roots have a different degree, so we exit the loop
					break;
				}					
				if (x->key > y->key) { // replace if x is the greater key. 
					Node* t = x;
					x = y;
					y = t;				
				}
				fibHeapLink(y, x); // node y becomes the child of node x 
				A[d] = nullptr; // reset the degree of the given array and go to the next degree
				++d;
			}
			if (DiffDegree == true) // if all the nodes are of a different degree 
				break;
			A[x->degree] = x;
			x = x->right; // go to next node
		}
		// update Hmin by traversing the circular array
		Hmin = x; 
		Node* Iterator = x;
		do {
			if(Iterator->key < Hmin->key) 
				Hmin = Iterator;
			Iterator = Iterator->right;
		}while(Iterator != x);
		delete []A;
	}

/**
 * fibHeapLink()
 * removes from the list of H roots
 * make y the son of x and increase degree [x] by 1
 * mark[y]=FALSE 
 * fibHeapLink(Node *child = y , Node *parent = x)
 * */
	void fibHeapLink(Node *child, Node *parent) {
		removeFromCircularList(child);
		child->left = child->right = child;
		child->parent = parent;
		parent->child = unionToCircularList(parent->child, child); // add a child to the parent's cyclic list of children
		parent->degree++;
		child->mark = false;
	}
	// runs a function that finds the value of the smallest key and deletes it 
	T pop() {
		if(empty()) throw std::out_of_range("empty");
		Node* minNode = extractMin();
		int result = minNode->key;
		delete minNode;
		return std::move(result);
	}

	// returns a pointer to the node searched for with the given key
 	Node* find(T key) {
		return find(Hmin, key);
	}
	Node* find(Node* node, T key) {
		Node* Current = node;
		if(Current == nullptr)return nullptr;
		do {
			if(Current->key == key)return Current;
			Node* nextLeTel = find(Current->child, key);
			if(nextLeTel != nullptr)return nextLeTel;
			Current = Current->right;
		}while(Current != node);
		return nullptr;
	}
/**
 * decreaseKey() - decrease the key value.
 * Checks that the new key value is not greater than the current key in node x, then store the new key in x.
 * If x is the root or key [x]> = key [y], where y is the father of x, 
 * there is no need to change the heap structure since the heap order has not been tampered with.
 * */
	void decreaseKey(Node* x, T newKey) {
		if(x->key > newKey) {
			x->key = newKey; // passign a new value
		Node* y = x->parent; // take parent
		if (y != nullptr && x->key < y->key) {// the new value is less than the parent 
			cut(x, y);
			cascadingCut(y);
		}
		if (x->key < Hmin->key) 
			Hmin = x;
		}else std::cout<<"ERROR: nowa wartosc klucza jest wieksza niz biezaca\n";
	}
	void decreaseKey(T key, T newKey) {
		Node* node = find(key);
		if(node!=nullptr)
		decreaseKey(node, newKey);
		else std::cout<<"klucz nie istnieje\n";
	}
	// remove the given key by reducing its value to INT_MIN and removing the smallest node 
	void remove(T key) {
		decreaseKey(key, INT_MIN);
		pop();
	}
	

/**
 * cut()
 * Breaks the link between x and his father y, making x the root.
 *  Continues cut () (aka cascadingCut ()) from the reduced node to the parent
 * until it is root or is not marked (mark)
 * */
	void cut(Node* x, Node* y) {
		removeFromCircularList(x); // unpinned from the circular list
		if (x->right == x) // if there is the only child, the parent does not point anything
			y->child = nullptr;
		else
			y->child = x->right; // parent point another child
		y->degree--; // reduces the parent's degree
		x->left = x->right = x;
		unionToCircularList(Hmin, x); // sets to the root
		x->parent = nullptr;
		x->mark = false;
	}
	void cascadingCut(Node* y) {
		Node* z = y->parent;
		if (z != nullptr) {
			if(y->mark == false)
				y->mark = true;		
			else {
				cut(y,z);
				cascadingCut(z);
			}
		}
	}
	// graphical display of the heap in the terminal 
 	void display() const {
		Node* node = Hmin;
		if (node == nullptr)
			return;
		displayTree(Hmin, "");
		std::cout << '\n';
	}
	void displayTree(Node* node, std::string pre) const {
		std::string p = "│  ";
		Node* x = node;
		do {
			if (x->left != node) {
				std::cout << pre << "├─";
			} else {
				std::cout << pre << "└─";
				p = "   ";
			}
			if (x->child == nullptr) {
				std::cout << "─" << x->key << '\n';
			} else {
				std::cout << "┐" << x->key << '\n';
				displayTree(x->child, pre + p);
			}
			x = x->left;
		}while (x != node);
	}
};

// generates a random heap 
template <class T>
void generator(FibonacciHeap<T> &heap, const int& n, const T& min,const T& max){
    std::uniform_int_distribution<T> dist1 ( min, max );
    std::mt19937 gen ( time ( NULL ) );
    
    for( int i = 0; i < n; i++ )
        heap.push(dist1(gen));
    
}