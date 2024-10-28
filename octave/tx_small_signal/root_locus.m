
numAf = []; % forma N*s^n + ... + a*s^2 + b*s + c
denAf = []; % forma N*s^n + ... + a*s^2 + b*s + c
Af = tf(numAf,denAf)
figure
rlocus(Af)
