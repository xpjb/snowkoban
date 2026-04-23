#include "scene.hpp"
#include "fireworks.hpp"
#include "util.hpp" 
#include "coolmath.hpp"

#define min(A,B) (A < B ? A : B)

fireworks_particle fireworks_particle::spawn_child(fireworks_particle_state child_state) {
    auto c = *this;
    nchildren++;
    c.age = 0;
    c.t_child1 = 0;
    c.t_child2 = 0;
    c.seed = hash_intn(seed + nchildren * 34234, 0, 999999);
    c.state = child_state;

    if (child_state == FPS_ASCENDING) {
        c.lifetime = 1.4 * hash_floatn(c.seed, 0.6, 1.4);
    } else if (child_state == FPS_ASCENDING_TRAIL) {
        c.lifetime = 1.2;
    } else if (child_state == FPS_SPARK) {
        c.lifetime = 0.8;
    } else if (child_state == FPS_EXPLODED) {
        c.lifetime = 1 * hash_floatn(c.seed, 1, 1.5);
    }

    return c;
}

void fireworks::update(double dt) {
    for (int i = 0; i < particles.length; i++) {
        const auto p = &particles.items[i];
        if (p->state == FPS_DEAD) continue;

        p->age += dt;
        p->t_child1 += dt;
        p->t_child2 += dt;
        p->x += p->vx * dt;
        p->y += p->vy * dt;

        switch(p->state) {
        case FPS_DEAD:
        {}
        case FPS_ASCENDING:
        {
            if (p->age > p->lifetime) {
                kill(i);
                const auto n_children = hash_intn(p->seed, 6, 12);
                for (int j = 0; j < n_children; j++) {
                    const auto angle = (2*M_PI / n_children) * j;
                    auto new_child = p->spawn_child(FPS_EXPLODED);
                    const auto new_velocity_magnitude = hash_floatn(new_child.seed, 0.08, 0.2);
                    new_child.vx = new_velocity_magnitude * cos(angle);
                    new_child.vy = new_velocity_magnitude * sin(angle);
                    insert_particle(new_child);
                }
            }
            const auto spawn_trail = 0.03;
            const auto spawn_spark = 0.08;
            if (p->t_child1 > spawn_trail) {
                p->t_child1 -= spawn_trail;
                auto new_child = p->spawn_child(FPS_ASCENDING_TRAIL);
                new_child.vx *= 0.1;
                new_child.vy *= 0.1;
                const auto random_magnitude = 0.01;
                new_child.vx += hash_floatn(new_child.seed + 234234, -random_magnitude, random_magnitude); 
                new_child.vy += hash_floatn(new_child.seed + 897575, -random_magnitude, random_magnitude);
                insert_particle(new_child);
            }
            if (p->t_child2 > spawn_spark) {
                p->t_child2 -= spawn_spark;
                auto new_child = p->spawn_child(FPS_SPARK);
                new_child.vx *= 0.05;
                new_child.vy *= 0.05;
                const auto random_magnitude = 0.05;
                new_child.vx += hash_floatn(new_child.seed + 234234, -random_magnitude, random_magnitude); 
                new_child.vy += hash_floatn(new_child.seed + 897575, -random_magnitude, random_magnitude); 
                insert_particle(new_child);
            }
            break;
        }
        case FPS_ASCENDING_TRAIL:
        case FPS_SPARK:
        {            
            if (p->age > p->lifetime) {
                kill(i);
            }
            break;
        }
        case FPS_EXPLODED:
        {
            if (p->age > p->lifetime) {
                kill(i);
            }

            p->vy += 0.03 * dt;
            
            const auto spawn_trail = 0.04;
            if (p->t_child1 > spawn_trail) {
                p->t_child1 -= spawn_trail;
                auto new_child = p->spawn_child(FPS_ASCENDING_TRAIL);
                new_child.vx *= 0.1;
                new_child.vy *= 0.1;
                const auto random_magnitude = 0.05;
                new_child.vx += hash_floatn(new_child.seed + 234234, -random_magnitude, random_magnitude); 
                new_child.vy += hash_floatn(new_child.seed + 897575, -random_magnitude, random_magnitude);
                insert_particle(new_child);
            }
            // 
            break;
        }
        case FPS_EXPLODED_TRAIL:
        {
        // fuk it lol just using asc trail for now
        break;
        }
        }
    }
}

// t 0..1
colour gradient3(float t, colour a, colour b, colour c, float t_b) {
    if (t < t_b) {
        return gef_rgba(
            cm_remap(0, t_b, a.r, b.r, t),
            cm_remap(0, t_b, a.g, b.g, t),
            cm_remap(0, t_b, a.b, b.b, t),
            cm_remap(0, t_b, a.a, b.a, t)
        );
    } else {
        return gef_rgba(
            cm_remap(t_b, 1, b.r, c.r, t),
            cm_remap(t_b, 1, b.g, c.g, t),
            cm_remap(t_b, 1, b.b, c.b, t),
            cm_remap(t_b, 1, b.a, c.a, t)
        );
    }
}

// ah it gets stretched so aspect ratio lol
// imma just get it drawing anything first
void fireworks::draw(shared_data *app_d) {
    const auto px_size = 6;
    for (int i = 0; i < particles.length; i++) {
        const auto p = &particles.items[i];
        if (p->state == FPS_DEAD) continue;

        const auto x = p->x * app_d->gc.xres;
        const auto y = p->y * app_d->gc.yres;

        const auto r = rect::centered(x, y, px_size, px_size);
        const auto colour = 
            p->state == FPS_ASCENDING ?
                p->primary :
            p->state == FPS_ASCENDING_TRAIL ?
                gradient3(p->normalized_t(), 
                    p->primary,
                    gef_rgb(255,255,255),
                    gef_rgba(0,0,0,0),
                    0.5
                ) :
            p->state == FPS_SPARK ?
                gradient3(p->normalized_t(),
                    gef_rgb(255, 255, 0),
                    gef_rgb(255,128, 0),
                    gef_rgb(255, 0, 0),
                    0.5
                ) :
            p->state == FPS_EXPLODED ?
                p->primary :
                gef_rgb(255, 0, 255);

        gef_draw_rect(&app_d->gc, colour, r);
    }
}

void fireworks::kill(int i) {
    first_vacancy = min(first_vacancy, i);
    particles.items[i].state = FPS_DEAD;
}

void fireworks::insert_particle(fireworks_particle p) {
    for (; first_vacancy < particles.length; first_vacancy++) {
        if (particles.items[first_vacancy].state == FPS_DEAD) {
            particles.items[first_vacancy] = p;
            return;
        }
    }
    particles.push(p); // maybe inc first vacancy idk
}

void fireworks::spawn_primary() {
    static int firework_seed = 0;
    firework_seed = hash_intn(firework_seed, 0, 999999);
    // todo hsv and random high sat or whatever
    const colour colours[] = {
        gef_rgb(255,0,0),
        gef_rgb(0,255,0),
        gef_rgb(0,0,255),
        gef_rgb(0,255,255),
        gef_rgb(255,255,255),
        gef_rgb(255,0,255),
        gef_rgb(255,255,0),
    };

    auto f = fireworks_particle();
    f.y = 1;
    f.x = 0.5;
    f.seed = firework_seed;
    f.primary = colours[hash_intn(f.seed + 324234, 0, 7)];
    const auto angle = hash_floatn(f.seed + 123123321, M_PI/4, 3*M_PI/4);

    const auto vmag = 0.4;
    f.vx = vmag * cos(angle);
    f.vy = -vmag * sin(angle);
    f.state = FPS_ASCENDING;
    f.lifetime = 1.4 * hash_floatn(f.seed, 0.6, 1.4);
    insert_particle(f);
}

void fireworks::reset() {
    particles.destroy();
    particles = vla<fireworks_particle>();
}