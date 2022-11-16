//
// Created by Brian Cheng on 14/11/2022.
//

#ifndef FLUID_SIMULATION_FLUIDCUBE3D_H
#define FLUID_SIMULATION_FLUIDCUBE3D_H
#define WINDOW_SIZE 1024
#define SIZE 60
#define IX(x, y) (x + y * SIZE)

typedef struct FluidCube2D {
    float dt;  //timestamp
    float diff; //diffusion
    float visc; //viscosity

    int* s;
    int* density; //density of dye

    int* Vx;
    int* Vy;

    int* Vx0;//keep old values
    int* Vy0;//keep old values

    FluidCube2D(float diffusion, float viscosity, float dt);
    void FluidCubeAddDensity(int x, int y, float amount);
    void FluidCubeAddVelocity(int x, int y,  float amountX, float amountY);

    void set_bnd(int b, int* x); //prevent fluid overflowing at boundaries
    void lin_solve(int b, int* x, int* x0, float a, float c); //solving linear equations of new cell values


    void diffuse(int b, int*x, int* x0, float diff, float dt);
    void project(int *velocX, int *velocY, int *p, int *div); //incompressible flow
    void advect(int b, int *d, int *d0,  int *velocX, int *velocY, float dt);
    void FluidCubeStep();


    void test();
}FluidCube;


#endif //FLUID_SIMULATION_FLUIDCUBE3D_H
