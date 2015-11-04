#include <iostream>

#include "hdd_fcfs.h"

const short hdd_controller_que_size = 8;
const unsigned int hdd_track_count = 16383;
const unsigned int hdd_sector_size_in_byte = 512;
const unsigned int puasson_intension = 70;

using namespace std;

int main()
{
    double one_rate_time = 6e+4 / 7200;
    cout << "one_rate_time: " << one_rate_time << endl;
    hdd_fcfs hdd(40);
    double avg = hdd.test_avg_seek(20);
    cout << "avg: " << avg << endl;

    return 0;
}
