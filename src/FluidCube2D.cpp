//
// Created by Brian Cheng on 14/11/2022.
//
#include <unistd.h>
#include "iostream"

#define iter 3
#include "FluidCube2D.h"
#include "math.h"


FluidCube2D::FluidCube2D(float diffusion, float viscosity, float dt){

    this->dt = dt;
    this->diff = diffusion;
    this->visc = viscosity;

    this->s =       (float*)std::calloc(SIZE * SIZE, sizeof(float));
    this->density = (float*)std::calloc(SIZE * SIZE, sizeof(float));

    this->Vx =      (float*)std::calloc(SIZE * SIZE, sizeof(float));
    this->Vy =      (float*)std::calloc(SIZE * SIZE, sizeof(float));

    this->Vx0 =     (float*)std::calloc(SIZE * SIZE, sizeof(float));
    this->Vy0 =     (float*)std::calloc(SIZE * SIZE, sizeof(float));
}

void FluidCube2D::FluidCubeAddDensity(int x, int y, float amount){
    this->density[IX(x,y)] += amount;
}

void FluidCube2D::FluidCubeAddVelocity(int x, int y, float amountX, float amountY){
    int index = IX(x,y);

    this->Vx[index] += amountX;
    this->Vy[index] += amountY;
}

void FluidCube2D::set_bnd(int b, float* x){
// bouncing off the walls
        for(int i = 1; i < SIZE - 1; i++) {
            x[IX(i, 0  )] = b == 2 ? -x[IX(i, 1  )] : x[IX(i, 1  )];
            x[IX(i, SIZE - 1)] = b == 2 ? -x[IX(i, SIZE - 2)] : x[IX(i, SIZE - 2)];
        }
        for(int j = 1; j < SIZE - 1; j++) {
            x[IX(0  , j)] = b == 1 ? -x[IX(1  , j)] : x[IX(1  , j)];
            x[IX(SIZE - 1, j)] = b == 1 ? -x[IX(SIZE - 2, j)] : x[IX(SIZE - 2, j)];
        }

// corners
    x[IX(0, 0)]       = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, SIZE - 1)]     = 0.5f * (x[IX(1, SIZE - 1)] + x[IX(0, SIZE - 2)]);
    x[IX(SIZE - 1, 0)]     = 0.5f * (x[IX(SIZE - 2, 0)] + x[IX(SIZE - 1, 1)]);
    x[IX(SIZE - 1, SIZE - 1)]   = 0.5f * (x[IX(SIZE - 2, SIZE - 1)] + x[IX(SIZE - 1, SIZE - 2)]);

}

void FluidCube2D::lin_solve(int b, float* x, float* x0, float a, float c){
    float cRecip = 1.0/c;

    for (int k=0; k<iter; k++){
            for (int j=1; j < SIZE - 1; j++){
                for (int i=1; i < SIZE - 1; i++){
                    x[IX(i,j)] =
                            (x0[IX(i,j)] + a*(
                                    x[IX(i+1,j)] +
                                    x[IX(i-1,j)] +
                                    x[IX(i,j+1)] +
                                    x[IX(i,j-1)]
                            )) * cRecip;
                }
        }
        set_bnd(b,x);
    }
}


void FluidCube2D::diffuse(int b, float*x, float* x0, float diff, float dt){
    float a = dt * diff * (SIZE - 2) * (SIZE - 2);
    lin_solve(b,x,x0, a,1+6*a);
}

void FluidCube2D::project(float *velocX, float *velocY, float *p, float *div)
{
        for (int j = 1; j < SIZE - 1; j++) {
            for (int i = 1; i < SIZE - 1; i++) {
                div[IX(i, j)] = -0.5f * (
                         velocX[IX(i+1, j)]
                        -velocX[IX(i-1, j)]
                        +velocY[IX(i , j+1)]
                        -velocY[IX(i , j-1)]
                ) / SIZE;
                p[IX(i, j)] = 0;
        }
    }
    set_bnd(0, div);
    set_bnd(0, p);
    lin_solve(0, p, div, 1, 6);


        for (int j = 1; j < SIZE - 1; j++) {
            for (int i = 1; i < SIZE - 1; i++) {
                velocX[IX(i, j)] -= 0.5f * (  p[IX(i+1, j)]
                                              -p[IX(i-1, j)]) * SIZE;
                velocY[IX(i, j)] -= 0.5f * (  p[IX(i, j+1)]
                                              -p[IX(i, j-1)]) * SIZE;
            }
        }

    set_bnd(1, velocX);
    set_bnd(2, velocY);
}

void FluidCube2D::advect(int b, float *d, float *d0, float *velocX, float *velocY, float dt)
{
    float i0, i1, j0, j1;

    float dtx = dt * (SIZE - 2);
    float dty = dt * (SIZE - 2);

    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;

    float Nfloat = SIZE;
    float ifloat, jfloat;
    int i, j;

        for(j = 1, jfloat = 1; j < SIZE - 1; j++, jfloat++) {
            for(i = 1, ifloat = 1; i < SIZE - 1; i++, ifloat++) {
                tmp1 = dtx * velocX[IX(i, j)];
                tmp2 = dty * velocY[IX(i, j)];
                x    = ifloat - tmp1;
                y    = jfloat - tmp2;

                if(x < 0.5f) x = 0.5f;
                if(x > Nfloat + 0.5f) x = Nfloat + 0.5f;
                i0 = floorf(x);
                i1 = i0 + 1.0f;
                if(y < 0.5f) y = 0.5f;
                if(y > Nfloat + 0.5f) y = Nfloat + 0.5f;
                j0 = floorf(y);
                j1 = j0 + 1.0f;

                s1 = x - i0;
                s0 = 1.0f - s1;
                t1 = y - j0;
                t0 = 1.0f - t1;

                int i0i = i0;
                int i1i = i1;
                int j0i = j0;
                int j1i = j1;

                d[IX(i, j)] =
                        s0 * ( t0 * d0[IX(i0i, j0i)] + t1 * d0[IX(i0i, j1i)]) +
                        s1 * ( t0 * d0[IX(i1i, j0i)] + t1 * d0[IX(i1i, j1i)]) ;
            }
        }
    set_bnd(b, d);
}

void FluidCube2D::FluidCubeStep(){
    float visc = this->visc;
    float diff = this->diff;
    float dt = this->dt;

    float* Vx = this->Vx;
    float* Vy = this->Vy;

    float* Vx0 = this->Vx0;
    float* Vy0 = this->Vy0;

    float* s = this->s;
    float* density = this->density;

    diffuse(1, Vx0, Vx, visc, dt);
    diffuse(2, Vy0, Vy, visc, dt);

    project(Vx0, Vy0, Vx, Vy);

    advect(1,Vx, Vx0, Vx0, Vy0, dt);
    advect(2,Vy, Vy0, Vx0, Vy0, dt);

    project(Vx, Vy, Vx0, Vy0);

    diffuse(0, s, density, diff, dt);
    advect(0,density, s,Vx,Vy,dt);
}

void FluidCube2D::test() {
    this->density[0] += 1;
    std::cout << this->density[0] << std::endl;
    sleep(2);
}

