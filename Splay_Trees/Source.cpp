//Cây tán loe đẩy dữ liệu nút thường dùng lên thành nút gốc
//Tìm kiếm nhanh hơn độ ưu tiên khác nhau

#include "iostream"
using namespace std;

struct node {
	int data;
	node* left;
	node* right;
};

node* makeNode(int value) {
	node* newNode = new node();
	newNode->data = value;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

void rotateLeft(node*& root) {
	if (root == NULL || root->right == NULL) return;
	node* pointer = root->right;
	root->right = pointer->left;
	pointer->left = root;
	root = pointer;
}

void rotateRight(node*& root) {
	if (root == NULL || root->left == NULL) return;
	node* pointer = root->left;
	root->left = pointer->right;
	pointer->right = root;
	root = pointer;
}

bool splayNode(node*& root, int value) {
	if (root == NULL) return true;
	if (value == root->data) return true;
	if (value < root->data) {
		node* pointer = root->left;
		if (pointer == NULL) return true;
		if (value == pointer->data) { // Zig
			rotateRight(root);
			return true;
		}
		else if (value < pointer->data) {
			node* temp = pointer->left;
			if (temp == NULL) return true;
			if (value == temp->data) { // Zig-Zig
				rotateRight(root);
				rotateRight(root);
				return false;
			}
			else
				return splayNode(root->left, value);
		}
		else {
			node* temp = pointer->right;
			if (temp == NULL) return true;
			if (value == temp->data) { // Zig-Zag
				rotateLeft(root->left);
				rotateRight(root);
				return false;
			}
			else
				return splayNode(root->left, value);
		}
	}
	else {
		node* pointer = root->right;
		if (pointer == NULL) return true;
		if (value == pointer->data) { //Zig
			rotateLeft(root);
			return true;
		}
		else if (value < pointer->data) {
			node* temp = pointer->left;
			if (temp == NULL) return true;
			if (value == temp->data) { //Zig-Zag
				rotateRight(root->right);
				rotateLeft(root);
				return false;
			}
			else
				return splayNode(root->right, value);
		}
		else {
			node* temp = pointer->right;
			if (temp == NULL) return true;
			if (value == temp->data) { //Zig-Zig
				rotateLeft(root);
				rotateLeft(root);
				return false;
			}
			else
				return splayNode(root->right, value);
		}
	}
}

void splayTree(node*& root, int value) {
	while (splayNode(root, value) == false);
}

bool search(node*& root, int key) {
	if (root == NULL) return false;
	node* pointer = root;
	while (pointer != NULL)
	{
		if (key == pointer->data) {
			splayTree(root, key);
			return true;
		}
		else if (key < pointer->data)
			pointer = pointer->left;
		else
			pointer = pointer->right;
	}

	//không tìm thấy->loe node gần node cần tìm
		node* parent = root;
	node* child = NULL;
	while (parent)
	{
		if (key < parent->data) {
			child = parent->left;
			if (child == NULL) {
				splayTree(root, parent->data);
				return false;
			}
		}
		else if (key > parent->data) {
			child = parent->right;
			if (child == NULL) {
				splayTree(root, parent->data);
				return false;
			}
		}
		parent = child;
	}
}

void insert(node*& root, int value)
{
	if (root == NULL) {
		root = makeNode(value);
		return;
	}

	node* pointer = root;
	while (true)
	{
		if (value == pointer->data)
			return;
		else if (value < pointer->data) {
			if (pointer->left == NULL) {
				pointer->left = makeNode(value);
				break;
			}
			else
				pointer = pointer->left;
		}
		else {
			if (pointer->right == NULL) {
				pointer->right = makeNode(value);
				break;
			}
			else
				pointer = pointer->right;
		}
	}
	splayTree(root, value);
}


void deleteNode(node*& root, int key) {
	if (root == NULL) return;

	//Splay nút cần xóa lên gốc cây
	splayTree(root, key);

	//Kiểm tra xem nút cần xóa có tồn tại trong cây không
	if (root->data != key) return;

	node* maxRightOfLeftSubTree = root->left;
	if (maxRightOfLeftSubTree != NULL) {
		while (maxRightOfLeftSubTree->right != NULL)
			maxRightOfLeftSubTree = maxRightOfLeftSubTree->right;

		splayTree(root->left, maxRightOfLeftSubTree->data);

		node* temp = root;
		root = root->left;
		root->right = temp->right;
		delete temp;
	}
	else {
		node* temp = root;
		root = root->right;
		delete temp;
	}
}


void preOrder(node* root) {
	if (root) {
		cout << root->data << " ";
		preOrder(root->left);
		preOrder(root->right);
	}
}

int main() {
	node* root = NULL;
	const int MAX = 10;
	int array[] = { 24,17,11,53,4,13,15,8,14,27 };
	for (int i = 0; i < MAX; i++) {
		insert(root, array[i]);
	}
	cout << "\nPreOrder: ";
	preOrder(root);

	int number;
	cout << "\nEnter number your want to search: ";
	cin >> number;
	if (search(root, number))
		cout << "\nFOUND";
	else
		cout << "\nNOT found";
	cout << "\nPreOrder after search: ";
	preOrder(root);

	cout << "\nEnter number your want to delete: ";
	cin >> number;
	deleteNode(root, number);
	cout << "\nPreOrder after delete: ";
	preOrder(root);

	return 0;
}