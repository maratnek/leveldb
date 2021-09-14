//
// Created by marat on 13.09.2021.
//

// add proto data to value
#include <iostream>

//#include "first.pb.h"
#include "block.pb.h"

int main()
{
    std::cout << "Test with proto data" << std::endl;
    Block block{}; //= {"name", 1, 0};
    block.set_id("name");
    block.set_hash(1);
    block.set_parent_hash(0);
//    block.id = "name";
//    block.hash = 1;
//    block.parent_hash = 0;
    std::cout << "Id: " << block.id() << " hash: " << block.hash() << " p hash: " << block.parent_hash() << std::endl;

}

