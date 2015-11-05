#ifndef ACRONIS_HDD_FCFS_H
#define ACRONIS_HDD_FCFS_H

#include <iostream>
#include <map>

#include "hdd_base.h"

using std::map;
using std::pair;
using std::default_random_engine;

extern const short hdd_controller_que_size;
extern const unsigned int hdd_track_count;
extern const unsigned int hdd_sector_size_in_byte;
extern const unsigned int poisson_intension;

class hdd_fcfs: protected hdd_base
{
    hdd_fcfs(){};                                 // HDD should have a size.
    map< double, pair< unsigned int, unsigned int > >::iterator
        get_io_task_from_que();
    double perform_next_io();
public:
    explicit hdd_fcfs( int hdd_size_in_GB ) : hdd_base( hdd_size_in_GB ) {};
    ~hdd_fcfs()
    {
        std::cout << "missed_io: " << missed_io << std::endl;
        std::cout << "max_controller_que_size: " << max_controller_que_size << std::endl;
    };
    double test_avg_seek( unsigned long long );
};

#endif // ACRONIS_HDD_FCFS_H
