// This was the problem I was given on a technical interview at Facebook.
// Print binary tree line by line, separating elements by exactly one space
// and terminating each line with exactlfy one \n.
//
// Ex.:
// 1\n
// 2 4\n
// 5 6 7 8\n
// 10 12\n

#include <iostream>
#include <deque>

#define TERM (static_cast<Node*>(0))

struct Node {
	Node(int val, Node* l=TERM, Node* r=TERM) 
		: value(val), left(l), right(r) {}
	~Node() { delete left; delete right; }

	int value;
	Node* left;
	Node* right;
};

// Using Breadth First Search algorithm with some modifications:
// TERM is used to track the end of line. Each time we reach TERM in the queue
// it means that the last child for the next line was just enqueued, so 
// we insert TERM to mark the end of line.
void print_tree(Node* root)
{
	std::deque<Node*> queue;
	queue.push_back(root);
	queue.push_back(TERM);

	while (!queue.empty()) {
		Node* node = queue.front();
		queue.pop_front();
		if (!node) {
			// We're at the end of the line, print \n
			std::cout << std::endl;
			// If more lines to print, insert end of line terminator
			if (!queue.empty())
				queue.push_back(TERM);
		}
		else {
			// Print the value
			std::cout << node->value;

			// If this is NOT the last element in the line, print space
			Node* next = queue.front();
			if (next)
				std::cout << " ";

			// Enqueue child elements
			if (node->left) queue.push_back(node->left);
			if (node->right) queue.push_back(node->right);
		}
	}
}

int main(int argc, const char* argv[])
{
	Node* root = 
		new Node(1, 
			new Node(2, 
				new Node(5, 
					TERM, 
					new Node(10)), 
				new Node(6)), 
			new Node(4, 
				new Node(7, 
					new Node(12), 
					TERM),
				new Node(8)));

	print_tree(root);

	return 0;
}
