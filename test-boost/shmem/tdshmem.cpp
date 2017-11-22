/*
 * tdshmem.cpp
 *
 *  Created on: 22.11.2017
 *      Author: netz
 */

#include <sys/shm.h>
#include <iostream>

int main( int argc, char** argv ) {

	constexpr size_t sh_size = 1000;
	constexpr key_t key = 1234;

	const int id = ::shmget( key, sh_size, IPC_CREAT );

	if ( id < 0 ) {
		std::cerr << "Error retrieving shared memory " << errno << std::endl;
		return 1;
	}

	::shmid_ds ds;

	const int ct = ::shmctl( id, IPC_RMID, &ds );

	if ( ct ) {
		std::cerr << "Error removing shared memory " << ct << std::endl;
		return 1;
	}

	std::cout << "Shared memory ops ok" << std::endl;
	return 0;
}


