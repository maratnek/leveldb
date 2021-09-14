//
// Created by marat on 13.09.2021.
//

// add proto data to value
#include <iostream>

//#include "first.pb.h"
#include "block.pb.h"
#include "protocols/routed.h"

int main()
{
    std::cout << "Test with proto data" << std::endl;
    Block block1{};
    std::cout << "Is initialized: " << block1.IsInitialized() << std::endl;
    std::cout << "Debug string: " << block1.DebugString() << std::endl;


    Block block{};
    block.set_id("name");
    block.set_hash(1);
    block.set_parent_hash(2);
    std::cout << "Serialized: " << block.SerializeAsString() << "\n";
    std::cout << "Serialized: " << block.SerializePartialAsString() << "\n";
    std::string value;
    block.SerializeToString(&value);
    std::cout << "Value: " << value << std::endl;
    std::cout << "Debug string: " << block.DebugString() << std::endl;
    std::cout << "Id: " << block.id() << " hash: " << block.hash() << " p hash: " << block.parent_hash() << std::endl;



}

