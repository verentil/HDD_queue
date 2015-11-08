#include "test.h"

const string test::fcfs_distribution_output_filename = "test_distribution_fcfs.bin";
const string test::ssf_distribution_output_filename = "test_distribution_ssf.bin";
const string test::elevator_distribution_output_filename = "test_distribution_elevator.bin";
const string test::params_distribution_output_filename = "test_distribution_params.bin";
const string test::default_test_by_intension_filename = "test_by_intension.bin";
const string test::default_test_convergence_filename = "test_convergence.bin";

void test::by_intension ( double size_in_GB, unsigned int test_count, string output_filename )
{
    fstream fs;
    fs.open(output_filename.c_str(), ios::out | ios::binary);

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
    poisson_intension = default_poisson_intension;
    fs.close();
}

void test::convergence ( double size_in_GB, string output_filename )
{
    fstream fs;
    fs.open(output_filename.c_str(), ios::out | ios::binary);

    poisson_intension = default_poisson_intension;
    for ( unsigned int test_count = 1000 ; test_count < 200000 ; test_count += 1000 )
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

        fs  << test_count << " "
            << fcfs.get_avg_io_seek() << " "
                << fcfs.get_overall_io_count() << " "
            << ssf.get_avg_io_seek() << " "
                << ssf.get_overall_io_count() << " "
            << elevator.get_avg_io_seek() << " "
                << elevator.get_overall_io_count() << " " << endl;

        cout << (double)test_count / 400000 << "%" << endl;
    }
    fs.close();
}

void test::distribution ( double hdd_size_in_GB )
{
    double elapsed_time;
    poisson_intension = default_poisson_intension;
    unsigned int test_count  = hdd_size_in_GB * 1e+09 / ( 2 * hdd_sector_size_in_byte );

    hdd_fcfs fcfs(hdd_size_in_GB);
    hdd_ssf ssf(hdd_size_in_GB);
    hdd_elevator elevator(hdd_size_in_GB);

    elapsed_time = GetTickCount();

    thread_fcfs = thread( &hdd_fcfs::test_avg_seek, &fcfs, test_count );
    thread_ssf = thread( &hdd_ssf::test_avg_seek, &ssf, test_count );
    thread_elevator = thread( &hdd_elevator::test_avg_seek, &elevator, test_count);
    thread_fcfs.join();
    thread_ssf.join();
    thread_elevator.join();

    elapsed_time = GetTickCount() - elapsed_time;
    elapsed_time /= 1000;

    fstream fs_fcfs;
    fstream fs_ssf;
    fstream fs_elevator;
    fs_fcfs.open(fcfs_distribution_output_filename.c_str(), ios::out | ios::binary);
    fs_ssf.open(ssf_distribution_output_filename.c_str(), ios::out | ios::binary);
    fs_elevator.open(elevator_distribution_output_filename.c_str(), ios::out | ios::binary);

    fs_fcfs << fcfs;
    fs_ssf << ssf;
    fs_elevator << elevator;

    fs_fcfs.close();
    fs_ssf.close();
    fs_elevator.close();

    fstream params;
    params.open(params_distribution_output_filename.c_str(), ios::out | ios::binary);

    params  << fcfs.get_overall_io_count() << " "
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
                << elevator.get_missed_io_count() << " "
                << elapsed_time << endl;

    params.close();

    cout << "Elapsed time: " << (int)elapsed_time << "seconds." << endl;
}

void test::avg_seek ( double hdd_size_in_GB, unsigned int test_count )
{
    hdd_fcfs fcfs(hdd_size_in_GB);
    hdd_ssf ssf(hdd_size_in_GB);
    hdd_elevator elevator(hdd_size_in_GB);

    double elapsed_time = GetTickCount();

    thread_fcfs = thread( &hdd_fcfs::test_avg_seek, &fcfs, test_count );
    thread_ssf = thread( &hdd_ssf::test_avg_seek, &ssf, test_count );
    thread_elevator = thread( &hdd_elevator::test_avg_seek, &elevator, test_count);
    thread_fcfs.join();
    thread_ssf.join();
    thread_elevator.join();

    elapsed_time = GetTickCount() - elapsed_time;
    elapsed_time /= 1000;

    cout    << "Average seek time for HDD with FCFS algorithm:\t\t"
                << fcfs.get_avg_io_seek() << "ms" << endl
            << "Average seek time for HDD with SSF algorithm:\t\t"
                << ssf.get_avg_io_seek() << "ms" << endl
            << "Average seek time for HDD with Elevator algorithm:\t"
                << elevator.get_avg_io_seek() << "ms" << endl;

    cout << "Elapsed time: " << (int)elapsed_time << " seconds." << endl;
}
