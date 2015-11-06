#ifndef ACRONIS_HDD_FCFS_H
#define ACRONIS_HDD_FCFS_H

#include <iostream>
#include <map>

#include "hdd_base.h"
#include "function_templates.h"

using std::map;
using std::pair;
using std::default_random_engine;

class hdd_fcfs: public hdd_base
{
    hdd_fcfs(){};                                 // HDD should have a size.
    map< double, pair< unsigned int, unsigned int > >::iterator
        get_io_task_from_que();
    double perform_next_io();
    friend double ::perform_next_io<hdd_fcfs> ( hdd_fcfs & );
    friend double ::test_avg_seek<hdd_fcfs> ( hdd_fcfs & , unsigned long long );
public:
    explicit hdd_fcfs( int hdd_size_in_GB ) : hdd_base( hdd_size_in_GB ) {};
    ~hdd_fcfs()
    {
        std::cout << std::endl << "HDD FCFS" << std::endl;
        std::cout << "missed_io: " << missed_io << std::endl;
        std::cout << "max_controller_que_size: " << max_controller_que_size << std::endl;
        std::cout << "Used track count: " << avg_time_for_track.size() << std::endl;
        std::cout << "Average controller que size: " << avg_controller_que_size << std::endl;
    };
    double test_avg_seek( unsigned long long );
};

#endif // ACRONIS_HDD_FCFS_H
