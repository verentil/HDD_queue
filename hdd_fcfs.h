#ifndef ACRONIS_HDD_FCFS_H
#define ACRONIS_HDD_FCFS_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>

using std::map;
using std::pair;

extern const short hdd_controller_que_size;
extern const unsigned int hdd_track_count;
extern const unsigned int hdd_sector_size_in_byte;
extern const unsigned int puasson_intension;

class hdd_fcfs
{
    static const int rpm;
    static const double track_to_track_seek_time;
    unsigned int sector_count;
    unsigned int current_track = 0;
    unsigned int current_sector = 1;
    unsigned long long current_time = 0;
    unsigned long long missed_io = 0;
    map< unsigned long long, pair< unsigned int, unsigned int > > controller_que;
    map< unsigned int, pair< double, unsigned long long > > avg_time_for_track;
    hdd_fcfs(){};                                 // HDD should have a size.
public:
    explicit hdd_fcfs( int );
    ~hdd_fcfs(){ std::cout << "missed_io: " << missed_io << std::endl; };
    double test_avg_seek( unsigned long long );
};

#endif // ACRONIS_HDD_FCFS_H
