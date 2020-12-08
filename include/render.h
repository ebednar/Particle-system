#ifndef RENDER_H
# define RENDER_H

#include "entity.h"
#include <vector>
#include "camera.h"

class Render
{
public:
	Entity *player;
public:
	void	draw_scene(std::vector<Entity *> scene, glm::vec3 **lights, Camera *cam, bool free_cam);
private:
};

#endif