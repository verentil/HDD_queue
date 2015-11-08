#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <boost/thread.hpp>
#include <fstream>
#include <iostream>
#include <windows.h>

#include "hdd_fcfs.h"
#include "hdd_ssf.h"
#include "hdd_elevator.h"

extern const unsigned int default_poisson_intension;
extern const unsigned int hdd_sector_size_in_byte;

using namespace std;
using boost::thread;

class test
// This class provides some methods for test HDD controller queue algorithms.
// Multithreading realised by boost::thread library and concept is "one hdd_type - one thread".
{
    thread thread_fcfs, thread_ssf, thread_elevator;
    static const string fcfs_distribution_output_filename;
    static const string ssf_distribution_output_filename;
    static const string elevator_distribution_output_filename;
    static const string params_distribution_output_filename;
    static const string default_test_by_intension_filename;
    static const string default_test_convergence_filename;
public:
    void by_intension ( double, unsigned int ,  string output_filename = default_test_by_intension_filename );
    // This method tests stability of the model by increasing Poisson intension from 50 IOPS to 300 IOPS.
    // Results will be written to a binary file which name contained at "output_filename" string variable.
    // The method has overload without "test_count" argument.
    void by_intension ( double hdd_size_in_GB,  string output_filename = default_test_by_intension_filename )
    {
        unsigned int test_count  = hdd_size_in_GB * 1e+09 / ( 2 * hdd_sector_size_in_byte );         // 1GB = 1e+09 Bytes.
        by_intension ( hdd_size_in_GB, test_count, output_filename );
    }
    void convergence ( double ,string output_filename = default_test_convergence_filename );
    // This method tests convergence of the model by increasing IOPS count.
    // Results will be written to a binary file which name contained at "output_filename" string variable.
    void distribution ( double );
    // This method displays distribution of a IO operations between various tracks of HDD.
    // Results will be written to a binary files that defined by static constant members.
    void avg_seek ( double , unsigned int );
    // This method displays average seek time for every used HDD controller queue algorithms.
    // Results will be written to the std::cout.
    // The method has overload without "test_count" argument.
    void avg_seek ( double hdd_size_in_GB )
    {
        unsigned int test_count  = hdd_size_in_GB * 1e+09 / ( 2 * hdd_sector_size_in_byte );         // 1GB = 1e+09 Bytes.
        avg_seek ( hdd_size_in_GB, test_count );
    }
};

#endif // TEST_H_INCLUDED
