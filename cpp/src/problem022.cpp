/**
Using names.txt (right click and 'Save Link/Target As...'), a 46K text
file containing over five-thousand first names, begin by sorting it
into alphabetical order. Then working out the alphabetical value for each name,
multiply this value by its alphabetical position in the list to
obtain a name score.

For example, when the list is sorted into alphabetical order, COLIN,
which is worth 3 + 15 + 12 + 9 + 14 = 53, is the 938th name in the list.
So, COLIN would obtain a score of 938 × 53 = 49714.

What is the total of all the name scores in the file?
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream> /* File operations. */
#include <string> /* C++ String class. */

/* STL Headers */
#include <algorithm>

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using std::string;

/***************** Constants & Macros *********************/
const char * const INPUT = "./src/input_e022.txt";

/************** Global Vars & Functions *******************/
int score_name(const int pos, const std::string &name) {
    int score = 0;
    char base = 'a' - 1;

    for (std::string::const_iterator itr = name.begin(); itr != name.end(); ++itr) {
        score += std::tolower(*itr) - base;
    }

    return pos * score;
}

TEST(Euler022, ScoreName) {
    std::string name = "COLIN";

    ASSERT_EQ(49714, score_name(938, name));
}

TEST(Euler022, FindAnswer) {
    std::ifstream input(INPUT);
    std::string name;
    std::vector<std::string> names;

    while (std::getline(input, name, ',')) {
        name.erase(0, 1);
        name.erase(name.size() -1, 1);
        names.push_back(name);
    }

    /* Sort list */
    std::sort(names.begin(), names.end());

    int pos = 1;
    unsigned long sum = 0;
    for (std::vector<std::string>::const_iterator itr = names.begin(); itr != names.end(); ++itr) {
        sum += score_name(pos++, *itr);
    }

    cout << "The sum of the scores is: " << sum << endl;
}

