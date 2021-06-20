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

typedef struct s_Grav_point
{
    float x;
	float y;
	float z;
}   Grav_point;

__kernel void update_parts(__global Particle *parts, Grav_point point)
{
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 	//int i = get_local_id(0);

    // Do the operation
	float x = point.x - parts[i].x;
	float y = point.y - parts[i].y;
	float z = point.z - parts[i].z;
	float dist = sqrt(x * x + y * y + z * z);

    //parts[i].x += x * parts[i].velocity / sqrt(x * x);
	//parts[i].y += y * parts[i].velocity / sqrt(y * y);
	//parts[i].z -= z * parts[i].velocity / sqrt(z * z);

	parts[i].g = 0.6f * (1 - dist);
}