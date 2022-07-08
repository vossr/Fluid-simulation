### Fluid simulation

![](sim.gif)

code from paper, i rewrote for rectangular && 3d && engine integration
c software render with wasm emscripten
emcc -sWASM=1 -sSINGLE_FILE=1 -sSTRICT=0 -sASSERTIONS=0 -sMEMORY64=0 -O3

Paper by Jos Stam
	https://www.researchgate.net/publication/2560062_Real-Time_Fluid_Dynamics_for_Games
	https://en.wikipedia.org/wiki/Navier%E2%80%93Stokes_equations
	https://wdv.com/Aerospace/Fluids/StamFluidsforGames.pdf
Patent expired
	https://patents.google.com/patent/US6266071B1/en

In game engine:
![](3d.gif)
