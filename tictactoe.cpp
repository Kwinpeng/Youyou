#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <map>

#include <cstdio>
#include <cstdlib>
#include <cstring>

/*********************************************************
 * Board Embedding Scheme
 *
 *      +---+
 *    2 |   |
 *  y 1 |   |  ---> bit[9]
 *    0 |   |
 *      +---+
 *       012
 *        x
 *
 *********************************************************/

#define EPT   0
#define FIL   1
#define AXE   2
#define OWE   3

#define QUAN_BASE  4

void show_board(int board[3][3])
{
    printf("---------------------\n");
    printf("\t   0  1  2\n");
    for (int i = 0; i < 3; ++i) {
        printf("\t%d: %d, %d, %d\n",
                i, board[i][0], board[i][1], board[i][2]);
    }
    printf("---------------------\n");
}

int quanternary2decimal(int *dat)
{
    int d = 0, base = 0;

    for (int i = 0; i < 9; ++i) {
        if (i == 0) {
            base = 1;
        } else {
            base *= QUAN_BASE;
        }

        d += dat[i] * base;
    }

    return d;
}

/***
 * @brief Initialize the value function for the specified
 *
 * @param board: the specified board containing AXE/OWE/EMPTY
 */
float score(const int board[3][3])
{
    /* horizontal test */
    for (int i = 0; i < 3; ++i) {
        int sum = board[i][0] + board[i][1] + board[i][2];

        if (sum == 3 * AXE) {
            return 1.0f;
        } else if (sum == 3 * OWE) {
            return 0.0f;
        }
    }

    /* vertical test */
    for (int j = 0; j < 3; ++j) {
        int sum = board[0][j] + board[1][j] + board[2][j];

        if (sum == 3 * AXE) {
            return 1.0f;
        } else if (sum == 3 * OWE) {
            return 0.0f;
        }
    }

    /* diagonal test */
    int sum = 0;

    sum = board[0][0] + board[1][1] + board[2][2];
    if (sum == 3 * AXE) {
        return 1.0f;
    } else if (sum == 3 * OWE) {
        return 0.0f;
    }

    sum = board[0][2] + board[1][1] + board[2][0];
    if (sum == 3 * AXE) {
        return 1.0f;
    } else if (sum == 3 * OWE) {
        return 0.0f;
    }

    return 0.5;
}

void combination(int select, int total, std::list<std::vector<int> >& list)
{
    list.clear();

    std::vector<int> bitmask(select, 1);
    bitmask.resize(total, 0);

    do {
        std::vector<int> cvec;
        for (int i = 0; i < total; ++i) {
            if (bitmask[i]) {
                std::cout << " " << i;
                cvec.push_back(i);
            }
        }

        std::cout << std::endl;
        list.push_back(cvec);

    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}

/***
 * @brief Score a specific kind of filling-pattern with X/O oblivious.
 *
 * @param pattern: embeded X/O oblivious filling pattern
 */
void evaluate(int pattern, std::map<int, float>& vfunc)
{
    /* convert embeded scheme to 2D scheme */
    int board[3][3] = {{EPT}}, tmp[9] = {EPT};

    /* record filling position */
    int counter = 0;
    int position[9] = {-1};

    std::cout << "Pattern: " << pattern
              << " being parsed.." << std::endl;

    int *pb = static_cast<int*>(&board[0][0]);
    std::cout << "Position: ";
    for (int i = 0; i < 9; ++i) {
        pb[i] = (0x1)&(pattern>>i);

        if (pb[i] == 1) {
            position[counter++] = i;
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;

    std::vector<int> select;
    select.push_back(counter / 2);
    if (counter > 2 && counter % 2) {
        select.push_back(counter - counter / 2);
    }

    while (!select.empty()) {
        /* for each filling pattern */
        int nselect = select.back();
        select.pop_back();

        std::list<std::vector<int> > scheme_list;

        /* enumerate all the possible scheme of this filling-
         * pattern */
        combination(nselect, counter, scheme_list);

        /* convert to 2D board for each combination scheme */
        for (auto itr1 : scheme_list) {
            /* copy the filling pattern for each possible 
             * combination */
            memcpy(tmp, board, sizeof(int) * 9);

            /* set position with the chosen default value */
            for (int i = 0; i < counter; ++i) {
                tmp[position[i]] = OWE;
            }

            /* update ones according to combination */
            std::cout << "The chosen ones as X are in position ";
            for (auto itr2 : itr1) {
                std::cout << itr2 << " ";

                tmp[position[itr2]] = AXE;
            }
            std::cout << std::endl;

            show_board(reinterpret_cast<int(*)[3]>(tmp));

            /* score this pattern and store in value func */
            float s = score(reinterpret_cast<int(*)[3]>(tmp));

            /* snapshot this board status and treat it as 3-based
             * representation, then convert to decimal */
            int d = quanternary2decimal(tmp);

            /* update the corresponding item in value function */
            std::pair<std::map<int, float>::iterator, bool> res =
                vfunc.insert(std::make_pair(d, s));

            if (!res.second)
                std::cout << "Warning: the key already exist!" << std::endl;

        } // end for

    } // end while

}

void init_value_function(std::map<int, float>& vfunc)
{
    vfunc.clear();

    const int nscheme = 512; // 2^(3*3)

    /* Traverse each filling pattern and initialize all the
     * schemes of that pattern.
     *
     * NOTE that a single filling pattern is not corresponding
     * to a single tic-tac-toe board situation, which called
     * scheme in our convention, since the board may contain
     * empty point and each fill point maybe an X or O. */
    for (int e = 0; e < nscheme; ++e) {
        evaluate(e, vfunc);
    }

}

void test_decimal_base()
{
    int a[9] = {
        0, 1, 0,
        0, 0, 0,
        0, 0, 0
    };

    std::cout << "a to decimal base is "
              << quanternary2decimal(a)
              << std::endl;
}

int main(int argc, const char *argv[])
{
    /* Global data structures */
    std::map<int, float> value_function;

    init_value_function(value_function);

    return 0;
}
