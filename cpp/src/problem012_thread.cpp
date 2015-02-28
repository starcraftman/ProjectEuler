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
#include <cstdlib>

/* Project */
#include "problem012_thread.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/****************** Class Definitions *********************/

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
    std::vector<int> v;
    boost::mutex mtx;
    while (true) {
        mtx.lock();
        long tval = tg.next();
        mtx.unlock();
        cout << "Triangle number is " << tval << "." << endl;
        v.clear();
        find_divisors(tg.number(), v);
        if (v.size() > divisors) {
            break;
        }
    }

    cout << "-------" << endl;
    // Print solution.
    cout << "The triangle number " << tg.number() << " has " << v.size() << " divisors." << endl;
    cout << "The divisors are:" << endl;
    int line_so_far = 0;
    for (std::vector<int>::const_iterator itr = v.begin(); itr != v.end(); ++itr) {
        cout << *itr << ",";
        line_so_far++;
        if ((line_so_far % 25) == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

