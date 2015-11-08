#include <iostream>

#include "test.h"

const short hdd_controller_que_size = 16;
const unsigned int hdd_track_count = 16383;
const unsigned int hdd_sector_size_in_byte = 512;
const unsigned int default_poisson_intension = 111;
unsigned int poisson_intension = default_poisson_intension;

using namespace std;

int main()
{
    test hdd_test;
    double size_in_GB = 0;

    cout    << "Welcome!" << endl
            << "Insert HDD volume in GB. Example: 100 or 0.2" << endl;
    cin >> size_in_GB;

    if ( size_in_GB <= 0 )
    {
        cout << "The HDD volume must be a positive number. Try once again." << endl;
        cin >> size_in_GB;
    }

    if ( size_in_GB > 0 )
    {
        cout << endl << "Test results." << endl;
        hdd_test.avg_seek( size_in_GB );
    }

    cout << "Goodbye." << endl;

    return 0;
}
