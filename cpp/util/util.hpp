#ifndef _UTIL_HPP_
#define _UTIL_HPP_

/********************* Header Files ***********************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

namespace util {

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/
typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned short u_shrt;

/************** Class & Func Declarations *****************/
template<class T>
T next_false(std::vector<bool> const & sieve, T start_index) {
    for (T i = start_index; i < sieve.size(); ++i) {
        if (sieve[i] == false) {
            return i;
        }
    }

    return -1;
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
    while (prime != -1) {
        res.push_back(prime);

        T index = prime;
        while (index <= max) {
            sieve[index] = true;
            index += prime;
        }

        prime = next_false(sieve, prime);
    }

    return res;
}

template <class T>
std::vector<T> find_divisors(T num, bool proper=false) {
    std::vector<T> divs;
    T root = std::floor(std::sqrt(num));

    for (int i = 1; i <= root; ++i) {
        if ((num % i) == 0) {
            divs.push_back(i);
            T pair = num / i;
            if (pair > root) {
                divs.push_back(pair);
            }
        }
    }

    std::sort(divs.begin(), divs.end());
    if (proper) {
        divs.erase(divs.end() - 1);
    }

    return divs;
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
    for (int i = 5; i <= root; i += 6) {
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

} /* end util:: */

#endif /* _UTIL_HPP_ */

