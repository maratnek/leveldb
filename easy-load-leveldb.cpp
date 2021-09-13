//
// Created by marat on 13.09.2021.
//

#include <string>
#include <iostream>

#include <cassert>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"

using namespace leveldb;

#include "utils.h"

int main(int ac, char **av)
{
    std::string db_name = std::string(av[0]);
    std::cout << " DB name " << db_name << std::endl;
    leveldb::DB* db_not_init;
    leveldb::Options options;
    options.create_if_missing = true;

    // open DB
    leveldb::Status s = leveldb::DB::Open(options, "./easy-load-leveldb", &db_not_init );
    assert(s.ok());
    // wrap db over unique_ptr
    std::unique_ptr<leveldb::DB> db{db_not_init};


    auto count = 10'000'000;
    std::unordered_map<std::string, std::string> v_r_str(count);
    std::cout << "Prepare data " << std::endl;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; ++i) {
        v_r_str.insert({std::to_string(i),gen_random_str(20)});
    }
    std::cout << "Prepared " << v_r_str.size() << " count" << std::endl;
    auto end = std::chrono::steady_clock::now();
    std::cout << "Prepared Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;


    std::cout << "Write to db: " << count << " values." << std::endl;
//    auto hash_val = std::hash<std::string>{}(random_value);
    start = std::chrono::steady_clock::now();
    for (auto& it:v_r_str) {
        // Put key-value
        s = db->Put(WriteOptions(), it.first, it.second);
        assert(s.ok());
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Write Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;


    std::string value;
    start = std::chrono::steady_clock::now();
    std::cout << "Read from db: " << count << " values." << std::endl;
    for (auto& it:v_r_str) {
        //     get value
        s = db->Get(ReadOptions(), it.first, &value);
        assert(s.ok());
        assert(value == it.second);
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Read Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;

    start = std::chrono::steady_clock::now();
    {
        for (auto& it:v_r_str) {
            //     get value
            s = db->Delete(WriteOptions(), it.first);
            assert(s.ok());
        }
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Delete Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;

    start = std::chrono::steady_clock::now();
    std::cout << "Read from db: " << count << " values." << std::endl;
    for (auto& it:v_r_str) {
        //     get value
        s = db->Get(ReadOptions(), it.first, &value);
        if (s.ok())
            std::cout << "Value: " << value << std::endl;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Read Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;
    // TODO it's temperatury
   return 0;


    std::cout << "Write to db over batch: " << count << " values." << std::endl;
    start = std::chrono::steady_clock::now();
    WriteBatch batch;
    auto it_count = 0;
    auto batch_size = 10000;
    auto commont_count = 0;
    for (auto& it:v_r_str) {
        ++commont_count;
        if (it_count++ < batch_size)
        {
            batch.Put(it.first, it.second);
        }
        else
        {
            s = db->Write(WriteOptions(), &batch);
            assert(s.ok());
            batch.Clear();
            it_count = 0;
        }
    }
//    if (batch.Count())
    {
        s = db->Write(WriteOptions(), &batch);
        assert(s.ok());
        batch.Clear();
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Write Time over batch: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;
    std::cout << "common count " << commont_count << std::endl;

    start = std::chrono::steady_clock::now();
    batch.Clear();
    it_count = 0;
    commont_count = 0;
    for (auto& it:v_r_str) {
        ++commont_count;
        if (it_count++ < batch_size)
        {
            batch.Delete(it.first);
        }
        else
        {
            s = db->Write(WriteOptions(), &batch);
            assert(s.ok());
            batch.Clear();
            it_count = 0;
        }
    }
//    if (batch.Count())
    {
        s = db->Write(WriteOptions(), &batch);
        assert(s.ok());
        batch.Clear();
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Delete Time over batch: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000'000.0 << " sec" << std::endl;
    std::cout << "common count " << commont_count << std::endl;


    return 0;
}



