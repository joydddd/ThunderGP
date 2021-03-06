#include "host_graph_api.h"
#include "fpga_application.h"
#include "customize_mem_1.h"


const float epsilion = 0.077;
const float nu = 0.023;

#define DATA_PATH                   "../../sir/"
#define MEM_ID_LOAD_EDGE_PROP       (MEM_ID_USER_DEFINE_BASE)
#define MEM_ID_LOAD_VERTEX_PROP     (MEM_ID_USER_DEFINE_BASE + 1)

int dataPrepareProperty(graphInfo *info)
{

    global.gamma = nu;
    global.beta = epsilion;

    load_from_csv<float>(DATA_PATH"input_S0.csv",    MEM_ID_THETA_S, MEM_ATTR_THETA_S);
    output_init<float>(MEM_ID_NEWTHETA_S, MEM_ATTR_NEWTHETA_S, MEM_ID_THETA_S);
    load_from_csv<float>(DATA_PATH"input_A0.csv",    MEM_ID_THETA_A, MEM_ATTR_THETA_A);
    output_init<float>(MEM_ID_NEWTHETA_A, MEM_ATTR_NEWTHETA_A, MEM_ID_THETA_A);
    load_from_csv<float>(DATA_PATH"input_R0.csv",    MEM_ID_THETA_R, MEM_ATTR_THETA_R);
    output_init<float>(MEM_ID_NEWTHETA_R, MEM_ATTR_NEWTHETA_R, MEM_ID_THETA_R);

    load_from_csv<float>(DATA_PATH"input_alpha.csv", MEM_ID_ALPHA,   MEM_ATTR_ALPHA);
    load_from_csv<float>(DATA_PATH"input_pi.csv",    MEM_ID_PI,      MEM_ATTR_PI);

    float * pop = load_from_csv<float>(DATA_PATH"input_pop.csv", MEM_ID_N,       MEM_ATTR_N);
    float * i0  = load_from_csv<float>(DATA_PATH"input_I0.csv", MEM_ID_LOAD_VERTEX_PROP, ATTR_HOST_ONLY);
   
    if ((pop == NULL) || (i0 == NULL))
    {
        return -1;
    }

    prop_t *vertexPushinProp = (prop_t*)get_host_mem_pointer(MEM_ID_PUSHIN_PROP);
    int vertexNum = info->vertexNum;
    for (int i = 0; i < info->vertexNum; i++)
    {
        vertexPushinProp[i] = (int)(i0[i] * pop[i] * FIXED_SCALE);
    }
    int alignedVertexNum = get_he_mem(MEM_ID_PUSHIN_PROP)->size / sizeof(int);
    for (int i = vertexNum; i < alignedVertexNum; i++) {
        vertexPushinProp[i]  = 0;
    }

    int * loadedEdgeProp = load_from_csv<int>(DATA_PATH"edge_prop.txt",     MEM_ID_LOAD_EDGE_PROP, ATTR_HOST_ONLY);
    prop_t *edgeProp    = (prop_t*)get_host_mem_pointer(MEM_ID_EDGE_PROP);
    int edgeNum = info->edgeNum;
    for (int i = 0; i < edgeNum; i++)
    {
        edgeProp[i] = loadedEdgeProp[i];
    }
    int alignedEdgeNum = get_he_mem(MEM_ID_EDGE_PROP)->size / sizeof(int);
    for (int i = edgeNum; i < alignedEdgeNum; i++)
    {
        edgeProp[i] = 0;
    }


    return 0;
}

unsigned int dataPrepareGetArg(graphInfo *info)
{
    return 0;
}