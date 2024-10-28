
% previamente op_point;

%% Polarizacion
%Ic = ;
%Ib = ;
vt = .7;

% para el 2n3904
Ic_datasheet = 1e-3;
hoe_datasheet = 20e-6;
vaf = Ic_datasheet / hoe_datasheet

hie = vt/Ib;
hfe = Ic/Ib;
hoe = Ic/vaf;
hre = 0;

hib = hie/(1+hfe);
hfb = -hfe/(1+hfe);
hob = hoe/(1+hfe);
hrb = (hie*hoe/(1+hfe))-hre;

hic = hie;
hfc = -(1+hfe);
hoc = hoe;
hrc = 1-hre;
