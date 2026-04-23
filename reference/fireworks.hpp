#pragma once

#include "scene.hpp"
#include "gef.hpp"
#include "vla.hpp"

enum fireworks_particle_state {FPS_DEAD, FPS_ASCENDING, FPS_ASCENDING_TRAIL, FPS_EXPLODED, FPS_EXPLODED_TRAIL, FPS_SPARK};

// so i guess its got pos and velocity
// how are we gonna do acceleration? by changing velocity? or also have it
// maybe a fn ptr for v(t)? or x(t)? hmm

// draw colour: gradient(t) based on type
// also age based on type

// trails: go white and then go black, also alpha? also sparks usually cool
// have a bloomy sort of kernel?

// we could do this all in floats

// ascent: emit sparks and colour-white-black smokey trail thing
// explosion: sparks and primary around in a circle, rapid deceleration
// make some cooked ones that emit along the way
// make some with twin explosion

// explodeds affected by gravity too so yeah an acceleration term does make sense
// sine + random might look good for ascending trail emitter

struct fireworks_particle {
    fireworks_particle_state state;
    float x;
    float y;
    float vx;
    float vy;
    colour primary;
    float age;
    float t_child1;
    float t_child2;
    float lifetime;
    
    unsigned int seed;
    unsigned int nchildren;

    float normalized_t() {return age / lifetime;}
    fireworks_particle spawn_child(fireworks_particle_state child_state);
};

struct fireworks {
    vla<fireworks_particle> particles = vla<fireworks_particle>();
    int first_vacancy = 0;
    // vla insert_overwrite(FPS_DEAD) eh just make it specific

    void insert_particle(fireworks_particle p);
    void kill(int i);
    void update(double dt);
    void draw(shared_data *app_d);
    
    void spawn_primary();
    void reset();
};