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
    parts[i].x = -0.9f + i / 5.0f;
    parts[i].y = -0.9f + i / 5.0f;
    parts[i].z = 0.0f;
	parts[i].r = 1.0f;
    parts[i].g = 0.6f * (1 - (parts[i].x * parts[i].x + parts[i].y * parts[i].y));
    parts[i].b = 0.0f;
	parts[i].velocity = 0.001f;
}