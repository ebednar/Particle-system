#include <OpenCL/opencl.h>

class Particles
{
public:
	size_t				number;
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			init_program;
	cl_program			update_program;
	cl_kernel			init_kernel;
	cl_mem				part_mem_obj;
	unsigned int		vbo;
	unsigned int		vao;
	int					shader_id;	

public:
	Particles();
	~Particles();
	void	alloc_mem();
	void	init_cl();
	void	update();
};
