/**
 * Implementation of the Dijkstra's algorithm for finding the shortest path in a graph.
 * http://en.wikipedia.org/wiki/Dijkstra's_algorithm
 */

#include <cassert>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>
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

    // exports the graph to a dot-file
    void export_dot(char const* filename) const
    {
        std::ofstream ofs(filename);
        ofs << "digraph D {\n"
			<< "\trankdir=LR\n"
			<< "\tsize=\"4,3\"\n"
			<< "\tratio=\"fill\"\n"
			<< "\tedge[style=\"bold\"]\n"
			<< "\tnode[shape=\"circle\"]\n"
            << "\tsize = \"8,8\";\n";
        // output all edges
        for (size_t c = 0; c < nodes_.size(); ++c) {
            for (size_t r = 0; r < nodes_.size(); ++r) {
                if (edges_[r][c] > 0) {
                    ofs << "\t" << nodes_[r] << " -> " << nodes_[c]
						<< " [label=" << edges_[r][c]
						<< ",color=" << ((r==c)?"red": "black") << "];\n";
                }
            }
        }
        // output all nodes (some of which might have no connections)
        for (size_t i = 0; i < nodes_.size(); ++i) {
            ofs << "\t" << nodes_[i] << ";" << std::endl;
        }
        ofs << "}" << std::endl;
    }

	void shortest_path(value_type&& A, value_type&& B, nodes_type& path)
	{
		// TODO: Implement
	}

private:
    // returns the index of a give node, -1 if the node doesn't exist
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
	graph<char> cities;
	// The following example should output: R E I W
	cities.connect('R', 'E', 5);
	cities.connect('E', 'I', 3);
	cities.connect('E', 'N', 1);
	cities.connect('E', 'W', 12);
	cities.connect('I', 'W', 6);
	cities.connect('N', 'I', 4);
	cities.export_dot("graph.dot");

	std::vector<char> path;
	cities.shortest_path('R', 'W', path);
	std::copy(path.begin(), path.end(), std::ostream_iterator<char>(std::cout, " "));

	return 0;
}
