#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <chrono>
#include <array>

#include "shmem/ShmWriter.h"
#include "shmem/ShmReader.h"
#include "test/TestDataA.h"

using namespace rshm;

int main(int argc, char *argv[])
{
	using namespace boost::interprocess;

	TestDataA testData{ 5.7, 55 };

	if(argc == 1){  //Parent process
		//Remove shared memory on construction and destruction

		std::cout << "Parent starting" << std::endl;

		ShmSegment::shm_remove remover;
		ShmSegment shm;

		//Write all the memory to 1
		ShmWriter<TestDataA> dwrite(shm);
		dwrite = testData;

		//Launch child process
		std::string s(argv[0]); s += " child ";
		if(0 != std::system(s.c_str()))
			return 1;
		std::cout << "Parent finished" << std::endl;
	}
	else{
		std::cout << "Child starting" << std::endl;

		//Open already created shared memory object.
		ShmSegment shm;

		//Map the whole shared memory in this process
		ShmReader<TestDataA> dread(shm);
		TestDataA d = dread.get();

		//Check that memory was initialized
		if ( d.a != testData.a || d.b != testData.b )
		{
			throw std::runtime_error( "Read error" );
		}

		if ( dread.getCounter() != 1ull )
		{
			throw std::runtime_error( "Counter error" );
		}

		if ( dread.now() - dread.getTimestamp() > std::chrono::microseconds(10000) )
		{
			throw std::runtime_error( "Timer error" );
		}

		std::cout << "Child finished "
				<< std::chrono::duration_cast<std::chrono::microseconds>(dread.now()-dread.getTimestamp()).count()
				<< std::endl;
	}
	return 0;
}
