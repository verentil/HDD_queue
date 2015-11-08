#ifndef ACRONIS_HDD_ELEVATOR_H
#define ACRONIS_HDD_ELEVATOR_H

#include <iostream>
#include <map>

#include "hdd_base.h"
#include "function_templates.h"

using std::map;
using std::pair;
using std::default_random_engine;

class hdd_elevator: public hdd_base
// This class implements the Elevator algorithm.
{
    const bool to_center = true;
    bool elevator_direction = to_center;                // Default direction to center, because default track location is track with number 0.
    hdd_elevator(){};                                   // HDD should have a size.
    map< double, pair< unsigned int, unsigned int > >::iterator
        get_io_task_from_que();
        // Returns an iterator to next IO request at controller queue.
    double perform_next_io();
    // Wrapping template function
    friend double ::perform_next_io<hdd_elevator> ( hdd_elevator & );
    // Template function to not overwrite same code. Has direct access to some members. Implementation see at "function_templates.h" file.
    friend double ::test_avg_seek<hdd_elevator> ( hdd_elevator & , unsigned long long );
    // Template function to not overwrite same code. Has direct access to some members. Implementation see at "function_templates.h" file.
public:
    explicit hdd_elevator( double hdd_size_in_GB ) : hdd_base( hdd_size_in_GB ) {};
    ~hdd_elevator(){};
    double test_avg_seek( unsigned long long );
    // Wrapping template function
};

#endif // ACRONIS_HDD_ELEVATOR_H
