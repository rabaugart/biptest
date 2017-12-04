/*
 * default_id.h
 *
 *  Created on: 04.12.2017
 *      Author: netz
 */

#ifndef SHMEM_RASHM_DEFAULT_ID_H_
#define SHMEM_RASHM_DEFAULT_ID_H_


namespace rashm {

struct DefaultId {
    static std::string name() {
        return "default";
    }
};

}



#endif /* SHMEM_RASHM_DEFAULT_ID_H_ */
