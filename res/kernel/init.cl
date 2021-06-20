typedef struct s_Particle
{
    float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	float velocity;
	float acceleraion;
}   Particle;

__kernel void init_parts(__global Particle *parts, float number, float seed) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 	int j = get_local_id(0);

    // Do the operation
	parts[i].x = i / number - 0.5f;
	//parts[i].y = seed * (j / number);
	parts[i].y = seed * (j / number);
    parts[i].z = 0.0f;
	parts[i].r = 1.0f;
    parts[i].g = 1.0f * (1 - (parts[i].x * parts[i].x + parts[i].y * parts[i].y));
    parts[i].b = 0.0f;
	parts[i].velocity = 0.002f;
}