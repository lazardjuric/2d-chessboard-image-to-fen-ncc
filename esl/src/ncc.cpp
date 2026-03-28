#include "ncc.hpp"

using namespace sc_core;
using namespace tlm;
using namespace std;

NCC_Target::NCC_Target(sc_module_name name) : sc_module(name), socket("socket"), 
    img_w(0), img_h(0), tmp_w(0), tmp_h(0), template_mean(0), hw_status(0) {
    socket.register_b_transport(this, &NCC_Target::b_transport);
}

void NCC_Target::b_transport(tlm_generic_payload& trans, sc_time& delay) {
    tlm_command cmd = trans.get_command();
    uint64_t    addr = trans.get_address();
    unsigned char* ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    if (cmd == TLM_WRITE_COMMAND) {
        delay += sc_time(20, SC_NS);

        if (addr == REG_IMG_W) img_w = *(int*)ptr;
        else if (addr == REG_IMG_H) img_h = *(int*)ptr;
        else if (addr == REG_TMP_W) tmp_w = *(int*)ptr;
        else if (addr == REG_TMP_H) tmp_h = *(int*)ptr;
        else if (addr == ADDR_IMG_DATA) {
            image.assign(ptr, ptr + len);
        }
        else if (addr == ADDR_TMP_DATA) {
            templ.assign(ptr, ptr + len);
            calculate_template_mean();
        }
        else if (addr == REG_CTRL && *(uint32_t*)ptr == 1) {
            hw_status = 0; 
            int res_points = (img_w - tmp_w + 1) * (img_h - tmp_h + 1);
            
            int hardware_cycles = res_points * (tmp_w * tmp_h);
            delay += sc_time(hardware_cycles * 10, SC_NS); 

            compute_full_matrix();
            hw_status = 1; 
        }
        trans.set_response_status(TLM_OK_RESPONSE);
    } 
    else if (cmd == TLM_READ_COMMAND) {
        if (addr == REG_STATUS) {
            memcpy(ptr, &hw_status, sizeof(uint32_t));
        }
        else if (addr == ADDR_RESULTS) {
            memcpy(ptr, result_map.data(), len);
        }
        trans.set_response_status(TLM_OK_RESPONSE);
    }
}

void NCC_Target::calculate_template_mean() {
    if (templ.empty()) return;
    accum_t sum = 0;
    for (uint8_t val : templ) sum += static_cast<pixel_t>(val);
    template_mean = sum / (int)templ.size();
}

void NCC_Target::compute_full_matrix() {
    int res_w = img_w - tmp_w + 1;
    int res_h = img_h - tmp_h + 1;
    if (res_w <= 0 || res_h <= 0) return;

    result_map.assign(res_w * res_h, 0);
    for (int v = 0; v < res_h; v++) {
        for (int u = 0; u < res_w; u++) {
            result_t val = solve_single_point(u, v);
            result_map[v * res_w + u] = val.range(31, 0).to_int(); 
        }
    }
}

result_t NCC_Target::solve_single_point(int u, int v) {
    accum_t sum_num = 0, sum_den_f = 0, sum_den_t = 0, sum_f = 0;
    int count = tmp_w * tmp_h;
    if (count == 0) return 0;

    for (int y = 0; y < tmp_h; y++) 
        for (int x = 0; x < tmp_w; x++) 
            sum_f += static_cast<pixel_t>(image[(v + y) * img_w + (u + x)]);
    
    mean_t f_bar = sum_f / (int)count; 

    for (int y = 0; y < tmp_h; y++) {
        for (int x = 0; x < tmp_w; x++) {
            diff_t diff_f = static_cast<pixel_t>(image[(v + y) * img_w + (u + x)]) - f_bar;
            diff_t diff_t_val = static_cast<pixel_t>(templ[y * tmp_w + x]) - template_mean;
            
            sum_num += diff_f * diff_t_val;
            sum_den_f += diff_f * diff_f;
            sum_den_t += diff_t_val * diff_t_val;
        }
    }

    if (sum_den_f == 0 || sum_den_t == 0) return 0;
    
    sq_accum_t num_sq   = (sq_accum_t)sum_num * (sq_accum_t)sum_num;
    sq_accum_t den_prod = (sq_accum_t)sum_den_f * (sq_accum_t)sum_den_t;
    if (den_prod == 0) return 0;

    sq_accum_t ncc_wide = num_sq / den_prod; 
    return (result_t)ncc_wide;
}
