#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <sodium.h>

// Define the structure for variable bounds
typedef struct {
    double lowerBound;
    double upperBound;
} Bound;

// Objective function prototype
typedef double (*ObjectiveFunction)(int, double[]);

// Helper function to generate secure random numbers in a range using Sodium
double random_double(double min, double max) {
    uint32_t rand_val = randombytes_uniform(UINT32_MAX);
    double normalized_rand = (double)rand_val / (double)UINT32_MAX;
    return min + (max - min) * normalized_rand;
}

// Particle Swarm Optimization (PSO) with Neighborhood Method
double pso(ObjectiveFunction objective_function, int NUM_VARIABLES, Bound *bounds, int NUM_PARTICLES, int MAX_ITERATIONS, double *best_position) {
    // Allocate memory for particles
    double **p_xi = (double**)malloc(NUM_PARTICLES * sizeof(double*));  // Particle positions
    double **vel_i = (double**)malloc(NUM_PARTICLES * sizeof(double*)); // Particle velocities
    double **best_xi = (double**)malloc(NUM_PARTICLES * sizeof(double*)); // Personal best positions
    double *fitPbest = (double*)malloc(NUM_PARTICLES * sizeof(double)); // Fitness of personal best
    double **neighBest_val_i = (double**)malloc(NUM_PARTICLES * sizeof(double*)); // Neighborhood best positions
    double *fitnessNeigh = (double*)malloc(NUM_PARTICLES * sizeof(double)); // Fitness of neighborhood best
    double fitnessGlob_b = DBL_MAX; // Global best fitness, initialized to infinity

    // Check for failed memory allocations
    if (p_xi == NULL || vel_i == NULL || best_xi == NULL || fitPbest == NULL || neighBest_val_i == NULL || fitnessNeigh == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);  // Exit with error
    }

    for (int i = 0; i < NUM_PARTICLES; i++) {
        p_xi[i] = (double*)malloc(NUM_VARIABLES * sizeof(double));
        vel_i[i] = (double*)malloc(NUM_VARIABLES * sizeof(double));
        best_xi[i] = (double*)malloc(NUM_VARIABLES * sizeof(double));
        neighBest_val_i[i] = (double*)malloc(NUM_VARIABLES * sizeof(double));
        if (p_xi[i] == NULL || vel_i[i] == NULL || best_xi[i] == NULL || neighBest_val_i[i] == NULL) {
            fprintf(stderr, "Memory allocation failed at particle %d!\n", i);
            exit(EXIT_FAILURE);  // Exit with error
        }
    }

    // Initialize constants
    double w = 0.7, c1 = 1.5, c2 = 1.5;

    // Initialization
    for (int i = 0; i < NUM_PARTICLES; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            p_xi[i][j] = random_double(bounds[j].lowerBound, bounds[j].upperBound);
            double bound_range = fabs(bounds[j].upperBound - bounds[j].lowerBound) / 5;
            vel_i[i][j] = random_double(-bound_range, bound_range);
            best_xi[i][j] = p_xi[i][j];
            neighBest_val_i[i][j] = p_xi[i][j]; // Initialize neighborhood best to current position
        }
        fitPbest[i] = objective_function(NUM_VARIABLES, p_xi[i]);
        fitnessNeigh[i] = fitPbest[i]; // Initialize neighborhood best fitness
        if (fitPbest[i] < fitnessGlob_b) {
            fitnessGlob_b = fitPbest[i];
            for (int j = 0; j < NUM_VARIABLES; j++) {
                best_position[j] = best_xi[i][j];
            }
        }
    }

    // PSO loop
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        printf("Iteration: %d, Global Fitness: %lf\n", iter, fitnessGlob_b);

        for (int i = 0; i < NUM_PARTICLES; i++) {
            int left_neighbor = (i == 0) ? NUM_PARTICLES - 1 : i - 1;
            int right_neighbor = (i == NUM_PARTICLES - 1) ? 0 : i + 1;

            // Update neighborhood best based on neighbors
            if (fitPbest[left_neighbor] < fitnessNeigh[i]) {
                fitnessNeigh[i] = fitPbest[left_neighbor];
                for (int j = 0; j < NUM_VARIABLES; j++) {
                    neighBest_val_i[i][j] = best_xi[left_neighbor][j];
                }
            }

            if (fitPbest[right_neighbor] < fitnessNeigh[i]) {
                fitnessNeigh[i] = fitPbest[right_neighbor];
                for (int j = 0; j < NUM_VARIABLES; j++) {
                    neighBest_val_i[i][j] = best_xi[right_neighbor][j];
                }
            }

            for (int j = 0; j < NUM_VARIABLES; j++) {
                double r1 = random_double(0, 1), r2 = random_double(0, 1);

                // Update velocity
                vel_i[i][j] = w * vel_i[i][j]
                            + c1 * r1 * (best_xi[i][j] - p_xi[i][j])
                            + c2 * r2 * (neighBest_val_i[i][j] - p_xi[i][j]);

                // Update position with periodic boundaries
                p_xi[i][j] += vel_i[i][j];
                if (p_xi[i][j] < bounds[j].lowerBound) {
                    p_xi[i][j] = bounds[j].lowerBound;
                } else if (p_xi[i][j] > bounds[j].upperBound) {
                    p_xi[i][j] = bounds[j].upperBound;
                }
            }

            // Evaluate fitness for the new position
            double fitness = objective_function(NUM_VARIABLES, p_xi[i]);
            if (fitness < fitPbest[i]) {
                fitPbest[i] = fitness;
                for (int j = 0; j < NUM_VARIABLES; j++) {
                    best_xi[i][j] = p_xi[i][j];
                }
            }

            if (fitness < fitnessGlob_b) {
                fitnessGlob_b = fitness;
                for (int j = 0; j < NUM_VARIABLES; j++) {
                    best_position[j] = p_xi[i][j];
                }
            }
        }
    }

    // Free allocated memory
    for (int i = 0; i < NUM_PARTICLES; i++) {
        free(p_xi[i]);
        free(vel_i[i]);
        free(best_xi[i]);
        free(neighBest_val_i[i]);
    }
    free(p_xi);
    free(vel_i);
    free(best_xi);
    free(neighBest_val_i);
    free(fitPbest);
    free(fitnessNeigh);

    return fitnessGlob_b;
}
