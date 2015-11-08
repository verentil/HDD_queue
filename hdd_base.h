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
using std::ostream;

extern const short hdd_controller_que_size;
extern const unsigned int hdd_track_count;
extern const unsigned int hdd_sector_size_in_byte;
extern unsigned int poisson_intension;

class hdd_base
// This class is a parent class for all classes which performs various HDD controller queue algorithms.
// This class defines all required methods and logic for emulate HDD IO request execution.
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
    unsigned long long missed_io = 0;       // This variable calculates number of IO request that doesn't served.
    double avg_controller_que_size = 0;
    unsigned int max_controller_que_size = 0;
    map< double, pair< unsigned int, unsigned int > > controller_que;
    // This map is HDD controller queue with following structure:
    // a key (double) - IO request coming time
    // a second value (pair):
    //      first value (unsigned int) - request track number
    //      second value (unsigned int) - request sector number.
    unordered_map< unsigned int, pair< double, unsigned long long > > avg_time_for_track;
    // This map is statistic container for every track with following structure:
    // a key (unsigned int) - track number
    // a second value (pair):
    //      first value (double) - sum of serve time for all IO request with this track number
    //      second value (unsigned long long) - count of IO request with this track number.
    default_random_engine random_uint;
    // Pseudorandom class generator.

    hdd_base(){};                                 // HDD should have a size.
    double next_poisson_time_step();
    // Returns time interval before next IO request received.
    // Implements Poisson stream.
    void add_io_task_to_que( double );
    // Generates request with random track number and sector number, push it to the HDD controller queue (the "controller_que" map).
    double move_heads( unsigned int, unsigned int );
    // Moves disk heads (changing "current_track", "current_sector") and returns required time for this.
    void update_avg_time( double );
    // Updates the "avg_time_for_track" map for a "current_track" track number with value which is time required for last IO request.

    virtual map< double, pair< unsigned int, unsigned int > >::iterator
        get_io_task_from_que() = 0;
        // This method implements HDD controller queue algorithm. Pure virtual method.
        // Returns an iterator to next IO request at controller queue.
    virtual double perform_next_io() = 0;
    // This method perform next IO request from queue. Returns required time.
public:
    explicit hdd_base( double );
    ~hdd_base(){};
    double get_avg_io_seek();
    unsigned int get_max_controller_que_size()
    {
        return  max_controller_que_size;
    };
    double get_avg_controller_que_size()
    {
        return  avg_controller_que_size;
    };
    unsigned long long get_missed_io_count()
    {
        return missed_io;
    };
    friend ostream & operator << (ostream & , hdd_base & );
    unsigned long long get_overall_io_count();

    virtual double test_avg_seek( unsigned long long ) = 0;
    // The main method that implements all job.
};


#endif // ACRONIS_HDD_BASE_H
