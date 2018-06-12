#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <map>

/**************************************************
 * Plate Embedding Scheme
 *
 *      +---+
 *    2 |   |
 *  y 1 |   |  ---> bit[9]
 *    0 |   |
 *      +---+
 *       012
 *        x
 *
 **************************************************/

#define AXE   32
#define ZRO   64

float score(int *board)
{
    return 0.0f;
}

void combination(int select, int total, std::list<std::vector<int> >& list)
{
    std::vector<int> bitmask(select, 1);
    bitmask.resize(total, 0);

    do {
        for (int i = 0; i < total; ++i) {
            if (bitmask[i])
                std::cout << " " << i;
        }
        std::cout << std::endl;
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}

float evaluate(int scheme)
{
    float s = 1.0f;

    // convert embeded scheme to 2D scheme
    int board[3][3] = {0};

    int counter = 0;
    int position[9] = {-1};
    int *pb = static_cast<int*>(&board[0][0]);
    for (int i = 0; i < 9; ++i) {
        pb[i] = (0x1)&(scheme>>i);

        if (pb[i] == 1) {
            position[counter++] = i;
            std::cout << "i: " << i << " ";
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

        // enumerate the selecting nselect of counter
        std::list<std::vector<int> > scheme_list;

        combination(nselect, counter, scheme_list);

    }

    return s;
}

void generate_value_function(std::map<int, float>& vfunc)
{
    vfunc.clear();

    const int nscheme = 512; // 2^(3*3)

    for (int i = 0; i < nscheme; ++i) {
        ;
    }

}

int main(int argc, const char *argv[])
{
    // table
    char board[3][3];
    
    evaluate(63);

    return 0;
}
