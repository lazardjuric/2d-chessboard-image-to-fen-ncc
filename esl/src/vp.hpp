#ifndef VP_HPP
#define VP_HPP

#include "common.hpp"
#include "sys_bus.hpp"
#include "ncc.hpp"
#include "bram.hpp"
#include "ddr.hpp"
#include "dma.hpp"

class vp : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<vp> s_cpu;
    tlm_utils::simple_initiator_socket<vp> s_bus_int;
    
    sys_bus* bus; 
    NCC_Target* ncc;
    BRAM_Module* bram;
    DDR_Module* ddr;
    DMA_Module* dma;

    vp(sc_core::sc_module_name name);
    ~vp();
    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};

#endif // VP_HPP
