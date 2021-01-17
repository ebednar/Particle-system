typedef struct s_Particle
{
    float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	float velocity;
}   Particle;

__kernel void init_parts(__global Particle *parts) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 	//int i = get_local_id(0);

    // Do the operation
    parts[i].y -= parts[i].velocity;
}