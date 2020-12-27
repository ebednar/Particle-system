#include <OpenCL/opencl.h>

struct Particle
{
	float	pos[3];
};

class Particles
{
public:
	unsigned int	vbo;
	unsigned int	vao;
	int				shader_id;
	cl_mem			part_mem_obj;
public:
	void alloc_mem();
	void init_cl();	
};