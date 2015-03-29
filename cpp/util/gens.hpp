#ifndef _GENS_HPP_
#define _GENS_HPP_

/********************* Header Files ***********************/
namespace util {

/************** Class & Func Declarations *****************/
template <class T>
class FibSeries {
public:
    FibSeries() : two_back(0), one_back(1), num(0) {};
    FibSeries(T two, T one) : two_back(two), one_back(one), num(0) {};
    
    void next() {
        num = one_back + two_back;
        two_back = one_back;
        one_back = num;
    }

    T number() { return num; }
private:
    T two_back;
    T one_back;
    T num;
};

} /* end util:: */

#endif /* _GENS_HPP_ */

