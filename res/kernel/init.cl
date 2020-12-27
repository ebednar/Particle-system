typedef struct s_Particle
{
    float3 pos;
}   Particle;

__kernel void vector_add(__global Particle *C) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 
    // Do the operation
    C[i].pos.x = -0.5f;
    //C[i].pos.y = -0.5f;
   // C[i].pos.z = -0.5f;
}