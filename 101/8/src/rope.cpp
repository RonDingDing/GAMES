#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL
{

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes, double damping_factor)
    {

        if (damping_factor < 0)
        {
            damping_factor = 0;
        }
        else if (damping_factor > 1)
        {
            damping_factor = 1;
        }
        else
        {
            this->damping_factor = damping_factor;
        }

        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.
        if (num_nodes < 2)
        {
            num_nodes = 2;
        }

        Vector2D direction = (end - start) / (num_nodes - 1);

        for (int j = 0; j < num_nodes; j++)
        {
            Vector2D node_j_pos = start + j * direction;
            Mass *node = new Mass(node_j_pos, node_mass, false);
            masses.push_back(node);
        }

        for (auto mass_point = masses.begin(); mass_point != masses.end() - 1; mass_point++)
        {
            Spring *spring = new Spring(*mass_point, *(mass_point + 1), k);
            springs.push_back(spring);
        }

        // Comment-in this part when you implement the constructor
        for (auto &i : pinned_nodes)
        {
            masses[i]->pinned = true;
        }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Vector2D b_minus_a = s->m2->position - s->m1->position;
            double b_minus_a_norm = b_minus_a.norm();
            double l = s->rest_length;
            float ks = s->k;
            float kd = damping_factor;

            Vector2D a_to_b_force = ks * b_minus_a / b_minus_a_norm * (b_minus_a_norm - l);
            Vector2D b_to_a_force = -a_to_b_force;

            s->m1->forces += a_to_b_force;
            s->m2->forces += b_to_a_force;

            // TODO (Part 2): Add global damping
            Vector2D vb = s->m2->velocity;
            Vector2D va = s->m1->velocity;
            Vector2D a_fliction = -kd * b_minus_a.unit() * dot(b_minus_a.unit(), vb - va);
            Vector2D b_fliction = -a_fliction;

            s->m1->forces += a_fliction;
            s->m2->forces += b_fliction;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                m->forces += gravity * m->mass;

                // a = F / m;
                double dt = delta_t;
                Vector2D at = m->forces / m->mass;
                Vector2D xt = m->last_position;
                Vector2D vt = m->velocity;

                Vector2D vt_plus_1 = vt + at * dt;
                // Vector2D xt_plus_1 = xt + vt * dt;
                Vector2D xt_plus_1 = xt + vt_plus_1 * dt;

                m->velocity = vt_plus_1;
                m->position = m->last_position = xt_plus_1;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet (solving constraints)

            Vector2D b_minus_a = s->m2->position - s->m1->position;
            double b_minus_a_norm = b_minus_a.norm();
            double l = s->rest_length;
            float ks = s->k;

            Vector2D a_to_b_force = ks * b_minus_a / b_minus_a_norm * (b_minus_a_norm - l);
            Vector2D b_to_a_force = -a_to_b_force;

            s->m1->forces += a_to_b_force;
            s->m2->forces += b_to_a_force;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // Vector2D temp_position = m->position;
                // TODO (Part 3.1): Set the new position of the rope mass
                m->forces += gravity * m->mass;

                Vector2D at = m->forces / m->mass;
                Vector2D xt = m->position;
                Vector2D xt_minus_1 = m->last_position;
                Vector2D xt_plus_1 = xt + (1 - damping_factor) * (xt - xt_minus_1) + at * delta_t * delta_t;

                m->last_position = xt;
                m->position = xt_plus_1;

                // TODO (Part 4): Add global Verlet damping
            }

            m->forces = Vector2D(0, 0);
        }
    }
}
