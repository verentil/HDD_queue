#ifndef ACRONIS_HDD_BASE_H
#define ACRONIS_HDD_BASE_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <unordered_map>
#include <random>

using std::map;
using std::unordered_map;
using std::pair;
using std::default_random_engine;

extern const short hdd_controller_que_size;
extern const unsigned int hdd_track_count;
extern const unsigned int hdd_sector_size_in_byte;
extern const unsigned int poisson_intension;

class hdd_base
{
protected:
    static const int rpm;
    static const double track_to_track_seek_time;
    double one_rate_time;
    double one_track_change_time;
    unsigned int sector_count;
    unsigned int current_track = 0;
    unsigned int current_sector = 1;
    double current_time = 0;
    unsigned long long missed_io = 0;
    double avg_controller_que_size = 0;
    unsigned int max_controller_que_size = 0;
    map< double, pair< unsigned int, unsigned int > > controller_que;
    unordered_map< unsigned int, pair< double, unsigned long long > > avg_time_for_track;
    default_random_engine random_uint;

    hdd_base(){};                                 // HDD should have a size.
    double next_poisson_time_step();
    void add_io_task_to_que( double );
    double move_heads( unsigned int, unsigned int );
    void update_avg_time( double );

    virtual map< double, pair< unsigned int, unsigned int > >::iterator
        get_io_task_from_que() = 0;
    virtual double perform_next_io() = 0;
public:
    explicit hdd_base( int );
    ~hdd_base(){};
    double get_avg_io_seek();

    virtual double test_avg_seek( unsigned long long ) = 0;
};


#endif // ACRONIS_HDD_BASE_H
