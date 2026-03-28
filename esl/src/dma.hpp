#ifndef DMA_HPP
#define DMA_HPP

#include "common.hpp"

class DMA_Module : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<DMA_Module> s_cpu;
    tlm_utils::simple_initiator_socket<DMA_Module> i_bus;
    
    uint64_t src, dst;
    uint32_t w, h, src_stride, dst_stride;

    DMA_Module(sc_core::sc_module_name name);
    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
    void do_transfer(sc_core::sc_time& delay);
};

#endif // DMA_HPP
