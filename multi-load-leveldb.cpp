//
// Created by maratworker on 13.09.2021.
//
//
// Created by maratworker on 09.09.2021.
//
#include <cstdio>
#include <string>
#include <iostream>
#include <mutex>
#include <future>

#include <cassert>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"

#include "utils.h"

using namespace leveldb;

std::atomic<uint32_t> atom_count = 0;
std::mutex mutex_gener_add_block;

int main(int ac, char** av) {
    std::string db_name = std::string(av[0]);
    std::cout << " DB name " << db_name << std::endl;
    leveldb::DB* db_not_init;
    leveldb::Options options;
    options.create_if_missing = true;

    // open DB
    leveldb::Status s = leveldb::DB::Open(options, "./multi-load-leveldb", &db_not_init );
    assert(s.ok());
    // wrap db over unique_ptr
    std::shared_ptr<leveldb::DB> db{db_not_init};

    auto count = 10'000'000;
    std::vector<std::string> v_r_str;
    v_r_str.reserve(count);
    v_r_str.resize(count);
    // multithread help functions
    using TLoadFunc = std::function<void(size_t, size_t)>;
    auto l_thread_data = [&](TLoadFunc func_load, size_t add_threads = 0) {
        auto block_count = count;
        std::vector<std::future<void>> v_futures;
        /// create several transactions
        auto conc_threads = std::thread::hardware_concurrency();
        conc_threads += add_threads;
        std::cout << "Real thread count: " << conc_threads << std::endl;
        auto interval = block_count / conc_threads;
        for (size_t ith = 0; ith < conc_threads; ++ith)
        {
            v_futures.emplace_back(std::move(std::async(std::launch::async, [=, &func_load]()
            {
                size_t i = ith * interval;
                auto max_count = i + interval;
//                std::cout << "Iter threads: " << ith << " Interval start: " << i << " Interval end: " << max_count << std::endl;
                func_load(i, max_count);
            })));
        }
        std::cout << "Wait futures" << std::endl;
        for (auto &it_future : v_futures)
        {
            it_future.get();
        }

    };

    std::cout << "Prepare data " << std::endl;

    auto start = std::chrono::steady_clock::now();
    l_thread_data([&v_r_str, &db](size_t i, size_t max_count) {
        for (; i < max_count; i++)
        {
            v_r_str[i] = gen_random_str(20);
        }
    });
    auto end = std::chrono::steady_clock::now();
    std::cout << "Prepared Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;

    atom_count = 0;
    std::cout << "Write data" << std::endl;
    start = std::chrono::steady_clock::now();
    l_thread_data([&v_r_str, &db](size_t i, size_t max_count){
        Status s;
        auto commont_count = 0;
        for (; i < max_count; i++)
        {
            ++commont_count;
            s = db->Put(WriteOptions(), std::to_string(i), v_r_str[i]);
            assert(s.ok());
        }
        atom_count += commont_count;
//        std::cout << "Common count " << commont_count << std::endl;
    }, 0);
    end = std::chrono::steady_clock::now();

    std::cout << "Common count " << atom_count << std::endl;
    std::cout << "Writed Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;

    atom_count = 0;
    std::cout << "Read data" << std::endl;
    start = std::chrono::steady_clock::now();
    l_thread_data([&v_r_str, &db](size_t i, size_t max_count){
        Status s;
        auto commont_count = 0;
        std::string value;
        for (; i < max_count; i++)
        {
            ++commont_count;
            //     get value
            s = db->Get(ReadOptions(), std::to_string(i), &value);
            assert(s.ok());
            assert(value == v_r_str[i]);
        }
        atom_count += commont_count;
//        std::cout << "Common count " << commont_count << std::endl;
    }, 0);
    end = std::chrono::steady_clock::now();

    std::cout << "Common count " << atom_count << std::endl;
    std::cout << "Readed Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;


    start = std::chrono::steady_clock::now();
    {
    }
    end = std::chrono::steady_clock::now();

    atom_count = 0;
    std::cout << "Delete data" << std::endl;
    start = std::chrono::steady_clock::now();
    l_thread_data([&v_r_str, &db](size_t i, size_t max_count){
        Status s;
        auto commont_count = 0;
        std::string value;
        for (; i < max_count; i++)
        {
            ++commont_count;
            //     get value
            s = db->Delete(WriteOptions(), std::to_string(i));
            assert(s.ok());
        }
        atom_count += commont_count;
//        std::cout << "Common count " << commont_count << std::endl;
    }, 0);
    end = std::chrono::steady_clock::now();

    std::cout << "Common count " << atom_count << std::endl;
    std::cout << "Deleted Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;

    atom_count = 0;
    std::cout << "Read data" << std::endl;
    start = std::chrono::steady_clock::now();
    l_thread_data([&v_r_str, &db](size_t i, size_t max_count){
        Status s;
        auto commont_count = 0;
        std::string value;
        for (; i < max_count; i++)
        {
            ++commont_count;
            //     get value
            s = db->Get(ReadOptions(), std::to_string(i), &value);
            if (s.ok())
                std::cout << "Value: " << value << std::endl;
        }
        atom_count += commont_count;
//        std::cout << "Common count " << commont_count << std::endl;
    }, 0);
    end = std::chrono::steady_clock::now();

    std::cout << "Common count " << atom_count << std::endl;
    std::cout << "Readed Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;

    return 0;
}
