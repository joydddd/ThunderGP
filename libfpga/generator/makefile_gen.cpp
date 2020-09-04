#include <cmath>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "common.h"
#include "para_check.h"

/* TODO: remove hard-code gen*/

int main(int argc, char **argv) {
    std::ofstream file;
    file.open("./tmp_fpga_top/gs_kernel.mk");
    for (int i = 0; i < SUB_PARTITION_NUM; i ++)
    {
        file << "############# gs " << (i + 1) << " #############" << std::endl;
        file << "$(XCLBIN)/readEdgesCU" << (i + 1)
             << ".$(TARGET).$(DSA).xo: $(GS_KERNEL_PATH)/scatter_gather_top_"
             << (i + 1) << ".cpp" << std::endl;
        file << "\tmkdir -p $(XCLBIN)" << std::endl;
        file << "\t$(XOCC) $(CLFLAGS) -c -k readEdgesCU" << (i + 1) << " -I'$(<D)' -o'$@' '$<'" << std::endl;
        file << "BINARY_CONTAINER_OBJS += $(XCLBIN)/readEdgesCU" << (i + 1) << ".$(TARGET).$(DSA).xo" << std::endl;
        file << "BINARY_LINK_OBJS    += --nk  readEdgesCU" << (i + 1) << ":1" << std::endl;
        file << "BINARY_LINK_OBJS    += --sp  readEdgesCU" << (i + 1) << "_1.edgesHeadArray:DDR[$(GS" << (i + 1) << "_DDR)]" << std::endl;
        file << "BINARY_LINK_OBJS    += --sp  readEdgesCU" << (i + 1) << "_1.edgesTailArray:DDR[$(GS" << (i + 1) << "_DDR)]" << std::endl;
        file << "BINARY_LINK_OBJS    += --sp  readEdgesCU" << (i + 1) << "_1.vertexPushinProp:DDR[$(GS" << (i + 1) << "_DDR)]" << std::endl;
        file << "BINARY_LINK_OBJS    += --sp  readEdgesCU" << (i + 1) << "_1.tmpVertexProp:DDR[$(GS" << (i + 1) << "_DDR)]" << std::endl;
        file << "ifeq ($(strip $(HAVE_EDGE_PROP)), $(strip $(VAR_TRUE)))" << std::endl;
        file << "BINARY_LINK_OBJS    += --sp  readEdgesCU" << (i + 1) << "_1.edgeProp:DDR[$(GS" << (i + 1) << "_DDR)]" << std::endl;
        file << "endif" << std::endl;
        file << "BINARY_LINK_OBJS    += --slr readEdgesCU" << (i + 1) << "_1:SLR$(GS" << (i + 1) << "_SLR)" << std::endl;
    }
    file.flush();
    file.close();

    return 0;
}
