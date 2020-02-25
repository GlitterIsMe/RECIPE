//
// Created by 張藝文 on 2020/2/25.
//

#ifndef RECIPE_CACHE_METER_H
#define RECIPE_CACHE_METER_H
#include <cstring>
#include <string>
#include <unordered_map>
#include <random>
#include <map>
#include <fstream>
#include <iostream>

#include <pthread.h>
#ifdef NO_USE_PAPI

/*
 * class CacheMeter - Placeholder for systems without PAPI
 */
class CacheMeter {
 public:
  CacheMeter() {};
  CacheMeter(bool) {};
  ~CacheMeter() {}
  void Start() {};
  void Stop() {};
  void PrintL3CacheUtilization() {};
  void PrintL1CacheUtilization() {};
  void GetL3CacheUtilization() {};
  void GetL1CacheUtilization() {};
};

#else

// This requires adding PAPI library during compilation
// The linking flag of PAPI is:
//   -lpapi
// To install PAPI under ubuntu please use the following command:
//   sudo apt-get install libpapi-dev
#include <papi.h>

/*
 * class CacheMeter - Measures cache usage using PAPI library
 *
 * This class is a high level encapsulation of the PAPI library designed for
 * more comprehensive profiling purposes, only using a small feaction of its
 * functionalities available. Also, the applicability of this library is highly
 * platform dependent, so please check whether the platform is supported before
 * using
 */
class CacheMeter {
private:
    // This is a list of events that we care about
    int event_list[6] = {
            PAPI_LD_INS,       // Load instructions
            PAPI_L1_LDM,       // L1 load misses

            PAPI_SR_INS,       // Store instructions
            PAPI_L1_STM,       // L1 store misses

            PAPI_L3_TCA,       // L3 total cache access
            PAPI_L3_TCM,       // L3 total cache misses
    };

    // Use the length of the event_list to compute number of events we
    // are counting
    static constexpr int EVENT_COUNT = sizeof(event_list) / sizeof(int);

    // A list of results collected from the hardware performance counter
    long long counter_list[EVENT_COUNT];

    // Use this to print out event names
    const char *event_name_list[EVENT_COUNT] = {
            "PAPI_LD_INS",
            "PAPI_L1_LDM",
            "PAPI_SR_INS",
            "PAPI_L1_STM",
            "PAPI_L3_TCA",
            "PAPI_L3_TCM",
    };

    // The level of information we need to collect
    int level;

    /*
     * CheckEvent() - Checks whether the event exists in this platform
     *
     * This function wraps PAPI function in C++. Note that PAPI events are
     * declared using anonymous enum which is directly translated into int type
     */
    inline bool CheckEvent(int event) {
        int ret = PAPI_query_event(event);
        return ret == PAPI_OK;
    }

    /*
     * CheckAllEvents() - Checks all events that this object is going to use
     *
     * If the checking fails we just exit with error message indicating which one
     * failed
     */
    void CheckAllEvents() {
        // If any of the required events do not exist we just exit
        for(int i = 0;i < level;i++) {
            if(CheckEvent(event_list[i]) == false) {
                fprintf(stderr,
                        "ERROR: PAPI event %s is not supported\n",
                        event_name_list[i]);
                exit(1);
            }
        }

        return;
    }

public:

    /*
     * CacheMeter() - Initialize PAPI and events
     *
     * This function starts counting if the argument passed is true. By default
     * it is false
     */
    CacheMeter(bool start=false, int p_level=2) :
            level{p_level} {
        int ret = PAPI_library_init(PAPI_VER_CURRENT);

        if (ret != PAPI_VER_CURRENT) {
            fprintf(stderr, "ERROR: PAPI library failed to initialize\n");
            exit(1);
        }

        // Initialize pthread support
        ret = PAPI_thread_init(pthread_self);
        if(ret != PAPI_OK) {
            fprintf(stderr, "ERROR: PAPI library failed to initialize for pthread\n");
            exit(1);
        }

        // If this does not pass just exit
        CheckAllEvents();

        // If we want to start the counter immediately just test this flag
        if(start == true) {
            Start();
        }

        return;
    }

    /*
     * Destructor
     */
    ~CacheMeter() {
        PAPI_shutdown();

        return;
    }

    /*
     * Start() - Starts the counter until Stop() is called
     *
     * If counter could not be started we just fail
     */
    void Start() {
        int ret = PAPI_start_counters(event_list, level);
        // Start counters
        if (ret != PAPI_OK) {
            fprintf(stderr,
                    "ERROR: Failed to start counters using"
                    " PAPI_start_counters() (%d)\n",
                    ret);
            exit(1);
        }

        return;
    }

    /*
     * Stop() - Stops all counters, and dump their values inside the local array
     *
     * This function will clear all counters after dumping them into the internal
     * array of this object
     */
    void Stop() {
        // Use counter list to hold counters
        if (PAPI_stop_counters(counter_list, level) != PAPI_OK) {
            fprintf(stderr,
                    "ERROR: Failed to start counters using PAPI_stop_counters()\n");
            exit(1);
        }

        // Store zero to all unused counters
        for(int i = level;i < EVENT_COUNT;i++) {
            counter_list[i] = 0LL;
        }

        return;
    }

    /*
     * GetL3CacheUtilization() - Returns L3 total cache accesses and misses
     *
     * These two values are returned in a tuple, the first element of which being
     * total cache accesses and the second element being L3 cache misses
     */
    std::pair<long long, long long> GetL3CacheUtilization() {
        return std::make_pair(counter_list[4], counter_list[5]);
    }

    /*
     * GetL1CacheUtilization() - Returns L1 cache utilizations
     */
    std::pair<long long, long long> GetL1CacheUtilization() {
        return std::make_pair(counter_list[0] + counter_list[2],
                              counter_list[1] + counter_list[3]);
    }

    /*
     * PrintL3CacheUtilization() - Prints L3 cache utilization
     */
    void PrintL3CacheUtilization() {
        // Return L3 total accesses and cache misses
        auto l3_util = GetL3CacheUtilization();

        std::cout << "    L3 total = " << l3_util.first << "; miss = " \
              << l3_util.second << "; hit ratio = " \
              << static_cast<double>(l3_util.first - l3_util.second) / \
                 static_cast<double>(l3_util.first) \
              << std::endl;

        return;
    }

    /*
     * PrintL1CacheUtilization() - Prints L1 cache utilization
     */
    void PrintL1CacheUtilization() {
        // Return L3 total accesses and cache misses
        auto l1_util = GetL1CacheUtilization();

        std::cout << "    LOAD/STORE total = " << l1_util.first << "; miss = " \
              << l1_util.second << "; hit ratio = " \
              << static_cast<double>(l1_util.first - l1_util.second) / \
                 static_cast<double>(l1_util.first) \
              << std::endl;

        return;
    }
};

#endif

#endif //RECIPE_CACHE_METER_H
