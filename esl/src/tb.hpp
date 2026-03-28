#ifndef TB_HPP
#define TB_HPP

#include "common.hpp"
#include <string>
#include <vector>

class tb_vp : public sc_core::sc_module {
public:
    tlm_utils::simple_initiator_socket<tb_vp> isoc;

    SC_HAS_PROCESS(tb_vp);
    tb_vp(sc_core::sc_module_name name);

    void test();

private:
    void loadFile(std::string path, std::vector<uint8_t>& data, int& w, int& h);
    void write_reg32(uint64_t addr, uint32_t val, sc_core::sc_time& d);
    void write_reg64(uint64_t addr, uint64_t val, sc_core::sc_time& d);
    void send_data(uint64_t addr, std::vector<uint8_t>& data, sc_core::sc_time& d);
    void read_data(uint64_t addr, std::vector<uint8_t>& data, sc_core::sc_time& d);
    void read_data(uint64_t addr, uint8_t* ptr, int len, sc_core::sc_time& d);
    void configure_dma(uint64_t src, uint64_t dst, uint32_t w, uint32_t h, uint32_t s_stride, uint32_t d_stride, sc_core::sc_time& d);
};

#endif // TB_HPP
