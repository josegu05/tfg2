rc = 470;
re = 2.2e3;
rb = 560e3;

hfe = 280;
vcc = 5;

rt = re+rc;
vce = ((vcc - (((hfe+1)*.7*rt)/rb)) / (1+(((hfe+1)*rt)/rb)))
Ic = hfe * ((vce-.7)/rb)
Ib = ((vce-.7)/rb);
