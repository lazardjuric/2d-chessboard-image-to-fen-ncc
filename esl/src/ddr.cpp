#include "ddr.hpp"
#include <cstring>

using namespace sc_core;
using namespace tlm;

DDR_Module::DDR_Module(sc_module_name name) : sc_module(name), socket("socket"), mem(8 * 1024 * 1024, 0) {
    socket.register_b_transport(this, &DDR_Module::b_transport);
}

void DDR_Module::b_transport(tlm_generic_payload& trans, sc_time& delay) {
    tlm_command cmd = trans.get_command();
    uint64_t    addr = trans.get_address();
    unsigned char* ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    // address bounds
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
            // Just return OK without doing anything
            break;
        default:
            trans.set_response_status(TLM_COMMAND_ERROR_RESPONSE);
            return;
    }

    // timing
    delay += sc_time(150, SC_NS) + sc_time(len * 3.75, SC_NS);
    trans.set_response_status(TLM_OK_RESPONSE);
}
