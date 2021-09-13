//
// Created by maratworker on 09.09.2021.
//

#include "utils.h"
#include <random>

using namespace std;

static std::random_device rd;   // non-deterministic generator
static std::mt19937 gen(rd());  // to seed mersenne twister.
////////////////////////////////////////////////////////////////
/// random for tests
std::string gen_random_str(const int len)
{
//    random_device rd;   // non-deterministic generator
//    mt19937 gen(rd());  // to seed mersenne twister.
    std::string tmp_s;
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    uniform_int_distribution<> dist(1,sizeof(alphanum)); // distribute results between 1 and 6 inclusive.

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[dist(gen)];

    return std::move(tmp_s);
}

uint64_t gen_num()
{
    return gen();
}