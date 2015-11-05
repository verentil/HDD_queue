#include "hdd_fcfs.h"

const int hdd_fcfs::rpm = 7200;
const double hdd_fcfs::track_to_track_seek_time = 1;                // Read 1 ms. Write 1.2 ms.

hdd_fcfs::hdd_fcfs( int hdd_size_in_GB )
{
    sector_count = hdd_size_in_GB * 1e+09                           // 1GB = 1e+09 Bytes.
                    / ( hdd_sector_size_in_byte * hdd_track_count );
    one_track_change_time = track_to_track_seek_time * 2 / hdd_track_count ;
    one_rate_time = 6e+04 / rpm;                                    // 1 minute = 6e+04 ms.
}

double hdd_fcfs::next_poisson_time_step()
{
    double poisson_probability = (double) rand() / RAND_MAX;
    double io_time_step = - log( poisson_probability ) * 1000 / poisson_intension;
    return io_time_step;
}

void hdd_fcfs::add_io_task_to_que( double next_io_time )
{
    if ( controller_que.size() < hdd_controller_que_size )
    {
        unsigned int next_io_track = rand() % hdd_track_count;
        unsigned int next_io_sector = rand() % sector_count + 1;
        controller_que.insert( pair< double, pair< unsigned int, unsigned int > >
                                (   next_io_time,
                                    pair< unsigned int, unsigned int >
                                    (   next_io_track,
                                        next_io_sector )
                                ) );
        if ( controller_que.size() > max_controller_que_size )
            max_controller_que_size = controller_que.size();
    } else {
        std::cout << "Quit from que size!" << std::endl;
        ++missed_io;
    }
}

map< double, pair< unsigned int, unsigned int > >::iterator
    hdd_fcfs::get_io_task_from_que()
{
    auto next_io = controller_que.begin();
    return next_io;
}

double hdd_fcfs::move_heads( unsigned int new_track, unsigned int new_sector )
{
    double track_changing_time = abs( new_track - current_track ) * one_track_change_time;
    double sector_waiting_time = abs( new_sector - current_sector ) * one_rate_time
                                    / sector_count;
    double elapsed_time = track_changing_time + sector_waiting_time;

    current_track = new_track;
    current_sector = new_sector;

    return elapsed_time;
}

void hdd_fcfs::update_avg_time( double last_io_time )
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

double hdd_fcfs::perform_next_io()
{
    auto next_io = get_io_task_from_que();
    if ( current_time < next_io->first )
        current_time = next_io->first;

    double elapsed_time = move_heads( next_io->second.first, next_io->second.second );
    double overall_elapsed_time = elapsed_time + ( current_time - next_io->first );
    update_avg_time( overall_elapsed_time );

    controller_que.erase(next_io);

    return elapsed_time;
}

double hdd_fcfs::get_avg_io_seek()
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

double hdd_fcfs::test_avg_seek( unsigned long long attempt_count )
{
    double next_io_time = current_time;
    while ( attempt_count > 0 )
    {
        while ( next_io_time <= current_time )
        {
            next_io_time += next_poisson_time_step();
            add_io_task_to_que( next_io_time );
        }
        while ( next_io_time > current_time )
        {
            if ( controller_que.size() == 0 )
            {
                current_time = next_io_time;
                break;
            }
            double elapsed_time = perform_next_io();
            current_time += elapsed_time;
        }
        --attempt_count;
        std::cout << "attempt count: " << attempt_count << std::endl;
    }
    return get_avg_io_seek();
}
