//
// Created by maratworker on 09.09.2021.
//

#ifndef ROCKSDB_UTILS_H
#define ROCKSDB_UTILS_H
#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include <iostream>
#include <ctime>
#include <unistd.h>

static std::string tp_to_string(const std::chrono::system_clock::time_point &tp)
{
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::string ts = std::ctime(&t);
    ts.resize(ts.size() - 1);
    return ts;
}


std::string gen_random_str(const int len);

uint64_t gen_num();

#endif //ROCKSDB_UTILS_H
