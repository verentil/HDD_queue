#include "hdd_fcfs.h"

map< double, pair< unsigned int, unsigned int > >::iterator
    hdd_fcfs::get_io_task_from_que()
{
    auto next_io = controller_que.begin();
    return next_io;
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

double hdd_fcfs::test_avg_seek( unsigned long long attempt_count )
{
    double next_io_time = current_time;
    for ( unsigned long long i = 0 ; i < attempt_count ; ++i )
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
    }
    return get_avg_io_seek();
}
