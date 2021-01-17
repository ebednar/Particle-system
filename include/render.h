#ifndef RENDER_H
# define RENDER_H

#include "entity.h"
#include <vector>
#include "scene.h"
#include "camera.h"
#include "skybox.h"
#include "particles.h"

struct vec3
{
	float x;
	float y;
	float z;
};

class Render
{
public:
	Entity *player;
public:
	void	init();
	void	set_lights_pos(glm::vec3 **lights, int numb);
	void	draw_scene(Scene* scene, Camera* cam);
	void	draw_skybox(Skybox *skybox, Camera* cam);
	void	draw_particles(Particles *particles);

private:
	glm::mat4	projection;
	glm::mat4	ortProjection;
	vec3		light_pos[3];
};

#endif