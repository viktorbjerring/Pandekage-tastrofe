fs = 0.2;
T = readtable("varmelegeme_maalinger.xlsx");
wn60=0.0043;
z60 = 0.85;
s = tf('s');
G60Own_Bode = (wn60^2)/((s^2)+(2*z60*wn60*s)+wn60^2)*(175-T.Temp60(1))/60+T.Temp60(1)/60;

figure; clf;
margin(G60Own_Bode);
[Gm,Pm,Wcg,Wcp] = margin(G60Own_Bode);

%%
wBW = 0.0067503;
curMag = bode(G60Own_Bode, wBW);
curMagdB = 20*log10(curMag);

propMagdB = -1*curMagdB;

figure; clf;
Gp = 10^(propMagdB/20);
[Gm1,Pm1,Wcg1,Wcp1] = margin(G60Own_Bode*Gp);
margin(G60Own_Bode*Gp);

%%

figure; clf;
margin(G60Own_Bode*3*((s*150+1)/(s*150)));