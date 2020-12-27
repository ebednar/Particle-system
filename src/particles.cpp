#include "particles.h"
#include <stdio.h>
#include <OpenCL/opencl.h>
#include <OpenCL/cl_gl.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

void Particles::alloc_mem()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(float), NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Particles::init_cl()
{
	FILE *fp;
    char *source_str;
    size_t source_size;
 
    fp = fopen("res/kernel/test.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }

	source_str = (char*)malloc(10000);
    source_size = fread( source_str, 1, 10000, fp);
    fclose( fp );
	printf("kernel loaded\n");

	cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	if (!ret_num_platforms)
		printf("platform error\n");
	ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_GPU, 1, 
            &device_id, &ret_num_devices);
	if (!ret_num_devices)
		printf("device error\n");
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
    cl_context context = clCreateContext(cont_props, 0, 0, NULL, NULL, &ret);
	if (ret != CL_SUCCESS)
		printf("context error\n");
 	printf("opencl context\n");
    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	printf("opencl comand queue\n");
 	// Create memory buffers on the device for each vector 
	//  cl_mem part_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
    //         64 * sizeof(int), NULL, &ret);
	cl_mem part_mem_obj = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, vbo, &ret);
	if (ret != CL_SUCCESS)
		printf("buffer error\n");
	printf("opencl mem buffers\n");
	// Copy the lists A and B to their respective memory buffers
    // ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
    //         size * sizeof(int), a, 0, NULL, NULL);
    //ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
      //      size * sizeof(int), b, 0, NULL, NULL);
	printf("copy to mem buffers\n");
	// Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, 
        (const char **)&source_str, (const size_t *)&source_size, &ret);
	printf("create program\n");
	// Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
	printf("create kernel\n");
	// Set the arguments of the kernel
    // ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    // ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&part_mem_obj);
	// Execute the OpenCL kernel on the list
    size_t global_item_size = 64; // Process the entire lists
    size_t local_item_size = 64; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
	printf("exec kernel\n");
	// Read the memory buffer C on the device to the local variable C
   int *c = (int*)malloc(sizeof(int)*64);
    ret = clEnqueueReadBuffer(command_queue, part_mem_obj, CL_TRUE, 0, 
            64 * sizeof(int), c, 0, NULL, NULL);
	for (int i = 0; i < 64; ++i)
		printf("%d ", c[i]);
	printf("\nread result\n");
	
	// Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    // ret = clReleaseMemObject(a_mem_obj);
    // ret = clReleaseMemObject(b_mem_obj);
    // ret = clReleaseMemObject(c_mem_obj);
	ret = clReleaseMemObject(part_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    // free(a);
    // free(b);
    // free(c);
}