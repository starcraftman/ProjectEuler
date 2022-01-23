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
static const std::string INPUT = "./src/input_e081.txt";
static const std::string INPUT_SMALL = "./src/input_e081.small.txt";

class Node;
class Node {
public:
    explicit Node(int value = 0) : value(value) {}

    friend std::ostream & operator<<(std::ostream& os, const Node &node);

    // Data
    int value = 0;
    Node *right = NULL;
    Node *down = NULL;
};
typedef std::vector<std::vector<Node> > vec_node_t;
typedef std::map<const Node*, int> memo_t;

std::ostream & operator<<(std::ostream& os, const Node &node) {
    int right = 0;
    int down = 0;
    if (node.right != NULL) {
        right = node.right->value;
    }
    if (node.down != NULL) {
        down = node.down->value;
    }
    os << "Node: " << node.value << " > " << right << " d " << down;

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
        const std::size_t last_row = row - 1;

        while (col != nodes[row].size()) {
            if (col != last_col_ind) {
                nodes[row][col].right = &nodes[row][col + 1];
            }
            if (row < last_row_ind) {
                nodes[row][col].down = &nodes[row + 1][col];
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

int explore_path(const Node &node, memo_t &memo) {
    if (memo[&node] != 0) {
        return memo[&node];
    }

    if (node.right == NULL && node.down == NULL) {
        return node.value;
    }

    int min_got = 0;
    if (node.right != NULL && node.down == NULL) {
        min_got = explore_path(*node.right, memo);
    } else if (node.right == NULL && node.down != NULL) {
        min_got = explore_path(*node.down, memo);
    } else {
        min_got = std::min(
            explore_path(*node.right, memo),
            explore_path(*node.down, memo)
        );
    }

    memo[&node] = node.value + min_got;
    return memo[&node];
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

TEST(Euler081, PrintMatrix) {
    vec_node_t nodes;
    read_all_nodes(std::string(INPUT_SMALL), nodes);
    connect_nodes(nodes);
    std::stringstream ss;
    print_matrix(ss, nodes);
    std::string expect_found = "0131 0673 0234 0103 0018";
    ASSERT_TRUE(ss.str().find(expect_found) != std::string::npos);
}

TEST(Euler081, ConnectRows) {
    vec_node_t nodes;
    read_all_nodes(std::string(INPUT), nodes);
    connect_nodes(nodes);

    auto first = nodes.front().front();
    ASSERT_EQ(first.right->value, 2697);
    ASSERT_EQ(first.down->value, 1096);
}

TEST(Euler081, MinSumTwoWays) {
    vec_node_t nodes;
    read_all_nodes(std::string(INPUT), nodes);
    connect_nodes(nodes);
    const Node &root = nodes.front().front();
    memo_t memo;
    int min_sum = explore_path(root, memo);

    ASSERT_EQ(min_sum, 427337);
    cout << "The sum of the path taken is " << min_sum << endl;
}
