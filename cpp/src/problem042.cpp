/**
The nth term of the sequence of triangle numbers is given by, tn = ½n(n+1); so the first ten triangle numbers are:

1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...

By converting each letter in a word to a number corresponding to its alphabetical position and adding these values we form a word value. For example, the word value for SKY is 19 + 11 + 25 = 55 = t10. If the word value is a triangle number then we shall call the word a triangle word.

Using words.txt (right click and 'Save Link/Target As...'), a 16K text file containing nearly two-thousand common English words, how many are triangle words?
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
#include <climits>

#include "boost/algorithm/string.hpp"

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
static const std::string INPUT = "./src/input_e042.txt";
typedef int num_t;
static const num_t BEFORE_A_VAL = int('A') - 1;
static const num_t MAX_TRIANGLE_VALUE = 5000;

void generate_triangles(int below_value, std::set<num_t> &triangles) {
    num_t index = 0;
    num_t current = 0;

    do {
        index++;
        current = (index * (index + 1)) / 2;
        triangles.insert(current);
    } while (current < below_value);
}

inline
num_t word_to_sum(const std::string &word) {
    num_t value = 0;
    // Use to modify case
    // const std::string lower = boost::algorithm::to_upper_copy(word);

    for (auto chr: word) {
        value += int(chr) - BEFORE_A_VAL;
    }

    return value;
}

std::string clean_line(const std::string &line) {
    std::string new_line = line;

    std::replace(new_line.begin(), new_line.end(), '"', ' ');
    std::replace(new_line.begin(), new_line.end(), ',', ' ');
    auto itr = new_line.begin();
    while (itr != new_line.end()) {
        if (*itr == ' ') {
            itr = new_line.erase(itr);
        } else {
            break;
        }
    }

    return new_line;
}

num_t number_of_triangles_in_input() {
    std::set<num_t> triangles;
    generate_triangles(MAX_TRIANGLE_VALUE, triangles);

    std::string line;
    std::ifstream fin(INPUT);
    std::getline(fin, line);
    std::stringstream ss(clean_line(line));

    num_t cnt = 0;
    std::string word;
    while (ss.good()) {
        ss >> word;
        num_t sum = word_to_sum(word);
        if (triangles.count(sum) != 0) {
            cnt++;
        }
    }

    return cnt;
}

TEST(Euler042, GenerateTriangles) {
    std::set<num_t> triangles;
    generate_triangles(5000, triangles);

    ASSERT_EQ(triangles.count(1035), 1);
}

TEST(Euler042, WordToSum) {
    std::string word("SKY");
    ASSERT_EQ(word_to_sum(word), 55);
}


TEST(Euler042, CleanLine) {
    std::string line("\"A\",\"ABILITY\",\"ABLE\",\"ABOUT\",\"ABOVE\"");
    std::string expect("A   ABILITY   ABLE   ABOUT   ABOVE ");

    ASSERT_EQ(clean_line(line), expect);
}

TEST(Euler042, StreamGenerator) {
    std::string line("\"A\",\"ABILITY\",\"ABLE\",\"ABOUT\",\"ABOVE\"");
    std::stringstream ss(clean_line(line));
    std::string expect = "ABILITY";
    std::string word;
    ss >> word;
    ss >> word;

    ASSERT_EQ(word, expect);
}

TEST(Euler042, NumberOfTrianglesInInput) {
    num_t cnt = number_of_triangles_in_input();
    ASSERT_EQ(cnt, 162);
    cout << "The number of triangles in input: " << cnt << endl;
}
