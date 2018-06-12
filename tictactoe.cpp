#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <map>

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
#define ZRO   3

int decimal_base(int *dat)
{
    int d = 0, base = 0;

    for (int i = 0; i < 9; ++i) {
        if (i == 0) {
            base = 1;
        } else {
            base *= 3;
        }

        d += dat[i] * base;
    }

    return d;
}

/***
 * @brief Initialize the value function for the specified
 *
 * @param board: the specified board containing AXE/ZRO/EMPTY
 */
float score(const int board[3][3])
{
    /* horizontal test */
    for (int i = 0; i < 3; ++i) {
        int sum = board[i][0] + board[i][1] + board[i][2];

        if (sum == 3 * AXE) {
            return 1.0f;
        } else if (sum == 3 * ZRO) {
            return 0.0f;
        }
    }

    /* vertical test */
    for (int j = 0; j < 3; ++j) {
        int sum = board[0][j] + board[1][j] + board[2][j];

        if (sum == 3 * AXE) {
            return 1.0f;
        } else if (sum == 3 * ZRO) {
            return 0.0f;
        }
    }

    /* diagonal test */
    int sum = 0;

    sum = board[0][0] + board[1][1] + board[2][2];
    if (sum == 3 * AXE) {
        return 1.0f;
    } else if (sum == 3 * ZRO) {
        return 0.0f;
    }

    sum = board[0][2] + board[1][1] + board[2][0];
    if (sum == 3 * AXE) {
        return 1.0f;
    } else if (sum == 3 * ZRO) {
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
                //std::cout << " " << i;
                cvec.push_back(i);
            }
        }

        //std::cout << std::endl;
        list.push_back(cvec);

    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}

/***
 * @brief Score a specific kind of filling-scheme with X/O oblivious.
 *
 * @param scheme: embeded X/O oblivious filling scheme
 */
void evaluate(int scheme, std::map<int, float>& vfunc)
{
    /* convert embeded scheme to 2D scheme */
    int board[3][3] = {EPT}, tmp[9] = {EPT};

    int counter = 0;
    int position[9] = {-1};
    int *pb = static_cast<int*>(&board[0][0]);
    for (int i = 0; i < 9; ++i) {
        pb[i] = (0x1)&(scheme>>i);

        if (pb[i] == 1) {
            position[counter++] = i;
            std::cout << "i: " << i << std::endl;
        }
    }
    std::cout << "scheme parsed done.." << std::endl;

    std::vector<int> select;
    select.push_back(counter / 2);
    if (counter % 2) {
        select.push_back(counter - counter / 2);
    }

    while (!select.empty()) {
        int nselect = select.back();
        select.pop_back();

        /* enumerate the selecting nselect of counter */
        std::list<std::vector<int> > scheme_list;

        combination(nselect, counter, scheme_list);

        /* convert to 2D board for each scheme */
        for (auto itr1 : scheme_list) {
            /* copy the filling pattern for each possible 
             * combination */
            memcpy(tmp, board, sizeof(int) * 9);

            /* set position with the chosen default value */
            for (int i = 0; i < counter; ++i) {
                tmp[i] = ZRO;
            }

            /* update ones according to combination */
            for (auto itr2 : itr1) {
                std::cout << itr2 << " ";

                tmp[itr2] = AXE;
            }
            std::cout << std::endl;

            /* score this pattern and store in value func */
            float s = score(reinterpret_cast<int(*)[3]>(tmp));

            /* snapshot this board status and treat it as 3-based
             * representation, then convert to decimal */
            int decimal = decimal_base(tmp);
        }


    }

}

void generate_value_function(std::map<int, float>& vfunc)
{
    vfunc.clear();

    const int nscheme = 512; // 2^(3*3)

    /* Traverse each filling pattern
     *
     * NOTE that a single filling pattern is not corresponding
     * to a single tic-tac-toe permutation, since the board may
     * contain empty point. */
    for (int i = 0; i < nscheme; ++i) {
        ;
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
              << decimal_base(a)
              << std::endl;
}

int main(int argc, const char *argv[])
{
    /* Global data structures */
    std::map<int, float> value_function;
    
    //evaluate(atoi(argv[1]), value_function);

    return 0;
}
