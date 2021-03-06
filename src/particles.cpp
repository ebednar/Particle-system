#include "particles.h"
#include <iostream>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>
#include "shader.h"

Particles::Particles()
{
	number = 10;
}

Particles::~Particles()
{
	// Clean up
	cl_int ret;
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(init_kernel);
    ret = clReleaseProgram(init_program);
    // ret = clReleaseMemObject(a_mem_obj);
	ret = clReleaseMemObject(part_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
}

void Particles::alloc_mem()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (3 + 3 + 2) * number * sizeof(float), NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	create_shader(&shader_id, "res/shaders/part_vert.glsl", "res/shaders/part_frag.glsl");
}

void	Particles::read_kernel(char* path, cl_program* program)
{
	FILE	*fp;
    char	*source_str;
    size_t	source_size;
 
    fp = fopen(path, "r");
    if (!fp) {
        std::cout << "Failed to load kernel" << std::endl;
        exit(1);
    }

	source_str = (char*)malloc(10000);
    source_size = fread( source_str, 1, 10000, fp);
    fclose( fp );
	std::cout << "kernel loaded" << std::endl;
	cl_int ret;
	*program = clCreateProgramWithSource(context, 1, 
        (const char **)&source_str, (const size_t *)&source_size, &ret);
}

void	Particles::init_cl()
{
	

	cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	if (!ret_num_platforms)
		std::cout << "platform error" << std::endl;
	ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_GPU, 1, 
            &device_id, &ret_num_devices);
	if (!ret_num_devices)
		std::cout << "device error" << std::endl;
	// Create an OpenCL context
	CGLContextObj CGLGetCurrentContext(void);
	CGLShareGroupObj  CGLGetShareGroup(CGLContextObj);

	CGLContextObj     kCGLContext     = CGLGetCurrentContext();
	CGLShareGroupObj  kCGLShareGroup  = CGLGetShareGroup(kCGLContext);

	cl_context_properties cont_props[] = 
	{
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		(cl_context_properties) kCGLShareGroup,
		0
	};
    context = clCreateContext(cont_props, 0, 0, NULL, NULL, &ret);
	if (ret != CL_SUCCESS)
		std::cout << "context error" << std::endl;
    // Create a command queue
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	// create memory object from GL Buffer
	part_mem_obj = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, vbo, &ret);
	if (ret != CL_SUCCESS)
		std::cout << "buffer error" << std::endl;

	// Create a program from the kernel source
    read_kernel("res/kernel/init.cl", &init_program);
	read_kernel("res/kernel/update.cl", &update_program);
	// Build the program
    ret = clBuildProgram(init_program, 1, &device_id, NULL, NULL, NULL);
	ret = clBuildProgram(update_program, 1, &device_id, NULL, NULL, NULL);
    // Create the OpenCL kernel
    init_kernel = clCreateKernel(init_program, "init_parts", &ret);
	update_kernel = clCreateKernel(update_program, "update_parts", &ret);
	// Set the arguments of the kernel
	int r = rand() % 100;
	cl_float seed = r / 100.0f;
	printf("seed %f\n", seed);
	cl_float fnumber = (cl_float)number;
    ret = clSetKernelArg(init_kernel, 0, sizeof(cl_mem), (void *)&part_mem_obj);
	ret = clSetKernelArg(init_kernel, 1, sizeof(cl_float), &fnumber);
	ret = clSetKernelArg(init_kernel, 2, sizeof(cl_float), &seed);
	ret = clSetKernelArg(update_kernel, 0, sizeof(cl_mem), (void *)&part_mem_obj);	
	ret = clSetKernelArg(update_kernel, 1, sizeof(grav_point), &g_point);

	// Execute the OpenCL kernel on the list
    global_item_size = number; // Process the entire lists
    local_item_size = number; // Divide work items into groups of
    ret = clEnqueueNDRangeKernel(command_queue, init_kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
			
	// ret = clEnqueueNDRangeKernel(command_queue, update_kernel, 1, NULL, 
    //         &global_item_size, &local_item_size, 0, NULL, NULL);
	// Read the memory buffer on the device to the local variable
	float *result = (float *)malloc(sizeof(float) * number * 8);
	ret = clEnqueueReadBuffer(command_queue, part_mem_obj, CL_TRUE, 0, 
		number * 8 * sizeof(float), result, 0, NULL, NULL);
	std::cout << "check result" << std::endl;
	for (int i = 0; i < number * 8; i += 8)
	{
		std::cout << result[i] << " ";
		std::cout << result[i + 1] << " ";
		std::cout << result[i + 2] << std::endl;
	}
	std::cout << "\n";

    free(result);
}

void Particles::update()
{
	cl_int ret = clEnqueueNDRangeKernel(command_queue, update_kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
}