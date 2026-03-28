#include "vp.hpp"

using namespace sc_core;
using namespace tlm;

vp::vp(sc_module_name name) : sc_module(name), s_cpu("s_cpu"), s_bus_int("s_bus_int") {
    bus = new sys_bus("bus"); 
    ncc = new NCC_Target("ncc");
    bram = new BRAM_Module("bram");
    ddr = new DDR_Module("ddr");
    dma = new DMA_Module("dma");

    s_cpu.register_b_transport(this, &vp::b_transport);
    
    s_bus_int.bind(bus->s_cpu); 
    dma->i_bus.bind(bus->s_dma); 
    bus->i_ncc.bind(ncc->socket);
    bus->i_bram.bind(bram->socket);
    bus->i_ddr.bind(ddr->socket);
    bus->i_dma.bind(dma->s_cpu);
}

vp::~vp() {
    delete bus;
    delete ncc;
    delete bram;
    delete ddr;
    delete dma;
}

void vp::b_transport(tlm_generic_payload& trans, sc_time& delay) { 
    s_bus_int->b_transport(trans, delay); 
}
