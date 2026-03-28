#ifndef COMMON_HPP
#define COMMON_HPP

#define SC_INCLUDE_FX
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <cstdint>

// Memory Mapped IPs
const uint64_t ADDR_BRAM = 0x40000000;
const uint64_t ADDR_NCC  = 0x50000000;
const uint64_t ADDR_DMA  = 0x60000000;
const uint64_t ADDR_DDR  = 0x80000000;

// NCC Dynamic Register Map
const uint64_t REG_IMG_W     = 0x00; 
const uint64_t REG_IMG_H     = 0x04; 
const uint64_t REG_TMP_W     = 0x08; 
const uint64_t REG_TMP_H     = 0x0C; 
const uint64_t ADDR_IMG_DATA = 0x10; 
const uint64_t ADDR_TMP_DATA = 0x20; 
const uint64_t REG_CTRL      = 0x30; 
const uint64_t REG_STATUS    = 0x34; 
const uint64_t ADDR_RESULTS  = 0x40; 

// Hardware Types
typedef sc_dt::sc_ufixed<8, 8>                                 pixel_t;
typedef sc_dt::sc_fixed<16, 8, sc_dt::SC_RND, sc_dt::SC_SAT>   mean_t;
typedef sc_dt::sc_fixed<18, 9, sc_dt::SC_RND, sc_dt::SC_SAT>   diff_t;
typedef sc_dt::sc_fixed<48, 32, sc_dt::SC_RND, sc_dt::SC_SAT>  accum_t;
typedef sc_dt::sc_fixed<96, 64, sc_dt::SC_RND, sc_dt::SC_SAT>  sq_accum_t; // potencijalna izmena broja bitova
typedef sc_dt::sc_fixed<32, 2, sc_dt::SC_RND, sc_dt::SC_SAT>   result_t;

#endif // COMMON_HPP
