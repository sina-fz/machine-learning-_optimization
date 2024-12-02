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
    double *fitPbest = (double*) malloc(NUM_PARTICLES*sizeof(double *));
    double *globalBest_val_i = (double*)malloc(NUM_VARIABLES*sizeof(double *));
    double *fitnessGlob_b = DBL_MAX; 
    // Check for failed memory allocations 
    if (p_xi == NULL || vel_i == NULL || best_xi == NULL || fitPbest == NULL || globalBest_val_i == NULL) { 
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

    //initalize randome positions and velocities for the particles
    for (int i = 0; i< NUM_PARTICLES;i++){
        for(int j = 0; j<NUM_VARIABLES; j++){
            P_xi[i][j] = random_double(bounds[j].lowerBound,bounds[j].upperBound);
            vel_i[i][j] = random_double(-fabs(bounds[j].lowerBound-bounds[j].upperBound),fabs(bounds[j].lowerBound-bounds[j].upperBound));
            best_xi[i][j] = P_xi[x][j];
        }
        fitPbest = objective_function(NUM_VARIABLES,P_xi[i]);
        if (fitPbest < fitnessGlob_b){
            fitnessGlob_b = fitPbest;
            for (int j = 0; j<NUM_VARIABLES; j++){
                gloabalBest_val_i [j] = best_xi[i][j];
            }
        }

        gloabalBest_val_i[]
    }








    // CODE: implement pso function here
}