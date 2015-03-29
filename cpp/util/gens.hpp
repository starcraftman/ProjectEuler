#ifndef _GENS_HPP_
#define _GENS_HPP_

/********************* Header Files ***********************/
namespace util {

namespace gens {

/************** Class & Func Declarations *****************/
/* Starts at Fibonacci number 1. */
template <class T>
class Fibonacci {
public:
    Fibonacci() : two_back(0), one_back(1), num(1), ind(1) {};
    Fibonacci(T two, T one, T ind) : two_back(two), one_back(one), 
            num(one + two), ind(ind) {};
    
    T next() {
        num = one_back + two_back;
        two_back = one_back;
        one_back = num;

        return num;
    }

    T number() { return num; }
    T index() { return ind; }
private:
    T two_back;
    T one_back;
    T num;
    T ind;
};

} /* end util::gens */

} /* end util:: */

#endif /* _GENS_HPP_ */

