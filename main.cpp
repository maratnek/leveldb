#include <iostream>
#include <cassert>
#include "leveldb/db.h"


int main() {
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "./testdb", &db);
    assert(status.ok());

    std::string value;
    auto key1 = "key1";
    leveldb::Status s = db->Get(leveldb::ReadOptions(), key1, &value);
    std::cout << "Get data:" << value << std::endl;
    value = "my first data";
    if (s.ok())
        s = db->Put(leveldb::WriteOptions(), key1, value);
    s = db->Get(leveldb::ReadOptions(), key1, &value);
    std::cout << "Get data:" << value << std::endl;
    if (s.ok()) s = db->Delete(leveldb::WriteOptions(), key1);
    std::cout << "Delete " << key1 << std::endl;
    s = db->Get(leveldb::ReadOptions(), key1, &value);
    assert(s.ok());
    if (s.ok())
        std::cout << "Get data:" << value << std::endl;
    return 0;
}
