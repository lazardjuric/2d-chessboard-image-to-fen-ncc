#ifndef NCC_HPP
#define NCC_HPP

#include "common.hpp"
#include <vector>

class NCC_Target : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<NCC_Target> socket;
    std::vector<uint8_t> image, templ; 
    std::vector<int32_t> result_map; 
    int img_w, img_h, tmp_w, tmp_h;
    mean_t template_mean;
    uint32_t hw_status;

    NCC_Target(sc_core::sc_module_name name);
    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);

private:
    void calculate_template_mean();
    void compute_full_matrix();
    result_t solve_single_point(int u, int v);
};

#endif // NCC_HPP
