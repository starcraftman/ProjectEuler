/**
 Spiral diagonals, find the sum of the numbers on the two diagonlas.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */

/************** Global Vars & Functions *******************/
int main(void) {
    unsigned long sum_diag = 1;
    int top_l = 0, top_r = 0, bot_l = 0, bot_r = 0, side = 1;

    while (side != 1001) {
        side += 2;
        top_r = side * side;
        top_l = top_r - side + 1;
        bot_l = top_l - side + 1;
        bot_r = bot_l - side + 1;
        sum_diag += top_r + top_l + bot_l + bot_r;
    };

    std::cout << "The sum of all diagonals: " << sum_diag << std::endl;

    return 0;
}

