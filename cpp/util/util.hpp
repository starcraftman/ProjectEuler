#ifndef _UTIL_HPP_
#define _UTIL_HPP_

/********************* Header Files ***********************/
#include <exception>
#include <iostream>
#include <set>
#include <vector>
#include <cmath>

namespace util {

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/
typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned short u_shrt;

/************** Class & Func Declarations *****************/
// Simple class to throw when processing done.
class NoMoreValues: public std::exception {
    public:
    virtual const char * msg() const throw() {
        return "No more values to process.";
    }
};


template<class T>
T next_false(std::vector<bool> const & sieve, T start_index) {
    for (typename std::vector<bool>::size_type i = start_index; i < sieve.size(); ++i) {
        if (sieve[i] == false) {
            return i;
        }
    }

    throw NoMoreValues();
}

template <class T>
T pow(T base, int exp) {
    if (exp < 0) {
        return 1 / pow(base, -1 * exp);;
    } else if (exp == 0) {
        return 1;
    } else if (exp == 1) {
        return base;
    } else if ((exp % 2) == 0) {
        T temp = pow(base, exp/2);
        return temp * temp;
    } else {
        T temp = pow(base, (exp - 1)/2);
        return temp * temp * base;
    }
}

/* Return all primes under max. */
template <class T>
std::vector<T> sieve_erat(T max) {
    std::vector<T> res;

    // Ignore 1 & 0
    std::vector<bool> sieve(max);
    sieve[0] = true;
    sieve[1] = true;

    T prime = 2;
    try {
        while (true) {
            res.push_back(prime);

            T index = prime;
            while (index <= max) {
                sieve[index] = true;
                index += prime;
            }

            prime = next_false(sieve, prime);
        }
    } catch (NoMoreValues &e) {
        std::cout << e.msg() << std::endl;
    }

    return res;
}

template <class T>
std::vector<T> find_divisors(T num, bool proper=false) {
    std::set<T> divs;

    typename std::set<T>::size_type root = std::floor(std::sqrt(num));
    for (typename std::set<T>::size_type i = 1; i <= root; ++i) {
        if ((num % i) == 0) {
            divs.insert(i);
            divs.insert(num / i);
        }
    }

    if (proper) {
        divs.erase(num);
    }

    return std::vector<T>(divs.begin(), divs.end());
}

template <class T>
bool is_prime(T num) {
    if (num <= 3) {
        return num > 1;
    }

    if ((num % 2) == 0 || (num % 3) == 0) {
        return false;
    }

    T root = std::floor(std::sqrt(num));
    for (T i = 5; i <= root; i += 6) {
        if ((num % i) == 0 || (num % (i + 2)) == 0) {
            return false;
        }
    }

    return true;
}

/* Only applies to base 10 numbers. */
template <class T>
T reverse(T num) {
    T reversed = 0;
    while (num != 0) {
        reversed *= 10;
        reversed += num % 10;
        num /= 10;
    }

    return reversed;
}

template <class T>
T euclid_gcd(T min, T max) {
    if (min > max) {
        std::swap(min, max);
    }

    while (min != 0) {
        max -= min * (max / min);
        std::swap(min, max);
    }

    return max;
}

template <class T>
bool is_coprime(T num1, T num2) {
    return euclid_gcd(num1, num2) == 1;
}

} /* end util:: */

#endif /* _UTIL_HPP_ */

