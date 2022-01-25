/**
In the 5 by 5 matrix below, the minimal path sum from the top left to the bottom right,
by only moving to the right and down, is indicated in bold red and is equal to 2427.
Find the minimal path sum from the top left to the bottom right by only moving right and down
in matrix.txt (right click and "Save Link/Target As..."), a 31K text file containing an 80 by 80 matrix.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <iomanip>
#include <fstream>
#include <sstream>
#include <exception>
#include <initializer_list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <climits>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
static const std::string INPUT = "./src/input_e081.txt";
static const std::string INPUT_SMALL = "./src/input_e081.small.txt";

class Node;
class Node {
public:
    explicit Node(int value = 0) : value(value) {}

    friend std::ostream & operator<<(std::ostream& os, const Node &node);

    // Data
    int value = 0;
    int sum_value = 0;
    bool visited = false;
    bool selected = false;
    Node *up = NULL;
    Node *right = NULL;
    Node *down = NULL;
    Node *left = NULL;
};
typedef std::vector<std::vector<Node> > vec_node_t;
typedef std::map<const Node*, int> memo_t;

std::ostream & operator<<(std::ostream& os, const Node &node) {
    int up = 0;
    int right = 0;
    int down = 0;
    int left = 0;
    if (node.up != NULL) {
        up = node.up->value;
    }
    if (node.right != NULL) {
        right = node.right->value;
    }
    if (node.down != NULL) {
        down = node.down->value;
    }
    if (node.left != NULL) {
        left = node.left->value;
    }
    os << "Node: " << node.value << "(" << up << ", " << right << ", " << down << ", " << left << ")";

    return os;
}

/*
 * Replace all for a string. Input is csv limited, so convert to space delimited.
 */
std::string replace_all(std::string &original,
        const std::string &what, const std::string &replacement) {
    std::size_t pos = std::string::npos;
    pos = original.find(what);
    while (pos != std::string::npos) {
        original.replace(pos, what.length(), replacement);
        pos = original.find(what, pos);
    }

    return original;
}

/*
 * Read in the value of a line of nodes.
 */
void read_line_nodes(const std::string &line, vec_node_t &nodes) {
    std::stringstream ss(line);
    std::vector<Node> row;
    while (ss) {
        Node node;
        ss >> node.value;
        if (node.value) {
            row.push_back(node);
        }
    }

    nodes.push_back(row);
}

/*
 * Read all nodes from file into nodes.
 */
void read_all_nodes(const std::string &fname, vec_node_t &nodes) {
    std::ifstream input(fname, std::ifstream::in);
    std::string line;
    while (std::getline(input, line)) {
        read_line_nodes(replace_all(line, ",", " "), nodes);
    }
}

/*
 * Connect all points to the right and down in the nodes.
 * Treat the operation as using index positions.
 */
void connect_nodes(vec_node_t &nodes) {
    std::size_t row = 0;
    const std::size_t last_row_ind = nodes.size() - 1;
    while (row != nodes.size()) {
        const std::size_t last_col_ind = nodes[row].size() - 1;
        std::size_t col = 0;

        while (col != nodes[row].size()) {
            if (col < last_col_ind) {
                nodes[row][col].right = &nodes[row][col + 1];
            }
            if (row < last_row_ind) {
                nodes[row][col].down = &nodes[row + 1][col];
            }
            if (row >= 1) {
                nodes[row][col].up = &nodes[row - 1][col];
            }
            if (col >= 1) {
                nodes[row][col].left = &nodes[row][col - 1];
            }

            ++col;
        }

        ++row;
    }
}

/*
 * Debug only, prints whole matrix of nodes.
 */
void print_matrix(std::ostream &os, const vec_node_t &nodes, bool pointers = false) {
    for (auto row : nodes) {
        for (auto node: row) {
            os << std::setfill('0') << std::setw(4);
            if (pointers) {
                os << node;
            } else {
                os << node.value;
            }
            os << " ";
        }
        os << endl;
    }
}

// N.B. This is a slow implementation modified quickly from previous, see explore_path2 for almost linear solution.
void explore_path(Node &node, int sum_so_far, memo_t &memo) {
    // Always record the cost to get to current node.
    // If the cost to get to current is HIGHER than recorded, abort route.
    const int current_sum = sum_so_far + node.value;
    if (!memo[&node]) {
        memo[&node] = current_sum;
    } else {
        if (current_sum < memo[&node]) {
            memo[&node] = current_sum;
        } else {
            return;
        }
    }

    // Stop condition, reached right, we are at a solution.
    if (node.right == NULL && node.down == NULL) {
        return;
    }

    node.visited = true;
    for (Node * next_node : {node.up, node.right, node.down, node.left}) {
        if (next_node != NULL && next_node->visited == false) {
            explore_path(*next_node, current_sum, memo);
        }
    }
    node.visited = false;
}

// Push BACK new values IF they are less than existing sum_value.
// Keep recursing until there is no improvement in sum_value
void recurse_push(Node *node, Node *origin) {
    for (Node *next_node : {node->up, node->right, node->down, node->left}) {
        if (next_node == NULL || next_node == origin) {
            continue;
        }
        int new_sum = next_node->value + node->sum_value;
        if (next_node->sum_value > new_sum) {
            next_node->sum_value = new_sum;
            recurse_push(next_node, node);
        }
    }
}

void explore_path2(Node &root) {
    std::deque<Node *> todo;
    root.selected = true;
    root.sum_value = root.value;
    todo.push_back(&root);
    Node *node;

    while (!todo.empty()) {
        node = todo.front();
        todo.pop_front();

        for (Node *next_node : {node->up, node->right, node->down, node->left}) {
            if (next_node == NULL) {
                continue;
            }

            int sum_to_next = node->sum_value + next_node->value;
            if (next_node->sum_value == 0) {
                next_node->sum_value = sum_to_next;
            } else if (next_node->sum_value > sum_to_next) {
                next_node->sum_value = sum_to_next;
                recurse_push(next_node, node);
            }

            if (!next_node->selected) {
                next_node->selected = true;
                todo.push_back(next_node);
            }
        }
    }
}


TEST(Euler081, ReplaceAll) {
    std::string input("4445,2697,5115,718,2209,2212");
    std::string expect("4445 2697 5115 718 2209 2212");
    ASSERT_EQ(replace_all(input, ",", " "), expect);
}

TEST(Euler081, ReadLineNodes) {
    vec_node_t nodes;
    read_line_nodes(std::string("33 22 11"), nodes);
    ASSERT_EQ(nodes.front().front().value, 33);
    ASSERT_EQ(nodes.back().back().value, 11);
}

TEST(Euler081, ReadAllNodes) {
    vec_node_t nodes;
    read_all_nodes(std::string(INPUT), nodes);
    ASSERT_EQ(nodes.front().front().value, 4445);
    ASSERT_EQ(nodes.back().back().value, 7981);
}

TEST(Euler081, ConnectRows) {
    vec_node_t nodes;
    read_all_nodes(std::string(INPUT), nodes);
    connect_nodes(nodes);

    auto node = nodes[1].at(1);
    ASSERT_EQ(node.up->value, 2697);
    ASSERT_EQ(node.right->value, 1318);
    ASSERT_EQ(node.down->value, 7385);
    ASSERT_EQ(node.left->value, 1096);
}

TEST(Euler081, PrintMatrix) {
    vec_node_t nodes;
    read_all_nodes(std::string(INPUT_SMALL), nodes);
    connect_nodes(nodes);
    std::stringstream ss;
    print_matrix(ss, nodes, true);
    std::string expected_found = "Node: 201(131, 96, 630, 0) Node: 96(673, 342, 803, 201) Node: 342(234, 965, 746, 96) Node: 965(103, 150, 422, 342) Node: 150(18, 0, 111, 965)";
    ASSERT_TRUE(ss.str().find(expected_found) != std::string::npos);
}

TEST(Euler081, MinSumFourWays) {
    vec_node_t nodes;
    read_all_nodes(std::string(INPUT), nodes);
    connect_nodes(nodes);

    Node &root = nodes.front().front();
    explore_path2(root);

    Node &end = nodes.back().back();
    cout << "Best path to node: " << end.value << " is " << end.sum_value << endl;
    ASSERT_EQ(end.sum_value, 425185);
}

// Naieve implementation left
// TEST(Euler081, MinSumFourWays) {
    // vec_node_t nodes;
    // read_all_nodes(std::string(INPUT), nodes);
    // connect_nodes(nodes);

    // memo_t memo;
    // Node &root = nodes.front().front();
    // explore_path(root, 0, memo);

    // Node &end = nodes.back().back();
    // cout << "Best path to node: " << end.value << " is " << memo[&end] << endl;
    // ASSERT_EQ(memo[&end], 425185);
// }
