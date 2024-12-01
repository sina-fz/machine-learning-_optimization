// CODE: include library(s)

// Helper function to generate random numbers in a range
double random_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

// CODE: implement other functions here if necessary

double pso(ObjectiveFunction objective_function, int NUM_VARIABLES, Bound *bounds, int NUM_PARTICLES, int MAX_ITERATIONS, double *best_position) {

    double **p_xi = (double**) malloc(NUM_PARTICLES*sizeof(double *));
    double **vel_i = (double**) malloc(NUM_PARTICLES*sizeof(double *));
    double **best_xi = (double**)malloc(NUM_PARTICLES*sizeof(double *));
    double *fitnessVal_i = (double*) malloc(MAX_ITERATIONS*sizeof(double *));
    double *globalBest_val_i = (double*)malloc(NUM_VARIABLES*sizeof(double *));
    // Check for failed memory allocations 
    if (p_xi == NULL || vel_i == NULL || best_xi == NULL || fitnessVal_i == NULL || globalBest_val_i == NULL) { 
        printf("Memory allocation failed!\n"); 
        return 1; }

    for (int i = 0; i < NUM_PARTICLES; i++){
        p_xi[i] = (double*) malloc(NUM_VARIABLES* sizeof(double));
        vel_i[i] = (double*) malloc(NUM_VARIABLES* sizeof(double));
        best_xi[i] = (double*) malloc(NUM_VARIABLES* sizeof(double));
        // Check for failed memory allocations in the loop 
        if (p_xi[i] == NULL || vel_i[i] == NULL || best_xi[i] == NULL) {
            printf("Memory allocation failed at particle %d!\n", i); 
            return 1;}
    }
    




    // CODE: implement pso function here
}