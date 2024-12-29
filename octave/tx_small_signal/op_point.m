r1 = 6.8e3;
r2 = 22e3;
re = 47;
vcc = 9;
hfe = 280;

vth = r2*vcc/(r1+r2);
rth = r2*r1/(r1+r2);
Ic = (vth-.7)/(re + rth/hfe)
Ib = Ic/hfe
Vce = vcc-Ic*re
