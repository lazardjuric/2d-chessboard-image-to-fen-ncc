#include "vp.hpp"
#include "tb.hpp"

int sc_main(int argc, char* argv[]) {
    vp uut("uut"); 
    tb_vp tb("tb");
    
    tb.isoc.bind(uut.s_cpu);
    
    sc_core::sc_start();
    
    return 0;
}
