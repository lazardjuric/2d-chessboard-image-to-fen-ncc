#include "bram.hpp"
#include <cstring>

using namespace sc_core;
using namespace tlm;

BRAM_Module::BRAM_Module(sc_module_name name) : sc_module(name), socket("socket"), mem(1024 * 1024, 0) {
    socket.register_b_transport(this, &BRAM_Module::b_transport);
}

void BRAM_Module::b_transport(tlm_generic_payload& trans, sc_time& delay) {
    tlm_command cmd = trans.get_command();
    uint64_t    addr = trans.get_address();
    unsigned char* ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    // bounds
    if (addr + len > mem.size()) {
        trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    // command 
    switch (cmd) {
        case TLM_READ_COMMAND:
            memcpy(ptr, &mem[addr], len);
            break;
        case TLM_WRITE_COMMAND:
            memcpy(&mem[addr], ptr, len);
            break;
        case TLM_IGNORE_COMMAND:
            break; 
        default:
            trans.set_response_status(TLM_COMMAND_ERROR_RESPONSE);
            return;
    }

    // timing: 20ns base + 2.5ns per byte
    delay += sc_time(20, SC_NS) + sc_time(len * 2.5, SC_NS);
    trans.set_response_status(TLM_OK_RESPONSE);
}
