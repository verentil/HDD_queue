#include "hdd_ssf.h"

map< double, pair< unsigned int, unsigned int > >::iterator
    hdd_ssf::get_io_task_from_que()
{
    auto next_io = controller_que.begin();
    int shortest_path = hdd_track_count;
    for ( auto candidate = controller_que.begin() ; candidate != controller_que.end() ; ++candidate )
    {
        if ( abs( candidate->second.first - current_track ) < shortest_path )
        {
            shortest_path = abs( candidate->second.first - current_track );
            next_io = candidate;
        }
    }
    return next_io;
}

double hdd_ssf::perform_next_io()
{
    return ::perform_next_io<> ( *this );
}

double hdd_ssf::test_avg_seek( unsigned long long attempt_count )
{
    avg_controller_que_size = 0;
    missed_io = 0;
    double avg_seek = ::test_avg_seek<> ( *this , attempt_count );
    avg_controller_que_size = avg_controller_que_size / attempt_count;
    return avg_seek;
}
