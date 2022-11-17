//
// Created by Brian Cheng on 14/11/2022.
//

#ifndef FLUID_SIMULATION_FLUIDCUBE2D_H
#define FLUID_SIMULATION_FLUIDCUBE2D_H
#define WINDOW_SIZE 1024
#define SIZE 64
#define IX(x, y) (x + y * SIZE)

typedef struct FluidCube2D {
    float dt;  //timestamp
    float diff; //diffusion
    float visc; //viscosity

    float* s;
    float* density; //density of dye

    float* Vx;
    float* Vy;

    float* Vx0;//keep old values
    float* Vy0;//keep old values

    FluidCube2D(float diffusion, float viscosity, float dt);
    void FluidCubeAddDensity(int x, int y, float amount);
    void FluidCubeAddVelocity(int x, int y,  float amountX, float amountY);

    void set_bnd(int b, float* x); //prevent fluid overflowing at boundaries
    void lin_solve(int b, float* x, float* x0, float a, float c); //solving linear equations of new cell values


    void diffuse(int b, float*x, float* x0, float diff, float dt);
    void project(float *velocX, float *velocY, float *p, float *div); //incompressible flow
    void advect(int b, float *d, float *d0,  float *velocX, float *velocY, float dt);
    void FluidCubeStep();

    void fadeout();


}FluidCube2D;


#endif //FLUID_SIMULATION_FLUIDCUBE2D_H
