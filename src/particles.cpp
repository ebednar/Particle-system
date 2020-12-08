#include "particles.h"
#include <stdio.h>

void Particles::init_cl()
{
	int i;
	int size = 64;
	int *a = (int *)malloc(sizeof(int) * size);
	int *b = (int *)malloc(sizeof(int) * size);
	for(i = 0; i < size; i++) {
        a[i] = i;
        b[i] = size - i;
    }

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
	ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_GPU, 1, 
            &device_id, &ret_num_devices);

	// Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
 	printf("opencl context\n");
    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	printf("opencl comand queue\n");
 	// Create memory buffers on the device for each vector 
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            size * sizeof(int), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
            size * sizeof(int), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            size * sizeof(int), NULL, &ret);
	printf("opencl mem buffers\n");
	// Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
            size * sizeof(int), a, 0, NULL, NULL);
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
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
	// Execute the OpenCL kernel on the list
    size_t global_item_size = size; // Process the entire lists
    size_t local_item_size = 64; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
	printf("exec kernel\n");
	// Read the memory buffer C on the device to the local variable C
    int *c = (int*)malloc(sizeof(int)*size);
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, 
            size * sizeof(int), c, 0, NULL, NULL);
	printf("read result\n");
	for(i = 0; i < size; i++)
        printf("%d + %d = %d\n", a[i], b[i], c[i]);
	// Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(a);
    free(b);
    free(c);
}