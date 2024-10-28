clear all;
close all;

op_point;
transistor_parameters;

hib_num =hib;
hfb_num =hfb;
hob_num =hob;
hrb_num =hrb;
re_num = re;
l1_num = 630e-9;
c1_num = 47e-12;
c2_num = 47e-12;


syms s c2 c1 l1 hib hob hfb hrb re;
% previamente transistor_parameters;

yt2 = hob +(1/(s*l1))+(s*c1);
yt1 = (s*c1) + (s*c2) + (1/re) + (1/hib);

% a = -hfb/(yt2*(1+ yt1/hib));
a = -hfb/(yt2*yt1*hib);
f = -s*c1;

Afsym = factor(a*f, s)
% Afsym = factor(a/(1+a*f), s)

%% Substitutions
hib_sym = float2sym(hib_num);
hfb_sym = float2sym(hfb_num);
hob_sym = float2sym(hob_num);
hrb_sym = float2sym(hrb_num);
l1_sym = vpa(l1_num, 20);
c1_sym= vpa(c1_num, 20);
c2_sym= vpa(c2_num, 20);
re_sym = sym(re_num);

Afsym = subs(Afsym, l1, l1_sym);
Afsym = subs(Afsym, c1, c1_sym);
Afsym = subs(Afsym, c2, c2_sym);
Afsym = subs(Afsym, re, re_sym);

Afsym = subs(Afsym, hib, hib_sym);
Afsym = subs(Afsym, hfb, hfb_sym);
Afsym = subs(Afsym, hob, hob_sym);
Afsym = subs(Afsym, hrb, hrb_sym);

Afsym = expand(Afsym)
[num,den]=numden(Afsym);
coeffs_num = fliplr(double(coeffs(num)))
coeffs_den = fliplr(double(coeffs(den)))
coeffs_num = [coeffs_num,0,0];

Al = tf(coeffs_num, coeffs_den)
figure
bode(Al)
