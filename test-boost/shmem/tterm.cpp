/*
 * tterm.cpp
 *
 *  Created on: 09.12.2017
 *      Author: rainer
 */

#include <iostream>
#include <thread>
#include <chrono>

#include <boost/format.hpp>

#include "rashm/Console.h"

int main( int argc, char** argv ) {
	{
		rashm::Console con;
		int count = 0;
		do {
			con.show( 3, 5, (boost::format("Hi %1%" ) % count++).str() );
		} while ( !con.key() );
	}
	std::cout << "Ok" << std::endl;
}
