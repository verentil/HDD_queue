#ifndef ACRONIS_FUNCTION_TEMPLATES_H
#define ACRONIS_FUNCTION_TEMPLATES_H

template <class hdd_type>
double perform_next_io( hdd_type & hdd )
{
    auto next_io = hdd.get_io_task_from_que();
    if ( hdd.current_time < next_io->first )
        hdd.current_time = next_io->first;

    double elapsed_time = hdd.move_heads( next_io->second.first, next_io->second.second );
    double overall_elapsed_time = elapsed_time + ( hdd.current_time - next_io->first );
    hdd.update_avg_time( overall_elapsed_time );

    hdd.controller_que.erase(next_io);

    return elapsed_time;
}

template <class hdd_type>
double test_avg_seek( hdd_type & hdd, unsigned long long attempt_count )
{
    double next_io_time = hdd.current_time;
    for ( unsigned long long i = 0 ; i < attempt_count ; ++i )
    {
        while ( next_io_time <= hdd.current_time )
        {
            next_io_time += hdd.next_poisson_time_step();
            hdd.add_io_task_to_que( next_io_time );
        }
        while ( next_io_time > hdd.current_time )
        {
            if ( hdd.controller_que.size() == 0 )
            {
                hdd.current_time = next_io_time;
                break;
            }
            double elapsed_time = hdd.perform_next_io();
            hdd.current_time += elapsed_time;
        }
    }
    return hdd.get_avg_io_seek();
}

#endif // ACRONIS_FUNCTION_TEMPLATES_H
