#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "utility.h"

// Helper function to generate random numbers in a range
double random_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

double pso(ObjectiveFunction objective_function, int NUM_VARIABLES, Bound *bounds, int NUM_PARTICLES, int MAX_ITERATIONS, double *best_position) {
    // Allocate memory for particles
    double **p_xi = (double**)malloc(NUM_PARTICLES * sizeof(double*));    // Particle positions
    double **vel_i = (double**)malloc(NUM_PARTICLES * sizeof(double*));    // Particle velocities
    double **best_xi = (double**)malloc(NUM_PARTICLES * sizeof(double*));  // Personal best positions
    double *fitPbest = (double*)malloc(NUM_PARTICLES * sizeof(double));    // Fitness of personal best
    double *globalBest_val_i = (double*)malloc(NUM_VARIABLES * sizeof(double));  // Global best position
    double fitnessGlob_b = DBL_MAX;  // Global best fitness, initialized to infinity

    // Check for failed memory allocations
    if (p_xi == NULL || vel_i == NULL || best_xi == NULL || fitPbest == NULL || globalBest_val_i == NULL) {
        printf("Memory allocation failed!\n");
        return 1;  // Exit with error
    }

    for (int i = 0; i < NUM_PARTICLES; i++) {
        p_xi[i] = (double*)malloc(NUM_VARIABLES * sizeof(double));
        vel_i[i] = (double*)malloc(NUM_VARIABLES * sizeof(double));
        best_xi[i] = (double*)malloc(NUM_VARIABLES * sizeof(double));
        if (p_xi[i] == NULL || vel_i[i] == NULL || best_xi[i] == NULL) {
            printf("Memory allocation failed at particle %d!\n", i);
            return 1;  // Exit with error
        }
    }

    // Initialize constants
    double w = 0.7;   // Inertia weight
    double c1 = 1.5;  // Cognitive coefficient
    double c2 = 1.5;  // Social coefficient

    // Step 1: Initialize positions, velocities, and personal bests
    for (int i = 0; i < NUM_PARTICLES; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            p_xi[i][j] = random_double(bounds[j].lowerBound, bounds[j].upperBound);
            vel_i[i][j] = random_double(-fabs(bounds[j].upperBound - bounds[j].lowerBound),
                                        fabs(bounds[j].upperBound - bounds[j].lowerBound));
            best_xi[i][j] = p_xi[i][j];  // Personal best starts as the initial position
        }
        // Evaluate fitness for the initial position
        fitPbest[i] = objective_function(NUM_VARIABLES, p_xi[i]);

        // Update global best if the current particle's fitness is better
        if (fitPbest[i] < fitnessGlob_b) {
            fitnessGlob_b = fitPbest[i];
            for (int j = 0; j < NUM_VARIABLES; j++) {
                globalBest_val_i[j] = best_xi[i][j];
            }
        }
    }

    // Step 2: Iterate over the maximum number of iterations
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        for (int i = 0; i < NUM_PARTICLES; i++) {
            for (int j = 0; j < NUM_VARIABLES; j++) {
                // Generate random numbers for velocity update
                double r1 = random_double(0, 1);
                double r2 = random_double(0, 1);

                // Update velocity
                vel_i[i][j] = w * vel_i[i][j]
                            + c1 * r1 * (best_xi[i][j] - p_xi[i][j])
                            + c2 * r2 * (globalBest_val_i[j] - p_xi[i][j]);

                // Update position
                p_xi[i][j] += vel_i[i][j];

                // Clamp position within bounds
                if (p_xi[i][j] < bounds[j].lowerBound) {
                    p_xi[i][j] = bounds[j].lowerBound;
                } else if (p_xi[i][j] > bounds[j].upperBound) {
                    p_xi[i][j] = bounds[j].upperBound;
                }
            }

            // Evaluate fitness for the new position
            double fitness = objective_function(NUM_VARIABLES, p_xi[i]);

            // Update personal best if the new position is better
            if (fitness < fitPbest[i]) {
                fitPbest[i] = fitness;
                for (int j = 0; j < NUM_VARIABLES; j++) {
                    best_xi[i][j] = p_xi[i][j];
                }
            }

            // Update global best if the new position is better
            if (fitness < fitnessGlob_b) {
                fitnessGlob_b = fitness;
                for (int j = 0; j < NUM_VARIABLES; j++) {
                    globalBest_val_i[j] = p_xi[i][j];
                }
            }
        }
    }

    // Copy global best position to the output array
    for (int j = 0; j < NUM_VARIABLES; j++) {
        best_position[j] = globalBest_val_i[j];
    }

    // Free allocated memory
    for (int i = 0; i < NUM_PARTICLES; i++) {
        free(p_xi[i]);
        free(vel_i[i]);
        free(best_xi[i]);
    }
    free(p_xi);
    free(vel_i);
    free(best_xi);
    free(fitPbest);
    free(globalBest_val_i);

    // Return the best fitness value
    return fitnessGlob_b;
}
