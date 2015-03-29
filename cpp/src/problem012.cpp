/**
 * Find first triangle number with over 500 divisors.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */

/* STL Headers */
#include <set>

/* C Headers */
#include <cstdlib>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/****************** Class Definitions *********************/
class TriangleGenerator {
public:
    TriangleGenerator() {
        this->t_add = 1;
        this->t_number = 0;
    }
    virtual	~TriangleGenerator() {};

    inline TriangleGenerator & operator=(TriangleGenerator other) {
        swap(*this, other);
        return *this;
    }
    void swap(TriangleGenerator &first, TriangleGenerator &second) {
        using std::swap;
        swap(first.t_add, second.t_add);
        swap(first.t_number, second.t_number);
    }

    long next() {
        this->t_number += t_add;
        this->t_add += 1;
        return this->t_number;
    }

    long index() {
        return this->t_add - 1;
    }

    long number() {
        return this->t_number;
    }

private:
    long t_add;
    long t_number;
};

/************** Global Vars & Functions *******************/
int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: " << *argv << " <divisors>" << endl;
        cout << "Where <divisors> is the number of divisors the triangle should have." << endl;
        return 1;
    }

    ++argv;
    unsigned long divisors = std::atol(*argv);

    TriangleGenerator tg;
    std::set<long> divs;
    while (true) {
        tg.next();
        cout << "Triangle number " << tg.index() << " is " << tg.number() << "." << endl;
        divs = util::find_divisors(tg.number());
        if (divs.size() > divisors) {
            break;
        }
    }

    cout << "-------" << endl;
    // Print solution.
    cout << "The triangle number " << tg.number() << " has " << divs.size() << " divisors." << endl;
    cout << "The divisors are:" << endl;
    int line_so_far = 0;
    for (std::set<long>::const_iterator itr = divs.begin(); itr != divs.end(); ++itr) {
        cout << *itr << ",";
        line_so_far++;
        if ((line_so_far % 25) == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

