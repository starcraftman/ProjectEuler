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
static const std::string INPUT = "./src/input_e081.txt";

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
Node nodes[80][80];

std::ostream & operator<<(std::ostream& os, const Node &node) {
    int right = 0;
    int down = 0;
    if (node.right != NULL) {
        right = node.right->value;
    }
    if (node.down != NULL) {
        down = node.down->value;
    }
    os << "Node: " << node.value << " > "
        << right << " d " << down;

    return os;
}

/*
 * Read in the value of a node and connect it right and down if required.
 */
void connect_row(int row, const std::string &line) {
    std::stringstream ss(line);
    int col = -1;
    // Starts at col == 1, so always one node ahead.
    while (++col != 80) {
        ss >> nodes[row][col].value;
        if (col < 79) {
            nodes[row][col].right = & nodes[row][col + 1];
        }
        if (row < 79) {
            nodes[row][col].down = & nodes[row + 1][col];
        }
    }
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
 * Read all nodes from matrix file into nodes array.
 * Ensure connections setup too.
 */
void read_in_nodes() {
    std::ifstream input(INPUT, std::ifstream::in);
    std::string line;
    int row = 0;
    while (std::getline(input, line)) {
        connect_row(row++, replace_all(line, ",", " "));
    }
}

/*
 * Debug only, prints whole matrix of nodes.
 */
void print_matrix() {
    for (int row = 0; row < 80; ++row) {
        for (int col = 0; col < 80; ++col) {
            cout << nodes[row][col] << endl;
        }
        break;
    }
}

/*
 * Simple tracker for sum and taken nodes.
 */
class PathTrack {
public:
    void add(const Node &node) {
        this->nodes.push_back(node.value);
    };
    int sum() {
        return std::accumulate(nodes.cbegin(), nodes.cend(), 0);
    }
    std::vector<int> nodes;
};

PathTrack sum_two_ways_path() {
    read_in_nodes();
    // Start in top left and go down or right, min value
    PathTrack track;
    Node *current = &nodes[0][0];
    track.add(*current);

    while (current->down != NULL || current->right != NULL) {
        if (current->down == NULL) {
            current = current->right;
        } else if (current->right == NULL) {
            current = current->down;
        } else {
            if (current->right->value <= current->down->value) {
                current = current->right;
            } else {
                current = current->down;
            }
        }
        track.add(*current);
    }
    // Must be at bottom right node, add last.

    return track;
}

TEST(Euler081, ReplaceAll) {
    std::string input("4445,2697,5115,718,2209,2212");
    std::string expect("4445 2697 5115 718 2209 2212");
    ASSERT_EQ(replace_all(input, ",", " "), expect);
}

TEST(Euler081, ConnectRow) {
    read_in_nodes();
    ASSERT_EQ(nodes[0][79].value, 5870);
    ASSERT_EQ(nodes[0][79].down->value, 9377);
}

// TODO: Initial naive solution incorrect, will have to explore all paths to find lowest possible.
TEST(Euler054, NumberWonHands) {
    PathTrack tracker = sum_two_ways_path();
    ASSERT_EQ(tracker.sum(), 548877);
    cout << "The sum of the path taken is " << tracker.sum() << endl;

    cout << "Path taken through nodes:" << endl;
    for (auto val: tracker.nodes) {
        cout << val << " ";
    }
    cout << endl;
}
