#include "hdd_fcfs.h"

map< double, pair< unsigned int, unsigned int > >::iterator
    hdd_fcfs::get_io_task_from_que()
{
    auto next_io = controller_que.begin();
    return next_io;
}

double hdd_fcfs::perform_next_io()
{
    return ::perform_next_io<> ( *this );
}

double hdd_fcfs::test_avg_seek( unsigned long long attempt_count )
{
    avg_controller_que_size = 0;
    missed_io = 0;
    double avg_seek = ::test_avg_seek<> ( *this , attempt_count );
    avg_controller_que_size = avg_controller_que_size / attempt_count;
    return avg_seek;
}
