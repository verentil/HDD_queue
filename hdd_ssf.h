#ifndef ACRONIS_HDD_SSF_H
#define ACRONIS_HDD_SSF_H

#include <iostream>
#include <map>

#include "hdd_base.h"
#include "function_templates.h"

using std::map;
using std::pair;
using std::default_random_engine;

extern const unsigned int hdd_track_count;

class hdd_ssf: public hdd_base
{
    hdd_ssf(){};                                 // HDD should have a size.
    map< double, pair< unsigned int, unsigned int > >::iterator
        get_io_task_from_que();
    double perform_next_io();
    friend double ::perform_next_io<hdd_ssf> ( hdd_ssf & );
    friend double ::test_avg_seek<hdd_ssf> ( hdd_ssf & , unsigned long long );
public:
    explicit hdd_ssf( int hdd_size_in_GB ) : hdd_base( hdd_size_in_GB ) {};
    ~hdd_ssf()
    {
        std::cout << std::endl << "HDD SSF" << std::endl;
        std::cout << "missed_io: " << missed_io << std::endl;
        std::cout << "max_controller_que_size: " << max_controller_que_size << std::endl;
        std::cout << "Used track count: " << avg_time_for_track.size() << std::endl;
        std::cout << "Average controller que size: " << avg_controller_que_size << std::endl;
    };
    double test_avg_seek( unsigned long long );
};

#endif // ACRONIS_HDD_SSF_H
