#include "fluid_sim.h"

#define IX(i,j) ((i)+(N+2)*(j))
#define SWAP(x0,x) {float * tmp=x0;x0=x;x=tmp;}

static void	add_source(int N, float *x, float *s, float dt)
{
	int i, size = (N + 2) * (N + 2);
	for (i = 0; i < size; i++)
		x[i] += dt * s[i];
}

static void	set_bnd(int N, int b, float *x)
{
	int i;

	for (i = 1; i <= N; i++)
	{
		x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
		x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
		x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
		x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
	}
	x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
	x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
	x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
	x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

static void lin_solve(int N, int b, float *x, float *x0, float a, float c)
{
	int i, j, k;

	for (k = 0; k < 20; k++)
	{
		for (i = 1; i <= N; i++)
		{
			for (j = 1; j <= N; j++)
			{
				x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] + x[IX(i, j - 1)] + x[IX(i, j + 1)])) / c;
			}
		}
		set_bnd(N, b, x);
	}
}

static void diffuse(int N, int b, float *x, float *x0, float diff, float dt)
{
	float a = dt * diff * N * N;
	lin_solve(N, b, x, x0, a, 1 + 4 * a);
}

static void advect(int N, int b, float *d, float *d0, float *u, float *v, float dt)
{
	int i, j, i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;

	dt0 = dt * N;
	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= N; j++)
		{
			x = i - dt0 * u[IX(i, j)];
			y = j - dt0 * v[IX(i, j)];
			if (x < 0.5f)
				x = 0.5f;
			if (x > N + 0.5f)
				x = N + 0.5f;
			i0 = (int)x;
			i1 = i0 + 1;
			if (y < 0.5f)
				y = 0.5f;
			if (y > N + 0.5f)
				y = N + 0.5f;
			j0 = (int)y;
			j1 = j0 + 1;
			s1 = x - i0;
			s0 = 1 - s1;
			t1 = y - j0;
			t0 = 1 - t1;
			d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
						s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
		}
	}
	set_bnd(N, b, d);
}

static void project(int N, float *u, float *v, float *p, float *div)
{
	int i, j;

	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= N; j++)
		{
			div[IX(i, j)] = -0.5f * (u[IX(i + 1, j)] - u[IX(i - 1, j)] + v[IX(i, j + 1)] - v[IX(i, j - 1)]) / N;
			p[IX(i, j)] = 0;
		}
	}
	set_bnd(N, 0, div);
	set_bnd(N, 0, p);

	lin_solve(N, 0, p, div, 1, 4);

	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= N; j++)
		{
			u[IX(i, j)] -= 0.5f * N * (p[IX(i + 1, j)] - p[IX(i - 1, j)]);
			v[IX(i, j)] -= 0.5f * N * (p[IX(i, j + 1)] - p[IX(i, j - 1)]);
		}
	}
	set_bnd(N, 1, u);
	set_bnd(N, 2, v);
}

static void dens_step(int N, float *x, float *x0, float *u, float *v, float diff, float dt)
{
	add_source(N, x, x0, dt);
	SWAP(x0, x);
	diffuse(N, 0, x, x0, diff, dt);
	SWAP(x0, x);
	advect(N, 0, x, x0, u, v, dt);
}

static void vel_step(int N, float *u, float *v, float *u0, float *v0, float visc, float dt)
{
	add_source(N, u, u0, dt);
	add_source(N, v, v0, dt);
	SWAP(u0, u);
	diffuse(N, 1, u, u0, visc, dt);
	SWAP(v0, v);
	diffuse(N, 2, v, v0, visc, dt);
	project(N, u, v, u0, v0);
	SWAP(u0, u);
	SWAP(v0, v);
	advect(N, 1, u, u0, u0, v0, dt);
	advect(N, 2, v, v0, u0, v0, dt);
	project(N, u, v, u0, v0);
}

t_fluid_sim fluid_sim_init(int N, float diffusion, float viscosity)
{
	t_fluid_sim	res;

	int size = (N + 2) * (N + 2);
	
	res.N = N;
	res.diff = diffusion;
	res.visc = viscosity;
	
	res.mem = malloc(sizeof(float) * size * 8);
	res.u = &res.mem[size * 0];
	res.v = &res.mem[size * 1];
	res.dens = &res.mem[size * 2];
	res.u_prev = &res.mem[size * 3];
	res.v_prev = &res.mem[size * 4];
	res.dens_prev = &res.mem[size * 5];
	res.divergence_x = &res.mem[size * 6];
	res.divergence_y = &res.mem[size * 7];
	return (res);
}

void fluid_sim_free(t_fluid_sim *sim)
{
	free(sim->mem);
}

void fluid_sim_tick(t_fluid_sim *sim, float delta_time)
{
	vel_step(sim->N, sim->u, sim->v, sim->u_prev, sim->v_prev, sim->visc, delta_time);
	dens_step(sim->N, sim->dens, sim->dens_prev, sim->u, sim->v, sim->diff, delta_time);
}
