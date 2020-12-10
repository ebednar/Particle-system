# Particle-system
Particle system using OpenCL and OpenGL

Goal is to achive 3kk particles with 30 fps as minimum

- At first we create OpenGL context and create buffer, with size for all particles, but do not provide any data from RAM (NULL)
- Then create shared OpenCL context
    (CGLContextObj gl_ctx = CGLGetCurrentContext();
	  CGLShareGroupObj gl_sharegroup = CGLGetShareGroup(gl_ctx);
- Now we can get buffer from OpenGL with clCreateFromGLBuffer();
