typedef struct		t_fluid_sim
{
	int				N;
	float			diff;
	float			visc;
	
	float			*mem;
	float			*divergence_x;
	float			*divergence_y;
	float			*u;
	float			*v;
	float			*dens;
	float			*u_prev;
	float			*v_prev;
	float			*dens_prev;
}					t_fluid_sim;

void		fluid_sim_tick(t_fluid_sim *sim, float delta_time);
t_fluid_sim	fluid_sim_init(int N, float diffusion, float viscosity);
void		fluid_sim_free(t_fluid_sim *sim);
