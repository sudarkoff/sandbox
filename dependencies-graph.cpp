/**
* A directed graph represents the dependencies between software packages.
* Write an algorithm for determining the order in which to install the packages.
*
* The solution is based on topological ordering.
*/
#include <cassert>
#include <cstring>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

static const int MAX_NODES=16;

template <typename T>
class graph {
public:
    typedef T value_type;
    typedef std::vector<value_type> nodes_type;
	typedef std::vector<size_t> indices_type;

    graph() {
		std::memset(edges_, 0, sizeof(edges_));
    }
    virtual ~graph() {}

    // inserts new (disconnected) node or return the index of an existing node
    int insert(value_type const& node)
    {
		// check whether the node already exists
		int index = get_node_index(node);
		if (index == -1) {
			// insert if doesn't exist
			nodes_.push_back(node);
			index = nodes_.size() - 1;
		}
		assert(index <= MAX_NODES);
		return index;
	}

    // connects two nodes (inserting the nodes if needed)
    void connect(value_type const& node1, value_type const& node2, int weight = 1)
    {
		int r = insert(node1);
		int c = insert(node2);
        edges_[r][c] = weight;
    }

	void sort(std::vector<value_type>& order)
	{
		// all starting nodes
		indices_type nodes;
		get_start_nodes(nodes);
		if (nodes.empty()) {
			std::cout << "Detected a loop" << std::endl;
			return;
		}

		while (!nodes.empty()) {
			auto start_node = nodes.back(); nodes.pop_back();
			order.push_back(start_node);
			indices_type adjacent;
			get_adjacent_nodes(start_node, adjacent);
			for (auto node : adjacent) {
				// ... Finish implementing toposort
				// 	see http://en.wikipedia.org/wiki/Topological_sorting for details
			}
		}
	}

    // exports the graph to a dot-file
    void export_dot(char const* filename) const
    {
        std::ofstream ofs(filename);
        ofs << "digraph FooBar {" << std::endl 
            << "\tsize = \"8,8\";" << std::endl;
        // output all edges
        for (size_t c = 0; c < nodes_.size(); ++c) {
            for (size_t r = 0; r < nodes_.size(); ++r) {
                if (edges_[r][c] > 0) {
                    ofs << "\t" << nodes_[r] << " -> " << nodes_[c] << ";" << std::endl;
                }
            }
        }
        // output all nodes (some of which might have no connections)
        for (size_t i = 0; i < nodes_.size(); ++i) {
            ofs << "\t" << nodes_[i] << ";" << std::endl;
        }
        ofs << "}" << std::endl;
    }

	void print()
	{
		std::copy(nodes_.begin(), nodes_.end(), std::ostream_iterator<value_type>(std::cout, " "));
		std::cout << std::endl;

        for (size_t c = 0; c < nodes_.size(); ++c) {
            for (size_t r = 0; r < nodes_.size(); ++r) {
                std::cout << edges_[r][c] << " ";
            }
			std::cout << std::endl;
        }
	}

private:
    // returns the index of a give node or -1 if the node doesn't exist
    int get_node_index(value_type const& node) const
    {
		auto pos = std::find(nodes_.begin(), nodes_.end(), node);
		if (pos == nodes_.end()) {
			// node not found
			return -1;
		}

	    return static_cast<int>(pos - nodes_.begin());
    }

	// Returns all nodes with no incoming connections (starting nodes)
	void get_start_nodes(indices_type& indices) const
	{
        bool no_incoming = true;
        for (size_t c = 0; c < nodes_.size(); ++c) {
            no_incoming = true;
            for (size_t r = 0; r < nodes_.size(); ++r) {
                if (edges_[r][c] > 0) {
                    no_incoming = false;
                    break;
                }
            }
            if (no_incoming) {
                indices.push_back(c);
            }
        }
	}

	// Returns all nodes adjacent to the given one
	void get_adjacent_nodes(size_t from, indices_type& adjacent) const
	{
        for (size_t c=0; c < nodes_.size(); ++c) {
            if (edges_[from][c] > 0) {
                adjacent.push_back(c);
            }
        }
	}

private:
	nodes_type nodes_;
	int edges_[MAX_NODES][MAX_NODES];
};

int main(int argc, char* argv[])
{
	graph<char> packages;
	// with the following example, the installation order should be: W I N E R
	packages.connect('R', 'E');
	packages.connect('E', 'I');
	packages.connect('E', 'N');
	packages.connect('E', 'W');
	packages.connect('I', 'W');
	packages.connect('N', 'I');
	packages.export_dot("graph.dot");

	std::vector<char> installation_order;
	packages.sort(installation_order);
	return 0;
}
