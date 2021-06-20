#define GL_SILENCE_DEPRECATION
#include <OpenCL/opencl.h>

struct grav_point
{
    cl_float x;
	cl_float y;
	cl_float z;
	grav_point():x(0.0f), y(0.0f), z(0.0f){};
};

class Particles
{
public:
	grav_point			g_point;
	size_t				number;
	size_t				global_item_size;
	size_t				local_item_size;
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			init_program;
	cl_program			update_program;
	cl_kernel			init_kernel;
	cl_kernel			update_kernel;
	cl_mem				part_mem_obj;
	unsigned int		vbo;
	unsigned int		vao;
	int					shader_id;

public:
	Particles();
	~Particles();
	void	alloc_mem();
	void	read_kernel(char* path, cl_program* program);
	void	init_cl();
	void	update();
};
