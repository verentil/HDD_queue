#include "hdd_fcfs.h"

const int hdd_fcfs::rpm = 7200;
const double hdd_fcfs::track_to_track_seek_time = 1;     // Read 1 ms. Write 1.2 ms.

hdd_fcfs::hdd_fcfs( int hdd_size_in_GB )
{
    sector_count = hdd_size_in_GB * 1e+09 / ( hdd_sector_size_in_byte * hdd_track_count );
    std::cout << "hdd_sector_count: " << sector_count << std::endl;
}

double hdd_fcfs::test_avg_seek( unsigned long long attempt_count )
{
    unsigned long long next_io_time = current_time;
    while ( attempt_count > 0 )
    {
        while ( next_io_time <= current_time )
        {
            double puasson_probability = (double) rand() / RAND_MAX;
            std::cout << "puasson_probability: " << puasson_probability << std::endl;
            int io_time_step = - log( puasson_probability ) * 1000 / puasson_intension;
            next_io_time += io_time_step;
            std::cout << "current_time: " << current_time << std::endl;
            std::cout << "next_io_time: " << next_io_time << std::endl;
            if ( controller_que.size() < hdd_controller_que_size )
            {
                unsigned int next_io_track = rand() % hdd_track_count;
                unsigned int next_io_sector = rand() % sector_count + 1;
                controller_que.insert( pair< unsigned long long, pair< unsigned int, unsigned int > >
                                        ( next_io_time,
                                        pair< unsigned int, unsigned int >
                                            ( next_io_track,
                                            next_io_sector ) ) );
            } else {
                ++missed_io;
            }
        }
        std::cout << "controller_que.size: " << controller_que.size() << std::endl;
        std::cout << "current_time: " << current_time << std::endl;
        std::cout << "next_io_time: " << next_io_time << std::endl;
        while ( next_io_time > current_time )
        {
            if ( controller_que.size() == 0 )
            {
                current_time = next_io_time;
                std::cout << "Que is empty!" << std::endl;
                break;
            }
            auto next_io = controller_que.begin();
            if ( current_time < next_io->first )
                current_time = next_io->first;
            double one_track_change_time = track_to_track_seek_time * 2 / hdd_track_count ;
            double track_changing_time = abs( next_io->second.first - current_track ) * one_track_change_time;
            double one_rate_time = 6e+4 / 7200;
            double sector_waiting_time = abs( next_io->second.second - current_sector ) * one_rate_time / sector_count;
            double elapsed_time = track_changing_time + sector_waiting_time;
            current_track = next_io->second.first;
            current_sector = next_io->second.second;
            auto it = avg_time_for_track.find( current_track );
            if ( it == avg_time_for_track.end() )
            {
                double all_elapsed_time = elapsed_time + ( current_time - next_io->first );
                double avg_time = all_elapsed_time ;
                std::cout << "time before insertion: " << avg_time << std::endl;
                std::cout << "elapsed_time before insertion: " << elapsed_time << std::endl;
                std::cout << "current_time before insertion: " << current_time << std::endl;
                std::cout << "next_io->first before insertion: " << next_io->first << std::endl;
                avg_time_for_track.insert( pair< unsigned int, pair< double, unsigned long long > >
                                            ( current_track,
                                            pair< double, unsigned long long >
                                                ( avg_time, 1 ) ) );
            } else
            {
                std::cout << "else block" << std::endl;
                double all_elapsed_time = elapsed_time + ( current_time - next_io->first );
                double avg_time = it->second.first + all_elapsed_time ;
                std::cout << "time before insertion: " << avg_time << std::endl;
                std::cout << "elapsed_time before insertion: " << elapsed_time << std::endl;
                std::cout << "current_time before insertion: " << current_time << std::endl;
                std::cout << "next_io->first before insertion: " << next_io->first << std::endl;
                avg_time_for_track[ current_track ] = pair< double, unsigned long long >
                                                        ( avg_time,
                                                         it->second.second + 1 );
            }
            current_time += elapsed_time;
            controller_que.erase(next_io);
        }
        --attempt_count;
    }
    double overall_io_time = 0;
    unsigned long long overall_io_count = 0;
    for ( auto track_time : avg_time_for_track )
    {
        overall_io_time += track_time.second.first;
        overall_io_count += track_time.second.second;
    }
    std::cout << "overall_io_time: " << overall_io_time << std::endl;
    std::cout << "overall_io_count: " << overall_io_count << std::endl;
    return overall_io_time / overall_io_count;
}
