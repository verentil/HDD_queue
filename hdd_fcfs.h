#ifndef ACRONIS_HDD_FCFS_H
#define ACRONIS_HDD_FCFS_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <random>

using std::map;
using std::pair;
using std::default_random_engine;

extern const short hdd_controller_que_size;
extern const unsigned int hdd_track_count;
extern const unsigned int hdd_sector_size_in_byte;
extern const unsigned int poisson_intension;

class hdd_fcfs
{
    static const int rpm;
    static const double track_to_track_seek_time;
    double one_rate_time;
    double one_track_change_time;
    unsigned int sector_count;
    unsigned int current_track = 0;
    unsigned int current_sector = 1;
    double current_time = 0;
    unsigned long long missed_io = 0;
    unsigned int max_controller_que_size = 0;
    map< double, pair< unsigned int, unsigned int > > controller_que;
    map< unsigned int, pair< double, unsigned long long > > avg_time_for_track;
    default_random_engine random_uint;

    hdd_fcfs(){};                                 // HDD should have a size.
    double next_poisson_time_step();
    void add_io_task_to_que( double );
    map< double, pair< unsigned int, unsigned int > >::iterator
        get_io_task_from_que();
    double move_heads( unsigned int, unsigned int );
    double perform_next_io();
    void update_avg_time( double );
public:
    explicit hdd_fcfs( int );
    ~hdd_fcfs()
    {
        std::cout << "missed_io: " << missed_io << std::endl;
        std::cout << "max_controller_que_size: " << max_controller_que_size << std::endl;
    };
    double get_avg_io_seek();
    double test_avg_seek( unsigned long long );
};

#endif // ACRONIS_HDD_FCFS_H
