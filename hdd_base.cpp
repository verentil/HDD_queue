#include "hdd_base.h"

const int hdd_base::rpm = 7200;
const double hdd_base::track_to_track_seek_time = 1.0;                // Read 1 ms. Write 1.2 ms.

hdd_base::hdd_base( int hdd_size_in_GB )
{
    sector_count = hdd_size_in_GB * 1e+09                           // 1GB = 1e+09 Bytes.
                    / ( hdd_sector_size_in_byte * hdd_track_count );
    one_track_change_time = track_to_track_seek_time * 2 / hdd_track_count ;
    one_rate_time = 6e+04 / rpm;                                    // 1 minute = 6e+04 ms.
}

double hdd_base::next_poisson_time_step()
{
    double poisson_probability = (double) ( rand() % ( RAND_MAX - 2 ) + 1 ) / RAND_MAX;
    double io_time_step = - log( poisson_probability ) * 1000 / poisson_intension;
    return io_time_step;
}

void hdd_base::add_io_task_to_que( double next_io_time )
{
    if ( controller_que.size() < hdd_controller_que_size )
    {
        unsigned int next_io_track = random_uint() % hdd_track_count - random_uint.min();
        unsigned int next_io_sector = random_uint() % sector_count;
        controller_que.insert( pair< double, pair< unsigned int, unsigned int > >
                                (   next_io_time,
                                    pair< unsigned int, unsigned int >
                                    (   next_io_track,
                                        next_io_sector )
                                ) );
        if ( controller_que.size() > max_controller_que_size )
            max_controller_que_size = controller_que.size();
    } else {
        ++missed_io;
    }
}

double hdd_base::move_heads( unsigned int new_track, unsigned int new_sector )
{
    double track_changing_time = abs( new_track - current_track ) * one_track_change_time;
    double sector_waiting_time = abs( new_sector - current_sector ) * one_rate_time
                                    / sector_count;
    double elapsed_time = track_changing_time + sector_waiting_time;

    current_track = new_track;
    current_sector = new_sector;

    return elapsed_time;
}

void hdd_base::update_avg_time( double last_io_time )
{
    auto appropriate_unit = avg_time_for_track.find( current_track );
    if ( appropriate_unit == avg_time_for_track.end() )
    {
        avg_time_for_track.insert( pair< unsigned int, pair< double, unsigned long long > >
                                    (   current_track,
                                        pair< double, unsigned long long >( last_io_time, 1 )
                                    ) );
    } else
    {
        appropriate_unit->second.first += last_io_time;
        appropriate_unit->second.second++;
    }
}

double hdd_base::get_avg_io_seek()
{
    double overall_io_time = 0;
    unsigned long long overall_io_count = 0;
    for ( auto track_time : avg_time_for_track )
    {
        overall_io_time += track_time.second.first;
        overall_io_count += track_time.second.second;
    }
    double avg_io_seek = overall_io_time / overall_io_count;
    return avg_io_seek;
}

ostream & operator << (ostream & os , hdd_base &hdd)
{
    for ( auto track : hdd.avg_time_for_track )
    {
        os  << track.first << " "
            << track.second.first << " "
            << track.second.second << " "
            << std::endl;
    }
    return os;
}
