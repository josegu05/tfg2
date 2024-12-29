%% calcular L dado N
uo = 4*pi*1e-7;
ur = 1;
N = 12;
d = .5e-2;
l = .8e-2;

disp('dado N')
N
A = (d/2)^2 *pi;
L = ur*uo*N^2*A/l

%% calcular N dado L
uo = 4*pi*1e-7;
ur = 1;
L = 10e-6;
d = .5e-2;
l = .8e-2;

disp('dado L')
A = (d/2)^2 *pi;
L
N = sqrt((l*L)/(ur*uo*A))
