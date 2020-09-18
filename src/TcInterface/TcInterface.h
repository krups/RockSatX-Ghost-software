#ifndef TcInterface_h
#define TcInterface_h

#if defined(ADAFRUIT_TRINKET_M0) || defined(__MK64FX512__)

// switch pin defs based depending on if this
// code is running on the safety processor or the 
// main Teensy 3.5
#if defined(ADAFRUIT_TRINKET_M0)
  #define MOSI        4
  #define SCK         3
  #define MISO        2
  #define CS_TC1      1
  #define CS_TC2      13
  #define SEC_CTRL_2  0
  #define SEC_ACT     7
  #define SEC_CTRL_1  8
  #define MUX0        23
  #define MUX1        24
  #define TC1_FAULT   21
  #define TC2_FAULT   22
  #define SPI_US SEC_CTRL_2
  #define SPI_THEM SEC_CTRL_1
#elif defined(__MK64FX512__) // teensy 3.5
  #define CS_TC1 20
  #define CS_TC2 21
  #define SEC_CTRL_2  27
  #define SEC_CTRL_1  39
  #define MUX0 16
  #define MUX1 17
  #define TC1_FAULT 25
  #define TC2_FAULT 26
  #define SPI_US SEC_CTRL_1
  #define SPI_THEM SEC_CTRL_2
#else
//#error "Unsupported TC pin config, plz fix TcInterface.h"
#endif

#include <Adafruit_MAX31856.h>

// Encapsulate selection and reading of thermocouple values
class TcInterface
{
    public:
        TcInterface( char* tc_types, // String of thermocouple types (B, E, J, K, N, R, S, or T)
            int cs_tc1 = CS_TC1, // Chip select pin for "TC1" MAX31856 
            int cs_tc2 = CS_TC2, // Chip select pin for "TC2" MAX31856 
            int mux0 = MUX0,  // Input for mux to select thermocouple output
            int mux1 = MUX1,  // Input for mux to select thermocouple output
            int tc1_fault = TC1_FAULT, // Fault pin for "TC1" MAX31856
            int tc2_fault = TC2_FAULT // Fault pin for "TC2" MAX31856
        );
        void enable(void);
        void disable(void);
        bool read_all(float* arr);
        void triggerOneShot(int tc);
        bool conversionComplete(int tc);
        float readThermocoupleTemperature(int tc);

        // Checks if either sensor has a fault.
        // Returns a uint16_t where the high byte is the fault in TC1
        // and the low byte is the fault in TC2
        uint16_t check_faults(void);

        // Sets appropriate mux outputs to select given tc according to the following mapping:
        //  MUX0 | MUX1 | TC
        //--------------------
        //    0  |  0   | 1, 5
        //    0  |  1   | 2, 6
        //    1  |  0   | 3, 7
        //    1  |  1   | 4, 8
        void select_tc(int tc);

    private:
        char* tc_types;
        int cs_tc1; 
        int cs_tc2;
        Adafruit_MAX31856 max1;// U13 TC to Digital - connected to IC1 mux output of TC 1-4
        Adafruit_MAX31856 max2;// U12 TC to Digital - connected to IC3 mux output of TC 5-8
        int mux0;// Input for mux to select thermocouple output
        int mux1;// Input for mux to select thermocouple output
        int tc1_fault; // Fault pin for "TC1" MAX31856
        int tc2_fault;         
        char tc_type_lookup[8];
        void set_types_from_chars(char* tc_types);
        Adafruit_MAX31856& get_max_from_tc(int tc);
};
#endif
#endif