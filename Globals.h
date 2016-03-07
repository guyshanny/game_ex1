#ifndef __GLOBAL_H
#define __GLOBAL_H

// Global defines
#define GRID_SIZE						30			// The mesh grid's size
#define TRIANGLE_RESOLUTION				2			// The triangle resolution
#define WINDOW_BOUNDARY				    vec3(GRID_SIZE/2-2.0f, -10.0f, GRID_SIZE/2-2.0f)		// The grid size's limit
#define EYE_MATRIX						glm::mat4(1.0f)


// Global macros
#define RAD2DEGREE(x) (x * (180.0/M_PI))


#endif