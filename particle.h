#ifndef PARTICLE_H__
#define PARTICLE_H__

#include "vector3.h"

class Particle
{
    public:
        Vector3f position;
        Vector3f oldPos;
        Vector3f velocity;
        int energy;
        float width;
        float height;

        Particle(const Vector3f& n_position, const Vector3f& n_velocity, int n_energy, float n_width, float n_height) :
            position(n_position), oldPos(n_position), velocity(n_velocity), energy(n_energy), width(n_width), height(n_height) {}
};

#endif // PARTICLE_H__
