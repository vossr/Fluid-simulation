#### Try here: https://rpehkone.github.io/Navier-Stokes-real-time/wasm/

![](sim.gif)

Motivation  
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Raymarched fluid sims are state of the art in real-time vfx  
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;https://www.youtube.com/c/AndrasKetzer/videos  

Code from paper, i rewrote for rectangular && 3d && engine integration  
c software render with wasm emscripten  
emcc -sWASM=1 -sSINGLE_FILE=1 -sSTRICT=0 -sASSERTIONS=0 -sMEMORY64=0 -O3  

Paper by Jos Stam  
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;https://www.researchgate.net/publication/2560062_Real-Time_Fluid_Dynamics_for_Games  
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;https://en.wikipedia.org/wiki/Navier%E2%80%93Stokes_equations  
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;https://wdv.com/Aerospace/Fluids/StamFluidsforGames.pdf  
Patent expired  
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;https://patents.google.com/patent/US6266071B1/en  

In game engine:  
![](3d.gif)


