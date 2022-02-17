/**
Sudoku (Japanese meaning number place) is the name given to a popular puzzle concept.
Its origin is unclear, but credit must be attributed to Leonhard Euler who invented a similar,
and much more difficult, puzzle idea called Latin Squares. The objective of Sudoku puzzles,
however, is to replace the blanks (or zeros) in a 9 by 9 grid in such that each row, column,
and 3 by 3 box contains each of the digits 1 to 9. Below is an example of a typical starting
puzzle grid and its solution grid.

p096_1.png     p096_2.png

A well constructed Sudoku puzzle has a unique solution and can be solved by logic, although it may be necessary to
employ "guess and test" methods in order to eliminate options (there is much contested opinion over this).
The complexity of the search determines the difficulty of the puzzle; the example above is considered easy because
it can be solved by straight forward direct deduction.

The 6K text file, sudoku.txt (right click and 'Save Link/Target As...'), contains fifty different Sudoku puzzles ranging in difficulty,
but all with unique solutions (the first puzzle in the file is the example above).

By solving all fifty puzzles find the sum of the 3-digit numbers found in the top left corner of each solution grid; for example,
483 is the 3-digit number found in the top left corner of the solution grid above.

NOTES:
Good resource on techniques: https://www.learn-sudoku.com/basic-techniques.html
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <iomanip>
#include <fstream>
#include <sstream>
#include <exception>
#include <initializer_list>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <numeric>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
static const std::string INPUT = "./src/input_e096.txt";
static const std::string INPUT_SMALL = "./src/input_e096.small.txt";
static const std::string INPUT_SMALL_SOLVED = "./src/input_e096.small_solved.txt";
static const std::string INPUT_SMALL2 = "./src/input_e096.small2.txt";
static const std::string INPUT_SMALL2_SOLVED = "./src/input_e096.small2_solved.txt";
static const std::string INPUT_SMALL3 = "./src/input_e096.small3.txt";
static const std::string INPUT_SMALL3_SOLVED = "./src/input_e096.small3_solved.txt";
const static std::string line_break = "==========================================";

typedef std::uint64_t num_t;
static const std::initializer_list<num_t> all_values = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// Debug info for CellCheck
enum CheckType { Row, Column, Block };
const static std::map<CheckType, std::string> checktype_to_str = {
    {CheckType::Row, std::string("Row")},
    {CheckType::Column, std::string("Column")},
    {CheckType::Block, std::string("Block")},
};

class Cell {
public:
    Cell(num_t row = 0, num_t col = 0, num_t value = 0) : row(row), col(col), value(value) {
        block = (row / 3) * 3 + col / 3;
        if (this->value == 0) {
            possible.insert(all_values.begin(), all_values.end());
        }
    };
    void add_possible(num_t val) {
        possible.insert(val);
    }
    void remove_possible(num_t val) {
        possible.erase(val);
    }
    void remove_possible(const std::set<num_t> &vals) {
        for (auto val : vals) {
            possible.erase(val);
        }
    }
    void set_possible(const std::vector<num_t> &vals) {
        possible.clear();
        possible.insert(vals.begin(), vals.end());
    }
    void set_possible(num_t val) {
        possible.clear();
        possible.insert(val);
    }
    // Set the value and return a copy of cell before change.
    Cell set_value(num_t val) {
        Cell copy = *this;
        value = val;
        possible.clear();

        return copy;
    }
    void restore(const Cell &cell) {
        value = cell.value;
        possible.clear();
        possible.insert(cell.possible.begin(), cell.possible.end());
    }
    std::ostream& print_details(std::ostream &os) {
        os << "Cell (" << this->row << ", " << this->col << ")"
            << ": " << this->value;

        if (this->possible.size() != 0) {
            os << " Possible: ";
            for (auto ele : this->possible) {
                os << ele << ", ";
            }
        }

        return os;
    }

    friend std::ostream & operator<<(std::ostream &os, const Cell &cell) {
        os << cell.value;
        return os;
    }
    bool operator==(const Cell &other) const {
        return this->row == other.row &&
            this->col == other.col &&
            this->value == other.value;
    }
    bool operator!=(const Cell &other) const { return !(*this == other); }
    bool operator<(const Cell &other) const {
        if (this->possible.size() != other.possible.size()) {
            return this->possible.size() < other.possible.size();
        }
        if (this->row == other.row) {
            return this->col < other.col;
        } else {
            return this->row < other.row;
        }
    }
    bool operator<=(const Cell &other) const { return (*this == other) || *this < other; }
    bool operator>(const Cell &other) const { return !(*this <= other); }
    bool operator>=(const Cell &other) const { return !(*this < other); }

    // Data
    num_t row = 0;
    num_t col = 0;
    num_t block = 0;
    num_t value = 0;
    std::set<num_t> possible;
};
typedef std::vector<std::reference_wrapper<Cell>> cells_vec_t;

// Record a change made to the puzzle, done every time we set a value for certain.
// This is just a storage class.
class ChangeSet {
public:
    ChangeSet(const Cell &cell, num_t new_val) : cell(cell), new_val(new_val) { };
    ChangeSet(num_t row = 0, num_t col = 0, num_t value = 0) :
        cell(Cell(row, col)), new_val(value) {};  // Default + testing short cut

    friend std::ostream & operator<<(std::ostream &os, const ChangeSet &change) {
        os << "Change (" << change.cell.row << "," << change.cell.col << ") => " << change.new_val;
        return os;
    }
    bool operator==(const ChangeSet &other) const {
        return this->cell.row == other.cell.row &&
            this->cell.col == other.cell.col &&
            this->new_val == other.new_val;
    }
    bool operator!=(const ChangeSet &other) const { return !(*this == other); }

    Cell cell;
    num_t new_val = 0;
};

// This class holds references to exactly 9 cells of a sudoku puzzle
// Depending on how references are added, it can accomodate row, col and block checks.
// TODO: Ability to create a "Task" of cells check and execute later.
class CellsCheck {
public:
    CellsCheck(CheckType type = CheckType::Row, num_t num = 0) :
        type(type), num(num) {
        remains.insert(all_values.begin(), all_values.end());
    }
    void add(Cell &cell ) {
        if (cell.value != 0) {
            remains.erase(cell.value);
        }
        cells.push_back(std::ref(cell));
    }
    /*
     * Marks off the possible against what remains. Following is done:
     * - Find all values set in the checker.
     * - Update the remains in the checker based on set found.
     * - For every cell, remove all found values from possible.
     * - Apply subsequent checks.
     */
    void mark_off_possible() {
        std::set<num_t> found;
        remains = all_values;
        for (Cell &cell : cells) {
            if (cell.value != 0) {
                found.insert(cell.value);
                remains.erase(cell.value);
            }
        }

        for (Cell &cell : cells) {
            if (cell.value == 0) {
                cell.remove_possible(found);
            }
        }
    }
    // Strategy: Hidden Singles
    void check_hidden_singles() {
        std::map<num_t, cells_vec_t> cells_possible;
        for (int i = 0; i != 10; ++i) {
            cells_possible[i] = cells_vec_t();
        }
        for (Cell &cell : cells) {
            if (cell.value == 0) {
                // Iterate remaining and build map of possible value -> cells vector.
                for (auto val : cell.possible) {
                    cells_possible[val].push_back(std::ref(cell));
                }
            }
        }
        // If only one cell in a row can be a value, eliminate all other possibles.
        for (auto possible_pair : cells_possible) {
            if (possible_pair.second.size() == 1) {
                Cell &only = possible_pair.second.back();
                only.set_possible(possible_pair.first);
            }
        }
    }
    // Strategy: Naked Pairs, Triples, Quadruples
    // Check and eliminate possible based on naked groupings.
    void check_naked_groups() {
        std::map<std::set<num_t>, cells_vec_t> possible_to_cells;
        for (Cell &cell : cells) {
            if (cell.value == 0 && cell.possible.size() > 1) {
                possible_to_cells[cell.possible].push_back(cell);
            }
        }

        for (auto pair : possible_to_cells) {
            if (pair.first.size() == pair.second.size()) {
                for (Cell &cell : cells) {
                    if (cell.value == 0 && cell.possible != pair.first) {
                        cell.remove_possible(pair.first);
                    }
                }
            }
        }
    }
    void restore(num_t value) {
        remains.insert(value);
    }
    void eliminate(num_t value) {
        remains.erase(value);
    }

    friend std::ostream & operator<<(std::ostream &os, const CellsCheck &check) {
        os << "Check " << checktype_to_str.at(check.type) << " " << check.num << ": ";
        for (auto ele : check.remains) {
            os << ele << " ";
        }
        return os;
    }
    bool operator==(const CellsCheck &other) const {
        if (this->type != other.type &&
                this->num != other.num &&
                this->remains.size() != other.remains.size()) {
            return false;
        }

        auto other_itr = other.remains.cbegin();
        for (auto &remain : remains) {
            if (remain != *other_itr) {
                return false;
            }
            ++other_itr;
        }

        return true;
    }
    bool operator!=(const CellsCheck &other) const { return !(*this == other); }

    CheckType type;
    num_t num;
    std::set<num_t> remains;
    cells_vec_t cells;
};

// Based on the classic 9x9 grid.
// Any positioning starts in top left corner.
// So block 0 is top left, row 0 is first and so on.
//
// General demo:
//  Sudoku puzzle(fin); // fin is any istream
//  cout << puzzle << endl;
//  puzzle.solve()
//  cout << puzzle << endl;
class Sudoku {
public:
    Sudoku() { init_cells(); }
    explicit Sudoku(std::ifstream &fin) {
        init_cells();
        fin >> *this;
        init_checkers();
    }
    // Initialize the cells.
    void init_cells() {
        for (int row = 0; row < 9; ++row) {
            std::vector<Cell> cell_row;
            for (int col  = 0; col  < 9; ++col ) {
                cell_row.push_back(Cell(row, col));
            }

            this->cells.push_back(cell_row);
        }
    }
    // Should be called after reading in values
    void init_checkers() {
        for (int i = 0; i < 9; ++i) {
            row_checks.push_back(CellsCheck(CheckType::Row, i));
            col_checks.push_back(CellsCheck(CheckType::Column, i));
            block_checks.push_back(CellsCheck(CheckType::Block, i));
        }

        for (auto &row : cells) {
            for (Cell &cell : row) {
                row_checks[cell.row].add(cell);
                col_checks[cell.col].add(cell);
                block_checks[cell.block].add(cell);
                if (cell.value == 0) {
                    cells_left.push_back(cell);
                }
            }
        }
    }

    // Looks over all cell areas and eliminates simple possible values.
    void reduce_possible() {
        std::vector<std::vector<CellsCheck> > all_checks = {row_checks, col_checks, block_checks};
        for (std::vector<CellsCheck> &checks : all_checks) {
            for (CellsCheck &check : checks) {
                check.mark_off_possible();
                check.check_hidden_singles();
                check.check_naked_groups();
            }
        }
    }

    // Strategy: Lone Singles - Any cell with only 1 possible is that value.
    // Find all cells with only one possibility and add the change.
    void find_changes(std::vector<ChangeSet> &changes) {
        for (auto itr = cells_left.begin(); itr != cells_left.end(); ++itr) {
            Cell &cell = *itr;
            if (cell.possible.size() == 1) {
                ChangeSet change(cell, *cell.possible.begin());
                changes.push_back(change);
                itr = cells_left.erase(itr) - 1;
            }
        }
    }

    void apply_changes(const std::vector<ChangeSet> &changes) {
        for (const auto &change : changes) {
            history.push_back(change);
            const Cell &cell = change.cell;

            cells[cell.row][cell.col].set_value(change.new_val);
            row_checks[cell.row].eliminate(change.new_val);
            col_checks[cell.col].eliminate(change.new_val);
            block_checks[cell.block].eliminate(change.new_val);
        }
    }

    void reverse_changes(const std::vector<ChangeSet> &changes) {
        for (auto itr = changes.rbegin(); itr != changes.rend(); ++itr) {
            history.pop_back();
            const Cell &cell = itr->cell;

            cells[cell.row][cell.col].restore(cell);
            row_checks[cell.row].restore(itr->new_val);
            col_checks[cell.col].restore(itr->new_val);
            block_checks[cell.block].restore(itr->new_val);
        }
    }

    // Sort the cells remaining that aren't definite.
    void sort_cells_left() {
        std::sort(cells_left.begin(), cells_left.end(),
                [](std::reference_wrapper<Cell> &left, std::reference_wrapper<Cell> &right) {
                return left.get() < right.get(); }
        );
    }

    // Alias for is_valid
    inline bool is_solved() { return is_valid(true); }

    // Check if a puzzle is valid or alternatively if it is solved.
    bool is_valid(bool also_solved = false) {
        for (auto &checks : {row_checks, col_checks, block_checks}) {
            for (auto &check : checks) {
                num_t cnt = 0;
                std::set<num_t> found;
                for (Cell &cell : check.cells) {
                    if (cell.value != 0) {
                        found.insert(cell.value);
                        ++cnt;
                    }
                }
                // Check for dupes
                if (cnt != found.size()) {
                    return false;
                }

                // Solution has to be solved for every checker.
                if (also_solved) {
                    if (cnt != all_values.size()) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    // Just there to answer Euler problem
    num_t top_cells() {
        num_t cnt = 3;
        num_t num = 0;
        for (auto &cell : cells.front()) {
            num = num * 10 + cell.value;
            if (--cnt == 0) {
                break;
            }
        }

        return num;
    }

    // Print detailed breakdown of the Sudoku puzzle, including possibles.
    std::ostream& print_details() {
        for (auto &row : cells) {
            for (auto &cell : row) {
                cell.print_details(cout) << endl;
            }
            cout << line_break << endl;
        }

        return cout;
    }

    // Large and left outside.
    void check_omissions();
    bool try_and_check(num_t frame);
    bool solve(num_t frame = 0);

    bool operator==(const Sudoku &other) const {
        if (cells.size() != other.cells.size()) {
            return false;
        }

        for (int row = 0; row < 9; ++row) {
            if (cells[row].size() != other.cells[row].size()) {
                return false;
            }
            for (int col = 0; col < 9; ++col) {
                if (cells[row][col] != other.cells[row][col]) {
                    return false;
                }
            }
        }

        return true;
    }
    bool operator!=(const Sudoku &other) const { return !(*this == other); }
    friend std::ostream & operator<<(std::ostream &os, const Sudoku &puzzle);
    friend std::istream & operator>>(std::istream &is, Sudoku &puzzle);

    // Data
    std::vector<ChangeSet> history;
    std::vector<CellsCheck> row_checks;
    std::vector<CellsCheck> col_checks;
    std::vector<CellsCheck> block_checks;
    cells_vec_t cells_left; // Cells that aren't solved with certainty.
    std::vector<std::vector<Cell> > cells;
};

// Strategy: Omission
// Cross check between two different types of checkers, like row and block.
// i.e. if 3 can only be in top 2 cells of top left block, eliminate 2 from all others in that block.
// Ref: https://www.learn-sudoku.com/omission.html
// Important: Run after reduce_possible
void Sudoku::check_omissions() {
    // Checks the row and columns against the blocks they intersect
    std::vector<CellsCheck> all_checks = row_checks;
    all_checks.insert(all_checks.end(), col_checks.begin(), col_checks.end());
    for (auto check : all_checks) {
        for (auto left : check.remains) {
            //For every block of row or col, count how many times left in that row.
            std::map<num_t, num_t> block_map;
            for (Cell &cell : check.cells) {
                if (cell.possible.find(left) != cell.possible.end()) {
                    block_map[cell.block]++;
                }
            }

            // Omission found when 2 or 3 of same left ONLY found in one block.
            if (block_map.size() == 1 && block_map.begin()->second > 1) {
                num_t block_num = block_map.begin()->first;
                CellsCheck &block = block_checks[block_num];
                // Erase from all cells that aren't the row.
                for (Cell &cell : block.cells) {
                    // If we are checking rows, eliminate those that aren't current row.
                    // If we are checking cols, eliminate when not equal on the column
                    if ((check.type == CheckType::Row && check.num != cell.row) ||
                        (check.type == CheckType::Column && check.num != cell.col)) {
                        cell.possible.erase(left);
                    }
                }
            }
        }
    }

    // Check rows and columns within a block
    for (auto check : block_checks) {
        for (auto left : check.remains) {
            //For every row of the block, count how many times left in that row only.
            std::map<num_t, num_t> row_map;
            std::map<num_t, num_t> col_map;
            for (Cell &cell : check.cells) {
                if (cell.possible.find(left) != cell.possible.end()) {
                    row_map[cell.row]++;
                    col_map[cell.col]++;
                }
            }

            // Omission found when 2 or 3 of same left ONLY found in one row.
            if (row_map.size() == 1 && row_map.begin()->second > 1) {
                num_t row_num = row_map.begin()->first;
                CellsCheck &row = row_checks[row_num];
                for (Cell &cell : row.cells) {
                    if (cell.block != check.num) {
                        cell.possible.erase(left);
                    }
                }
            }

            // Omission found when 2 or 3 of same left ONLY found in one column.
            if (col_map.size() == 1 && col_map.begin()->second > 1) {
                num_t row_num = col_map.begin()->first;
                CellsCheck &col = col_checks[row_num];
                for (Cell &cell : col.cells) {
                    if (cell.block != check.num) {
                        cell.possible.erase(left);
                    }
                }
            }
        }
    }
}

// Take a low possibilities cell and try them temporarily and check.
// Try one cell's possible values per call, one of the reamining MUST be valid.
bool Sudoku::try_and_check(num_t frame) {
    cout << "Called Try and Check with frame: " << frame << endl;
    sort_cells_left();

    // Pick the first cell with the LEAST possible values to check.
    // Assuming valid prior deductions, ONE MUST be valid part of solution.
    std::deque<ChangeSet> possible_changes;
    for (Cell &cell : cells_left) {
        if (cell.possible.size() > 1) {
            cout << "Selecting this cell to try: " << endl;
            cell.print_details(cout) << endl;
            for (auto val : cell.possible) {
                possible_changes.push_back(ChangeSet(cell, val));
            }

            break;
        }
    }

    Sudoku saved = *this; // Save state here, on failed trial restore.
    while (possible_changes.size() != 0) {
        std::vector<ChangeSet> changes;
        changes.push_back(possible_changes.front());
        possible_changes.pop_front();

        cout << "Trying: " << changes.back() << endl;
        apply_changes(changes);
        if (solve(frame + 1)) {
            cout << "HIT SOLUTION" << endl;
            return true;
        } else {
            cout << "Restoring state...";
            *this = saved; // Retore the state
        }
    }

    return false;
}

// Returns true if was able to solve without issue.
bool Sudoku::solve(num_t frame) {
    cout << "SOLVE: Frame " << frame << " history size " << history.size() << endl;
    std::vector<ChangeSet> changes_so_far;
    num_t cnt = 0;

    while (!is_solved()) {
        cout << "Round : " << ++cnt << endl;
        std::vector<ChangeSet> changes;

        if (!is_valid()) {
            cout << "Failed to validate: " << endl << *this << endl;
            break;
        }

        // Mark down possible cells.
        reduce_possible();
        check_omissions();

        // Visit cells left and determine possible changes, returned in vector
        find_changes(changes);

        // If deduced changes possible, make them
        if (changes.size() != 0) {
            for (auto change : changes) {
                changes_so_far.push_back(change);
                cout << change << endl;
            }

            // Affect the changes
            apply_changes(changes);
        } else {
            cout << "Ran out of deductions... calling try_and_check" << endl;

            // Deductions have failed, pick a possible value of node and check solution.
            return try_and_check(frame);
        }
    }

    return is_solved();
}


// Standard view of just cell values.
std::ostream & operator<<(std::ostream &os, const Sudoku &puzzle) {
    for (auto cell_row : puzzle.cells) {
        for (auto cell : cell_row) {
                os << cell << " ";
        }
        os << endl;
    }

    return os;
}

// Read in the values for the puzzle from input.
std::istream & operator>>(std::istream &is, Sudoku &puzzle) {
    char chr;
    for (auto &cell_row : puzzle.cells) {
        for (auto &cell : cell_row) {
            do {
                is.get(chr);
            } while (chr == '\n');
            num_t val = int(chr) - int('0');
            if (val != 0) {
                cell.set_value(val);
            }
        }
    }
    std::string line;
    std::getline(is, line);

    return is;
}

TEST(Euler096_Cell, Constructor) {
    Cell cell(7, 0, 5);
    ASSERT_EQ(cell.row, 7);
    ASSERT_EQ(cell.col, 0);
    ASSERT_EQ(cell.block, 6);
    ASSERT_EQ(cell.value, 5);
}

TEST(Euler096_Cell, AddPossible) {
    Cell cell(7, 0, 0);
    cell.possible.clear();
    cell.add_possible(7);
    cell.add_possible(2);
    cell.add_possible(3);
    std::set<num_t> expect = {2, 3, 7};
    ASSERT_EQ(cell.possible, expect);
}

TEST(Euler096_Cell, RemovePossible) {
    Cell cell(7, 0, 0);
    cell.possible.clear();
    cell.add_possible(7);
    cell.add_possible(3);
    cell.remove_possible(7);
    std::set<num_t> expect = {3};
    ASSERT_EQ(cell.possible, expect);
}

TEST(Euler096_Cell, RemovePossibleSet) {
    Cell cell(7, 0, 0);
    cell.possible.clear();
    cell.add_possible(7);
    cell.add_possible(6);
    cell.add_possible(3);
    cell.add_possible(1);
    cell.remove_possible({1, 6, 7});
    std::set<num_t> expect = {3};
    ASSERT_EQ(cell.possible, expect);
}

TEST(Euler096_Cell, SetPossible) {
    Cell cell(7, 0, 0);
    cell.set_possible(5);
    ASSERT_TRUE(cell.possible.find(5) != cell.possible.end());
    ASSERT_EQ(cell.possible.size(), 1);
}

TEST(Euler096_Cell, SetPossibleVector) {
    Cell cell(7, 0, 0);
    std::vector<num_t> new_pos = {1, 2, 4};
    cell.set_possible(new_pos);
    ASSERT_TRUE(cell.possible.find(4) != cell.possible.end());
    ASSERT_EQ(cell.possible.size(), 3);
}

TEST(Euler096_Cell, SetValue) {
    Cell cell(7, 0, 0);
    cell.set_possible({2, 3, 7});
    Cell old_cell = cell.set_value(7);
    std::set<num_t> expect;
    ASSERT_EQ(cell.possible, expect);
    ASSERT_EQ(cell.value, 7);
    ASSERT_EQ(old_cell.value, 0);
    ASSERT_EQ(old_cell.possible.size(), 3);
}

TEST(Euler096_Cell, Restore) {
    Cell cell(7, 0, 0);
    cell.set_possible({2, 3, 7});
    Cell old_cell = cell.set_value(7);
    cell.restore(old_cell);
    std::set<num_t> expect = {2, 3, 7};
    ASSERT_EQ(cell.possible, expect);
    ASSERT_EQ(cell.value, 0);
}

TEST(Euler096_Cell, EqualityOperator) {
    Cell cell(7, 0, 9);
    Cell cell2(7, 0, 9);
    Cell cell3(8, 0, 9);
    Cell cell4(7, 0, 1);
    ASSERT_EQ(cell, cell2);
    ASSERT_NE(cell, cell3);
    ASSERT_NE(cell, cell4);
}

TEST(Euler096_Cell, LessThanOperator) {
    Cell cell(7, 0, 0);
    cell.set_possible({1, 2, 7, 8, 9});
    Cell cell2(7, 0, 0);
    cell2.set_possible({1, 2, 7});
    Cell cell3(7, 0, 0);
    cell3.possible.clear();
    ASSERT_LT(cell2, cell);
    ASSERT_LT(cell3, cell);

    // Least row is less if equal
    Cell cell4(4, 0, 0);
    cell4.possible.clear();
    ASSERT_LT(cell4, cell3);

    // Least col is less if equal
    Cell cell5(4, 2, 0);
    cell5.possible.clear();
    ASSERT_LT(cell4, cell5);
}

TEST(Euler096_Cell, OutputOperator) {
    Cell cell(7, 0, 9);
    std::stringstream ss;
    ss << cell;
    ASSERT_EQ(ss.str(), std::string("9"));
}

TEST(Euler096_Cell, PrintDetails) {
    Cell cell(7, 0, 9);
    std::stringstream ss;
    cell.print_details(ss);
    ASSERT_EQ(ss.str(), std::string("Cell (7, 0): 9"));
}

TEST(Euler096_ChangeSet, Constructor) {
    Cell cell(7, 0, 0);
    cell.set_possible({1, 2, 9});

    ChangeSet change(cell, 7);
    ASSERT_EQ(change.cell, cell);
    ASSERT_EQ(change.new_val, 7);
}

TEST(Euler096_ChangeSet, ConstructorDefault) {
    ChangeSet change(0, 1, 7);
    ASSERT_EQ(change.cell.col, 1);
    ASSERT_EQ(change.new_val, 7);
}

TEST(Euler096_ChangeSet, OutputOperator) {
    Cell cell(7, 0, 0);
    cell.set_possible({1, 2, 9});

    ChangeSet change(cell, 7);
    std::stringstream ss;
    ss << change;
    ASSERT_EQ(ss.str(), std::string("Change (7,0) => 7"));
}

TEST(Euler096_ChangeSet, EqualityOperator) {
    Cell cell(7, 0, 0);
    cell.set_possible({1, 2, 9});

    ChangeSet change(cell, 7);
    ChangeSet change2(cell, 7);
    ASSERT_EQ(change, change2);

    ChangeSet change3(cell, 5);
    ASSERT_NE(change, change3);
    Cell cell2(3, 3, 0);
    ChangeSet change4(cell2, 5);
    ASSERT_NE(change, change4);
}

TEST(Euler096_CellsCheck, Add) {
    CellsCheck check;
    Cell cell(0, 0, 9);
    check.add(cell);
    cell = Cell(0, 1, 1);
    check.add(cell);
    cell = Cell(0, 2, 5);
    check.add(cell);

    Cell back = check.cells.back();
    ASSERT_EQ(back, cell);
    ASSERT_EQ(check.remains.size(), 6);
}

TEST(Euler096_CellsCheck, MarkOffPossible) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle(input);

    auto check = puzzle.row_checks[0];
    ASSERT_EQ(puzzle.cells[0][0].possible.size(), 9);
    check.mark_off_possible();
    ASSERT_EQ(puzzle.cells[0][0].possible.size(), 6);
}

TEST(Euler096_CellsCheck, CheckHiddenSingles) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle(input);

    std::vector<num_t> poss = {7, 9};
    puzzle.cells[0][1].set_possible(poss);
    puzzle.cells[0][7].set_possible(poss);
    CellsCheck &check = puzzle.row_checks[0];
    check.mark_off_possible();
    for (Cell & e : check.cells) {
        if (e.col != 0) {
            e.remove_possible(1);
        }
    }

    check.check_hidden_singles();
    std::set<num_t> expect = {1};
    ASSERT_EQ(check.cells[0].get().possible, expect);
}

TEST(Euler096_CellsCheck, CheckNakedPairs) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle(input);

    std::vector<num_t> poss = {7, 9};
    puzzle.cells[0][1].set_possible(poss);
    puzzle.cells[0][7].set_possible(poss);
    CellsCheck &check = puzzle.row_checks[0];

    check.mark_off_possible();
    check.check_naked_groups();
    std::set<num_t> expect = {1, 4, 5, 8};
    ASSERT_EQ(check.cells[8].get().possible, expect);
}

TEST(Euler096_CellsCheck, Restore) {
    CellsCheck check;
    Cell cell(0, 0, 9);
    check.add(cell);
    cell = Cell(0, 1, 1);
    check.add(cell);
    cell = Cell(0, 2, 5);
    check.add(cell);

    std::set<num_t> expect = {2, 3, 4, 6, 7, 8};
    ASSERT_EQ(check.remains, expect);
    check.restore(9);
    expect.insert(9);
    ASSERT_EQ(check.remains, expect);
}

TEST(Euler096_CellsCheck, Eliminate) {
    CellsCheck check;
    Cell cell(0, 0, 9);
    check.add(cell);
    cell = Cell(0, 1, 1);
    check.add(cell);
    cell = Cell(0, 2, 5);
    check.add(cell);

    std::set<num_t> initial = {2, 3, 4, 6, 7, 8};
    ASSERT_EQ(check.remains, initial);
    check.eliminate(7);
    check.eliminate(8);
    std::set<num_t> expect = {2, 3, 4, 6};
    ASSERT_EQ(check.remains, expect);
}

TEST(Euler096_CellsCheck, OutputOperator) {
    CellsCheck check;
    Cell cell(0, 0, 9);
    check.add(cell);
    cell = Cell(0, 1, 1);
    check.add(cell);
    cell = Cell(0, 2, 5);
    check.add(cell);
    std::stringstream ss;
    ss << check;
    std::string expect("Check Row 0: 2 3 4 6 7 8 ");
    ASSERT_EQ(ss.str(), expect);
}

TEST(Euler096_Sudoku, DefaultConstructor) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle;
    std::stringstream ss;
    ss << puzzle;
    std::string expect = "0 0 0 0 0 0 0 0 0";

    ASSERT_TRUE(ss.str().find(expect) != std::string::npos);
}

TEST(Euler096_Sudoku, FileConstructor) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle(input);
    std::stringstream ss;
    ss << puzzle;
    std::string expect = "0 0 3 0 2 0 6 0 0";

    ASSERT_TRUE(ss.str().find(expect) != std::string::npos);
}

TEST(Euler096_Sudoku, InputOperator) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle;
    input >> puzzle;
    std::stringstream ss;
    ss << puzzle;
    std::string expect = "0 0 3 0 2 0 6 0 0";

    ASSERT_TRUE(ss.str().find(expect) != std::string::npos);
}

TEST(Euler096_Sudoku, InitCheckers) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle(input);

    std::set<num_t> expect = {1, 4, 5, 7, 8, 9};
    ASSERT_EQ(puzzle.row_checks.at(0).remains, expect);
}

TEST(Euler096_Sudoku, ReducePossible) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle(input);

    ASSERT_EQ(puzzle.cells[4][5].value, 0);
    ASSERT_EQ(puzzle.cells[4][6].value, 0);
    ASSERT_EQ(puzzle.cells[4][5].possible.size(), 9);
    ASSERT_EQ(puzzle.cells[4][6].possible.size(), 9);
    puzzle.reduce_possible();
    ASSERT_EQ(puzzle.cells[4][5].possible.size(), 1);
    ASSERT_EQ(puzzle.cells[4][6].possible.size(), 1);
    ASSERT_EQ(*puzzle.cells[4][5].possible.begin(), 4);
    ASSERT_EQ(*puzzle.cells[4][6].possible.begin(), 1);
}

TEST(Euler096_Sudoku, CheckOmissions) {
    Sudoku puzzle;
    // Manually create an omission scenario across top rows
    puzzle.cells[0][0].set_possible({6, 7});
    puzzle.cells[0][1].set_possible({1, 6, 7});
    puzzle.cells[0][2].set_value(3);
    puzzle.cells[0][3].set_possible({1, 6, 9});
    puzzle.cells[0][4].set_possible({1, 8, 9});
    puzzle.cells[0][5].set_possible({2, 6});
    puzzle.cells[0][6].set_value(4);
    puzzle.cells[0][7].set_possible({2, 8});
    puzzle.cells[0][8].set_value(5);

    puzzle.cells[1][0].set_possible({7, 8});
    puzzle.cells[1][1].set_value(2);
    puzzle.cells[1][2].set_possible({1, 5});
    puzzle.cells[1][3].set_value(4);
    puzzle.cells[1][4].set_value(3);
    puzzle.cells[1][5].set_possible({5, 7});
    puzzle.cells[1][6].set_possible({1, 9});
    puzzle.cells[1][7].set_value(6);
    puzzle.cells[1][8].set_possible({1, 7, 9});

    puzzle.cells[2][0].set_value(9);
    puzzle.cells[2][1].set_possible({1, 6, 7});
    puzzle.cells[2][2].set_value(4);
    puzzle.cells[2][3].set_possible({1, 5, 6});
    puzzle.cells[2][4].set_possible({1, 5, 8});
    puzzle.cells[2][5].set_possible({2, 5, 6});
    puzzle.cells[2][6].set_possible({2, 7, 8});
    puzzle.cells[2][7].set_possible({2, 3, 8});
    puzzle.cells[2][8].set_possible({2, 3, 7});

    ASSERT_EQ(puzzle.cells[1][0].possible.count(7), 1);
    ASSERT_EQ(puzzle.cells[2][1].possible.count(7), 1);
    puzzle.init_checkers();
    puzzle.reduce_possible();
    puzzle.check_omissions();
    ASSERT_EQ(puzzle.cells[1][0].possible.count(7), 0);
    ASSERT_EQ(puzzle.cells[2][1].possible.count(7), 0);
}

TEST(Euler096_Sudoku, FindChanges) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle(input);
    puzzle.reduce_possible();
    std::vector<ChangeSet> changes;
    puzzle.find_changes(changes);

    ASSERT_EQ(changes.size(), 11);
    auto front = changes.front();
    ASSERT_EQ(front.cell.row, 0);
    ASSERT_EQ(front.cell.col, 5);
    ASSERT_EQ(front.new_val, 1);
}

TEST(Euler096_Sudoku, IsSolvedTrue) {
    std::ifstream input(INPUT_SMALL_SOLVED);
    Sudoku puzzle(input);
    ASSERT_TRUE(puzzle.is_solved());
}

TEST(Euler096_Sudoku, IsSolvedFalse) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle(input);
    ASSERT_FALSE(puzzle.is_solved());
}

TEST(Euler096_Sudoku, IsValidTrue) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle(input);
    ASSERT_TRUE(puzzle.is_valid());
}

TEST(Euler096_Sudoku, IsValidFalseRow) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle(input);
    puzzle.cells[0][2].value = 2;
    ASSERT_FALSE(puzzle.is_valid());
}

TEST(Euler096_Sudoku, IsValidFalseCol) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle(input);
    puzzle.cells[2][0].value = 9;
    ASSERT_FALSE(puzzle.is_valid());
}

TEST(Euler096_Sudoku, IsValidFalseBlock) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle(input);
    puzzle.cells[2][1].value = 9;
    ASSERT_FALSE(puzzle.is_valid());
}

TEST(Euler096_Sudoku, TopCells) {
    std::ifstream input(INPUT_SMALL_SOLVED);
    Sudoku puzzle;
    input >> puzzle;
    ASSERT_EQ(puzzle.top_cells(), 483);
}

TEST(Euler096_Sudoku, ApplyChanges) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle(input);
    std::vector<ChangeSet> changes;
    changes.push_back(ChangeSet(0, 0, 5));
    changes.push_back(ChangeSet(1, 1, 9));

    ASSERT_EQ(puzzle.cells[0][0].value, 0);
    ASSERT_EQ(puzzle.cells[1][1].value, 0);
    puzzle.apply_changes(changes);
    ASSERT_EQ(puzzle.cells[0][0].value, 5);
    ASSERT_EQ(puzzle.cells[1][1].value, 9);
}

TEST(Euler096_Sudoku, ReverseChanges) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle(input);
    std::vector<ChangeSet> changes;
    changes.push_back(ChangeSet(0, 0, 5));
    changes.push_back(ChangeSet(1, 1, 9));

    puzzle.apply_changes(changes);
    ASSERT_EQ(puzzle.cells[0][0].value, 5);
    ASSERT_EQ(puzzle.cells[1][1].value, 9);
    puzzle.reverse_changes(changes);
    ASSERT_EQ(puzzle.cells[0][0].value, 0);
    ASSERT_EQ(puzzle.cells[1][1].value, 0);
}

TEST(Euler096_Sudoku, SortCellsLeft) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle(input);
    puzzle.reduce_possible();


    Cell &cell = puzzle.cells_left[0];
    ASSERT_EQ(cell.row, 0);
    ASSERT_EQ(cell.col, 0);

    puzzle.sort_cells_left();

    Cell &cell2 = puzzle.cells_left[0];
    ASSERT_EQ(cell2.row, 0);
    ASSERT_EQ(cell2.col, 5);
    ASSERT_EQ(cell2.possible.size(), 1);
    Cell &cell3 = puzzle.cells_left.back();
    ASSERT_EQ(cell3.row, 8);
    ASSERT_EQ(cell3.col, 7);
    ASSERT_EQ(cell3.possible.size(), 5);
}

TEST(Euler096_Sudoku, SolveSimple) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle(input);
    puzzle.solve();

    std::ifstream input2(INPUT_SMALL_SOLVED);
    Sudoku puzzle_expect(input2);
    ASSERT_TRUE(puzzle == puzzle_expect);
}

TEST(Euler096_Sudoku, SolvePossibleMap) {
    std::ifstream input(INPUT_SMALL2);
    Sudoku puzzle(input);
    puzzle.solve();

    std::ifstream input2(INPUT_SMALL2_SOLVED);
    Sudoku puzzle_expect(input2);

    ASSERT_TRUE(puzzle == puzzle_expect);
}

TEST(Euler096_Sudoku, SolveDueToOmission) {
    std::ifstream input(INPUT_SMALL3);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    puzzle.solve();

    Sudoku puzzle_expect;
    std::ifstream input2(INPUT_SMALL3_SOLVED);
    input2 >> puzzle_expect;

    cout << "FINAL" << endl;
    cout << puzzle << endl;
    cout << puzzle_expect << endl;
    ASSERT_TRUE(puzzle == puzzle_expect);
}

TEST(Euler096, FinalSolution) {
    std::ifstream input(INPUT, std::ifstream::in);
    int corner_sum = 0;
    int grid_num = 0;
    std::vector<int> failed;

    while (input) {
        Sudoku puzzle;
        grid_num++;
        std::string grid_line;
        std::getline(input, grid_line);
        cout << grid_line << endl;

        input >> puzzle;
        cout << puzzle << endl;
        puzzle.init_checkers();
        puzzle.solve();
        if (!puzzle.solve()) {
            cout << "Failed: " << grid_num << endl;
            failed.push_back(grid_num);
        } else {
            corner_sum += puzzle.top_cells();
        }
        cout << line_break << endl << puzzle << endl;

    }

    if (failed.size()) {
        cout << "Failed to solve " << failed.size() << " grids: ";
        for (auto num : failed) {
            cout << num << ", ";
        }
        cout << endl;
    } else {
        cout << "The sum of the top left cells of solved sudoku puzzles is: " << corner_sum << endl;
        ASSERT_EQ(corner_sum, 24702);
    }
}
