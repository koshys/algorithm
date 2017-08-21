/**
 * Author : Samson Koshy
 * Desc : suduko 4x4
 *
 */

#include <fstream>
#include <iostream>
//#include <set>

/**
 * overall game board length
 */
constexpr int LEN = 16;
//constexpr int LEN = 9;

/**
 * for the 4X4 grid
 */
constexpr int SUBLEN = 4;
//constexpr int SUBLEN = 3;

/**
 * symbols used are unique -- hex values
 */
//const std::set<char> sym {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//const std::set<char> sym {'1','2','3','4','5','6','7','8','9'};
const char sym[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//const char sym[9] = {'1','2','3','4','5','6','7','8','9'};
/**
 * character indicating that the position is not filled
 */
const char NOTFILLED = '-';

/**
 *  hold the metrics
 */
struct metrics {
    int maxStackHeight = -1;
    int backtracked = 0;
    int iterations = 0;
} m;


/**
 * Iterate through the sub board and check for the value
 * Helper Function
 * @param b address of the board
 * @param subR sub board row
 * @param subC sub board column
 * @param v value
 * @return bool
 */
bool inSubBoard(char b[LEN][LEN], int subR, int subC, char v)
{
    for (int r = 0; r < SUBLEN; r++) {

        for (int c = 0; c < SUBLEN; c++) {

            if (b[r + subR][c + subC] == v) {

                return true;
            }
        }
    }

    return false;
}

/**
 * Iterate through the entire row of the board and check for the value
 * Helper Function
 * @param b address of the board
 * @param r row
 * @param v value
 * @return bool
 */
bool inRow(char b[LEN][LEN], int r, char v)
{
    for (int c = 0; c < LEN; c++) {

        if ( b[r][c] == v) {

            return true;
        }
    }

    return false;
}

/**
 * Iterate through the entire column of the board and check for the value
 * Helper Function
 * @param b address of the board
 * @param c column
 * @param v value
 * @return bool
 */
bool inColumn(char b[LEN][LEN], int c, char v)
{
    for (int r = 0; r < LEN; r++) {

        if ( b[r][c] == v) {

            return true;
        }
    }

    return false;
}

/**
 * Helps to identify the co-ordinate on the board that needs to be updated
 * Helper Function
 * @param b address of the board
 * @param r a reference to the row to maintain the right address during recursion
 * @param c a reference to the column  to maintain the right address during recursion
 * @return bool
 */
bool isNotFilled(char b[LEN][LEN], int &r, int &c)
{
    for (r = 0; r < LEN; r++) {
        for (c = 0; c < LEN; c++) {
            if (b[r][c] == NOTFILLED) {
                return true;
            }
        }
    }

    return false;
}

/**
 * check if the value is on the subBoard or the column or row of the co-ordinate r,c
 * its legal if the value is not there on the three constraints
 * Helper Function
 * @param b address of the board
 * @param r row
 * @param c column
 * @param v value
 * @return bool
 */
bool isLegal(char b[LEN][LEN], int r, int c, char v)
{

    /**
     *  (r  - r%SUBLEN, c - c%SUBLEN ) gives the top left of the Sub Board.
     */

    return
            !inRow(b,r,v) &&
            !inColumn(b,c,v) &&
            !inSubBoard(b,r - r%SUBLEN, c - c%SUBLEN,v);
}

/**
 * the actual work comes together here
 *
 * @param b
 * @return
 */
bool work(char b[LEN][LEN])
{
    int r = 0, c = 0;
    ++m.iterations;
    ++m.maxStackHeight;

    // at this point the puzzle is solved
    if ( !isNotFilled(b, r, c) ) {

        return true;
    }

    // iterate through the allowed symbols
//    for ( auto it = sym.begin(); it != sym.end(); ++it ) {
    for ( int i = 0; i < LEN; ++i ) {

        // v is a possible candidate during traversal
//        char v = *it;
        char v = sym[i];
        if ( isLegal(b, r, c, v) )
        {
            b[r][c] = v;

            // if filled by the next move and passed the three
            // constraints then v is the correct choice
            if ( work(b) ) {

                return true;

            }

            // at this point v failed to meet the constraints and is set to NOTFILLED
            // this propagates the recursion to traverse back one step
            // and try from the address of r and c on the recursion stack
            b[r][c] = NOTFILLED;
            ++m.backtracked;
            --m.maxStackHeight;

        }
    }
    return false;
}


/**
 * Print the board
 * @param b board
 */
void print(char b[LEN][LEN])
{
    std::cout << std::endl;

    for (int r = 0; r < LEN; ++r)
    {
        for (int c = 0; c < LEN; ++c) {

            std::cout << b[r][c];
        }

        std::cout << std::endl;
    }

    std::cout << " Metrics " << " |  Nodes : " << m.iterations <<
              " | Backtracked : " << m.backtracked <<
              " | MaxStackHeight : " << m.maxStackHeight;
}

int main() {

    // array storing the puzzle
    char board[LEN][LEN];
    int r = 0 ,c = 0;

    // read the puzzle file into the board
    std::ifstream ifs;
    char value = '\0';
//    ifs.open("worldsHardest.txt");
    ifs.open("SudokuPuzzle7.txt");
    if ( ifs.is_open() ) {

        while ( ! ifs.eof() ) {
            ifs.get(value);
            if ( value == '\r' ) {
                ifs.get(value);
            }
            if ( value == '\n' ) {
                ++r;
            } else {
                board[r][c] = value;
                ++c;
                if ( c == LEN ) {
                    c = 0;
                    if ( r == LEN - 1) {
                        break;
                    }
                }
            }
        }
    } else {
        std::cout << "error opening file.." << std::endl;
        return 1;
    }
    ifs.close();
    if ( ifs.is_open() ) {
        std::cout << "error closing file.." << std::endl;
        return 1;
    }

    // print the problem Sudoku Puzzle first
    std::cout << std::endl << std::endl << "The Sudoku Puzzle" << std::endl << std::endl;
    print(board);

    // solve the puzzle
    if ( work( board ) ) {

        std::cout << std::endl << std::endl << "The Sudoku Puzzle Solution" << std::endl << std::endl;

        print( board );
    }

}

