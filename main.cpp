#include <boost/thread.hpp>
#include <fstream>
#include <iostream>

#include "hdd_fcfs.h"
#include "hdd_ssf.h"
#include "hdd_elevator.h"

const short hdd_controller_que_size = 300;
const unsigned int hdd_track_count = 16383;
const unsigned int hdd_sector_size_in_byte = 512;
//const unsigned int poisson_intension = 285;
unsigned int poisson_intension = 285;

using namespace std;
using boost::thread;

void test_by_intension ( fstream & fs, unsigned int test_count, unsigned int size_in_GB )
{
    thread thread_fcfs, thread_ssf, thread_elevator;
    for ( poisson_intension = 50 ; poisson_intension <= 300 ; poisson_intension +=10 )
    {
        hdd_fcfs fcfs(size_in_GB);
        hdd_ssf ssf(size_in_GB);
        hdd_elevator elevator(size_in_GB);

        thread_fcfs = thread( &hdd_fcfs::test_avg_seek, &fcfs, test_count );
        thread_ssf = thread( &hdd_ssf::test_avg_seek, &ssf, test_count );
        thread_elevator = thread( &hdd_elevator::test_avg_seek, &elevator, test_count);
        thread_fcfs.join();
        thread_ssf.join();
        thread_elevator.join();

        fs  << poisson_intension << " "
            << fcfs.get_avg_io_seek() << " "
                << fcfs.get_max_controller_que_size() << " "
                << fcfs.get_avg_controller_que_size() << " "
                << fcfs.get_missed_io_count() << " "
            << ssf.get_avg_io_seek() << " "
                << ssf.get_max_controller_que_size() << " "
                << ssf.get_avg_controller_que_size() << " "
                << ssf.get_missed_io_count() << " "
            << elevator.get_avg_io_seek() << " "
                << elevator.get_max_controller_que_size() << " "
                << elevator.get_avg_controller_que_size() << " "
                << elevator.get_missed_io_count() << endl;
    }
}

int main()
{

    unsigned int test_count = 4000000;
    unsigned int size_in_GB = 2;

    fstream fs;
    fs.open("test_by_intension.bin", ios::out | ios::binary);

    test_by_intension ( fs, test_count, size_in_GB );

    fs.close();

    return 0;
}
