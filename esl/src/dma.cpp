#include "dma.hpp"
#include <vector>

using namespace sc_core;
using namespace tlm;
using namespace std;

DMA_Module::DMA_Module(sc_module_name name) : sc_module(name), s_cpu("s_cpu"), i_bus("i_bus"), 
                                              src(0), dst(0), w(0), h(0), src_stride(0), dst_stride(0) {
    s_cpu.register_b_transport(this, &DMA_Module::b_transport);
}

void DMA_Module::b_transport(tlm_generic_payload& trans, sc_time& delay) {
    if (trans.get_command() == TLM_WRITE_COMMAND) {
        uint64_t addr = trans.get_address();
        uint64_t val = (trans.get_data_length() == 8) ? *(uint64_t*)trans.get_data_ptr() : *(uint32_t*)trans.get_data_ptr();
        
        if (addr == 0x00) src = val;
        else if (addr == 0x08) dst = val;
        else if (addr == 0x10) w = val;
        else if (addr == 0x14) h = val;
        else if (addr == 0x18) src_stride = val;
        else if (addr == 0x1C) dst_stride = val;
        else if (addr == 0x20 && val == 1) { 
            delay += sc_time(50, SC_NS); 
            do_transfer(delay);
        }
    }
    trans.set_response_status(TLM_OK_RESPONSE);
}

void DMA_Module::do_transfer(sc_time& delay) {
    if (src_stride == w && dst_stride == w) {
        uint32_t total_len = w * h;
        vector<uint8_t> buffer(total_len);
        
        tlm_generic_payload pl_rd;
        pl_rd.set_command(TLM_READ_COMMAND); 
        pl_rd.set_address(src); 
        pl_rd.set_data_ptr(buffer.data()); 
        pl_rd.set_data_length(total_len);
        i_bus->b_transport(pl_rd, delay);
        
        tlm_generic_payload pl_wr;
        pl_wr.set_command(TLM_WRITE_COMMAND); 
        pl_wr.set_address(dst); 
        pl_wr.set_data_ptr(buffer.data()); 
        pl_wr.set_data_length(total_len);
        i_bus->b_transport(pl_wr, delay);
      
    } else {
        vector<uint8_t> buffer(w);
        for (uint32_t y = 0; y < h; y++) {
            tlm_generic_payload pl_rd;
            pl_rd.set_command(TLM_READ_COMMAND); 
            pl_rd.set_address(src + y * src_stride); 
            pl_rd.set_data_ptr(buffer.data()); 
            pl_rd.set_data_length(w);
            i_bus->b_transport(pl_rd, delay);
            
            tlm_generic_payload pl_wr;
            pl_wr.set_command(TLM_WRITE_COMMAND); 
            pl_wr.set_address(dst + y * dst_stride); 
            pl_wr.set_data_ptr(buffer.data()); 
            pl_wr.set_data_length(w);
            i_bus->b_transport(pl_wr, delay);
        }
    }
}
