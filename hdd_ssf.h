#ifndef ACRONIS_HDD_SSF_H
#define ACRONIS_HDD_SSF_H

#include <iostream>
#include <map>

#include "hdd_base.h"
#include "function_templates.h"

using std::map;
using std::pair;
using std::default_random_engine;

extern const unsigned int hdd_track_count;

class hdd_ssf: public hdd_base
// This class implements the SSF (Shortest Seek (Time) First) algorithm.
{
    hdd_ssf(){};                                 // HDD should have a size.
    map< double, pair< unsigned int, unsigned int > >::iterator
        get_io_task_from_que();
        // Returns an iterator to next IO request at controller queue.
    double perform_next_io();
    // Wrapping template function
    friend double ::perform_next_io<hdd_ssf> ( hdd_ssf & );
    // Template function to not overwrite same code. Has direct access to some members. Implementation see at "function_templates.h" file.
    friend double ::test_avg_seek<hdd_ssf> ( hdd_ssf & , unsigned long long );
    // Template function to not overwrite same code. Has direct access to some members. Implementation see at "function_templates.h" file.
public:
    explicit hdd_ssf( double hdd_size_in_GB ) : hdd_base( hdd_size_in_GB ) {};
    ~hdd_ssf(){};
    double test_avg_seek( unsigned long long );
    // Wrapping template function
};

#endif // ACRONIS_HDD_SSF_H
