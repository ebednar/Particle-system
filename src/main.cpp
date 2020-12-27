#include "engine.h"
#include <time.h>

int		main(void)
{
	Engine	engine;
	
	
	srand(time(NULL));
	engine.init_engine(640, 480);
	engine.part.alloc_mem();
	engine.part.init_cl();
	engine.run_engine();
	return 0;
}
