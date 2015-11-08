#include "hdd_elevator.h"

map< double, pair< unsigned int, unsigned int > >::iterator
    hdd_elevator::get_io_task_from_que()
    // Should return a next queue element at the correct direction
    // If none are available, change direction and repeat th searching process.
{
    auto next_io = controller_que.begin();
    int shortest_path = hdd_track_count;            // maximum possible value + 1
    bool not_find = true;
    for ( auto candidate = controller_que.begin() ; candidate != controller_que.end() ; ++candidate )
    {
        if ( abs( candidate->second.first - current_track ) < shortest_path )
        {
            if ( ( elevator_direction == to_center ) && ( candidate->second.first >= current_track ) )
            {
                shortest_path = candidate->second.first - current_track;
                next_io = candidate;
                not_find = false;
            }
            if ( ( elevator_direction != to_center ) && ( candidate->second.first <= current_track ) )
            {
                shortest_path = current_track - candidate->second.first;
                next_io = candidate;
                not_find = false;
            }
        }
    }
    if ( not_find )
    {
        elevator_direction = !elevator_direction;
        next_io = get_io_task_from_que();
    }
    return next_io;
}

double hdd_elevator::perform_next_io()
{
    return ::perform_next_io<> ( *this );
}

double hdd_elevator::test_avg_seek( unsigned long long attempt_count )
{
    avg_controller_que_size = 0;
    missed_io = 0;
    double avg_seek = ::test_avg_seek<> ( *this , attempt_count );
    avg_controller_que_size = avg_controller_que_size / attempt_count;
    return avg_seek;
}
