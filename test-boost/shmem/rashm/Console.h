/*
 * Console.h
 *
 *  Created on: 09.12.2017
 *      Author: rainer
 */

#ifndef SHMEM_RASHM_CONSOLE_H_
#define SHMEM_RASHM_CONSOLE_H_

#include <memory>
#include <string>

namespace rashm {

class Console {
public:
	Console();
	virtual ~Console();

	bool key();

	void show( int row, int col, std::string const & text );

protected:

	struct ConsoleImpl;

	std::unique_ptr<ConsoleImpl> impl;
};

} /* namespace rashm */

#endif /* SHMEM_RASHM_CONSOLE_H_ */
