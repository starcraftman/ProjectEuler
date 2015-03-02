/**
 * Template C++ file.
 * Function with references to pointers: func(int *&ptr);
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <sstream>
#include <string> /* C++ String class. */

/* C Headers */
#include <cstdlib>

#include "boost/multiprecision/cpp_int.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/************** Global Vars & Functions *******************/
long sum_digits(std::string fact) {
    long sum = 0;

    for (std::string::const_iterator itr = fact.begin(); itr != fact.end(); ++itr) {
        std::string temp;
        temp += *itr;
        sum += std::atoi(temp.c_str());
    }

    return sum;
}

int main() {
    int multiplier = 100;
    boost::multiprecision::cpp_int factorial(1);

    while (multiplier != 1) {
        factorial *= multiplier;
        multiplier -= 1;
    }

    std::stringstream ss;
    ss << factorial;

    cout << "The factorial is: " << factorial << endl
        << "The sum of digits is: " << sum_digits(ss.str()) << endl;

    return 0;
}


/* Notes:
 * Force call to use another version of virtual function: baseP->Item_base::net_price(42);
 *
 */

