/**
 * Find first triangle number with over 500 divisors.
 * Threaded version, have n threads maxing cores. Each tests a triangle.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <string> /* C++ String class. */

/* STL Headers */
#include <vector>

/* C Headers */
#include <cmath>

#include "gtest/gtest.h"
#include "boost/thread.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

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
    boost::mutex lock;
};

/************** Global Vars & Functions *******************/
void find_divisors(long tval, std::vector<int> &v) {
    for (int i = 1; i <= tval; ++i) {
        if ((tval % i) == 0) {
            v.push_back(i);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: " << *argv << " <divisors>" << endl;
        cout << "Where <divisors> is the number of divisors the triangle should have." << endl;
        return 1;
    }

    ++argv;
    unsigned long divisors = std::atol(*argv);

    TriangleGenerator tg;
    std::vector<int> v;
    while (true) {
        tg.next();
        cout << "Triangle number " << tg.index() << " is " << tg.number() << "." << endl;
        v.clear();
        find_divisors(tg.number(), v);
        if (v.size() > divisors) {
            break;
        }
    }

    cout << "-------" << endl;
    // Print solution.
    cout << "The number is " << tg.number() << "." << endl;
    cout << "The divisors are:" << endl;
    int line_so_far = 0;
    for (auto i : v) {
        cout << i << " ";
        line_so_far++;
        if ((line_so_far % 25) == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

