//
// Created by Brian Cheng on 14/11/2022.
//

#ifndef FLUID_SIMULATION_FLUIDCUBE3D_H
#define FLUID_SIMULATION_FLUIDCUBE3D_H

typedef struct FluidCube3D {
    int size;
    float dt;  //timestamp
    float diff; //diffusion
    float visc; //viscosity

    float* s;
    float* density; //density of dye

    float *Vx;
    float* Vy;
    float* Vz;

    float *Vx0;//keep old values
    float* Vy0;//keep old values
    float* Vz0;//keep old values

    FluidCube3D(int size, int diffusion, int viscosity, float dt);
    void FluidCubeAddDensity(FluidCube3D& cube, int x, int y, int z, float amount);
    void FluidCubeAddVelocity(FluidCube3D& cube, int x, int y, int z, float amountX, float amountY, float amountZ);

    void set_bnd(int b, float* x, int N); //prevent fluid overflowing at boundaries
    void lin_solve(int b, float* x, float* x0, float a, float c, int iter, int N); //update new values in cells


    void diffuse(int b, float*x, float* x0, float diff, float dt, int iter, int N);
    void project(float *velocX, float *velocY, float *velocZ, float *p, float *div, int iter, int N); //incompressible flow
    void advect(int b, float *d, float *d0,  float *velocX, float *velocY, float *velocZ, float dt, int N);
    void FluidCubeStep(FluidCube3D* cube);

}FluidCube;


#endif //FLUID_SIMULATION_FLUIDCUBE3D_H
