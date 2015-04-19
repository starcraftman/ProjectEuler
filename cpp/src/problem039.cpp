/**
If p is the perimeter of a right angle triangle with integral length sides,
{a,b,c}, there are exactly three solutions for p = 120.

{20,48,52}, {24,45,51}, {30,40,50}

For which value of p ≤ 1000, is the number of solutions maximised?

Note: This problem is just an alias to which number under 1000 generates
most pythagorean triplets.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <vector>

#include "gtest/gtest.h"
#include "boost/assign.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using std::string;

/************** Global Vars & Functions *******************/
class Triplet {
public:
    Triplet(int a, int b, int c) : a(a), b(b), c(c) {};
    int a;
    int b;
    int c;
};

bool is_triplet(int a, int b, int c) {
    return (a * a + b * b) == c * c;
}

std::vector<Triplet> search_triplets(int p) {
    std::vector<Triplet> trips;

    // Search for all possible = p here

    return trips;
}

TEST(Euler039, Triplet) {
    ASSERT_TRUE(is_triplet(20, 48, 52));
    ASSERT_TRUE(is_triplet(30, 40, 50));
}

TEST(Euler039, FindTriplets) {
    std::vector<Triplet> expected;
}
