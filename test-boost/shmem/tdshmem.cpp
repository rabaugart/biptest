/*
 * tdshmem.cpp
 *
 *  Created on: 22.11.2017
 *      Author: netz
 */

#include <sys/shm.h>
#include <iostream>
#include <stdexcept>
#include <map>

#include <boost/format.hpp>

#include <string.h>

int main(int argc, char** argv) {

    constexpr size_t sh_size = 1000;
    constexpr key_t key = 1234;

    const int id = ::shmget(key, sh_size, IPC_CREAT | 511 );

    if (id < 0) {
        std::cerr << "Error retrieving shared memory " << strerror(errno) << std::endl;
        return 1;
    }

    void* const addr = ::shmat( id, (void*)0x3f00000000, SHM_RND );
    if ( (long) addr < 0 ) {
        std::cerr << "Error in shmat " << strerror(errno) << std::endl;
    }

    ::shmid_ds ds;

    const int ct = ::shmctl(id, IPC_RMID, &ds);

    if (ct) {
        std::cerr << "Error removing shared memory " << ct << std::endl;
        return 1;
    }

    std::cout << "Shared memory ops ok" << std::endl;
    return 0;
}

