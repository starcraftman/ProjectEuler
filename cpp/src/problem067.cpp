/**
In the 5 by 5 matrix below, the minimal path sum from the top left to the bottom right,
by only moving to the right and down, is indicated in bold red and is equal to 2427.
Find the minimal path sum from the top left to the bottom right by only moving right and down
in matrix.txt (right click and "Save Link/Target As..."), a 31K text file containing an 80 by 80 matrix.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream>
#include <sstream>
#include <exception>
#include <initializer_list>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
static const std::string INPUT = "./src/input_e067.txt";

class Node;
class Node {
public:
    explicit Node(int value = 0) : value(value) {}

    friend std::ostream & operator<<(std::ostream& os, const Node &node);

    // Data
    int value = 0;
    Node *left = NULL;
    Node *right = NULL;
};
// 2d node holder while linking
typedef std::vector<std::vector<Node> > vec_node_t;

std::ostream & operator<<(std::ostream &os, const Node &node) {
    int left = 0;
    int right = 0;
    if (node.left != NULL) {
        left = node.left->value;
    }
    if (node.right != NULL) {
        right = node.right->value;
    }
    os << "Node: " << node.value
        << " L(" << left << ") R(" << right << ")";

    return os;
}
/*
 * Debug only, prints whole matrix of nodes.
 */
void print_nodes(const vec_node_t &all_nodes) {
    for (auto row : all_nodes) {
        for (auto node : row) {
            cout << node << endl;
        }
    }
}

/*
 * Read in the value of a node and connect it right and down if required.
 */
void read_nodes(const std::string &line, vec_node_t &nodes) {
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
 * Ensure connections setup too.
 */
void read_in_nodes(vec_node_t &nodes) {
    std::ifstream input(INPUT, std::ifstream::in);
    std::string line;
    while (std::getline(input, line)) {
        read_nodes(line, nodes);
    }
}

void connect_paths(vec_node_t &nodes) {
    auto cur_row = nodes.begin();
    auto next_row = nodes.begin();

    // next_row always pointing at one row beyond cur_row
    while (++next_row != nodes.end()) {
        int cnt = 0;
        for (Node &node: *cur_row) {
            node.left = &next_row->at(cnt);
            node.right = &next_row->at(cnt + 1);
            cnt++;
        }

        cur_row++;
    }
}

// Naieve implementation
int check_nodes_recursion(const Node &node) {
    if (node.left == NULL && node.right == NULL) {
        return node.value;
    }

    return node.value + std::max(check_nodes_recursion(*node.left),
                                 check_nodes_recursion(*node.right));
}

// Memo implementation
int check_nodes_memo(const Node &node, std::map<const Node*, int> &memo) {
    if (memo[&node] != 0) {
        return memo[&node];
    }

    if (node.left == NULL && node.right == NULL) {
        return node.value;
    }

    memo[&node] = node.value + std::max(check_nodes_memo(*node.left, memo),
                                        check_nodes_memo(*node.right, memo));
    return memo[&node];
}

TEST(Euler067, ReadNodes) {
    vec_node_t nodes;
    read_nodes(std::string("33 22 11"), nodes);
    ASSERT_EQ(nodes.front().front().value, 33);
    ASSERT_EQ(nodes.back().back().value, 11);
}

TEST(Euler067, ReadInNodes) {
    vec_node_t nodes;
    read_in_nodes(nodes);
    ASSERT_EQ(nodes.front().front().value, 59);
    ASSERT_EQ(nodes.back().back().value, 35);
}

TEST(Euler067, ConnectLinks) {
    vec_node_t nodes;
    read_in_nodes(nodes);
    connect_paths(nodes);

    Node front = nodes.front().front();
    ASSERT_EQ(front.left->value, 73);
    ASSERT_EQ(front.right->value, 41);
}

// Runs too slow.
// TEST(Euler067, CheckNodesRecursion) {
    // vec_node_t nodes;
    // read_in_nodes(nodes);
    // connect_paths(nodes);
    // const Node &node = nodes.front().front();

    // int max_total = check_nodes_recursion(node);
    // cout << "The max value found going down: " << max_total << endl;
    // ASSERT_EQ(max_total, 1074);
// }

TEST(Euler067, CheckNodesMemo) {
    vec_node_t nodes;
    read_in_nodes(nodes);
    connect_paths(nodes);
    const Node &node = nodes.front().front();
    std::map<const Node*, int> memo;

    int max_total = check_nodes_memo(node, memo);
    cout << "The max value found going down: " << max_total << endl;
    ASSERT_EQ(max_total, 7273);
}
