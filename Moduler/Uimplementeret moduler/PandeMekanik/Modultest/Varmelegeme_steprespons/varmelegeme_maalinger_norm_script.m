%% Varmelegeme målinger
T = readtable("varmelegeme_maalinger.xlsx");

% I sekunder

figure('name','Steprespons for varmelegme, sekunder'); clf;
plot(T.Tid60,T.Temp60)
xlim([0,1500]);

% finder max:
steadyState50 = max(T.Temp50);
steadyState60 = max(T.Temp60);
steadyState70 = max(T.Temp70);

% finder min:
steadyStateMin50 = min(T.Temp50((100/5):end));
steadyStateMin60 = min(T.Temp60((100/5):end));
steadyStateMin70 = min(T.Temp70((100/5):end));

steadyStateMinReal50 = min(T.Temp50);
steadyStateMinReal60 = min(T.Temp60);
steadyStateMinReal70 = min(T.Temp70);
steadyStateMinReal100 = min(T.Temp100);

% 10% af max;
steadyStatePct10 = (steadyState60-steadyStateMin60)*0.1+steadyStateMin60;

% 90% af max;
steadyStatePct90 = (steadyState60-steadyStateMin60)*0.9+steadyStateMin60;

RC60 = (steadyState60-steadyStateMin60) * (1-exp(-1))+steadyStateMin60;

% Fjerner alle ikke unikke værdier for at kunne bruge interp1
[Temp60uq, Temp60uqIndex] = unique(T.Temp60);

% Finder tiden for 10% og 90%.
T_s_begin = interp1( rmmissing(Temp60uq), rmmissing(T.Tid60(Temp60uqIndex)), steadyStatePct10);
T_s_end = interp1(rmmissing(Temp60uq), rmmissing(T.Tid60(Temp60uqIndex)),  steadyStatePct90);
RC_Time = interp1(rmmissing(Temp60uq), rmmissing(T.Tid60(Temp60uqIndex)),  RC60);

% Finder T_s.
T_s = T_s_end-T_s_begin;
%hold on;
%plot(T_s_begin, steadyStatePct10,  'o');
%plot(T_s_end, steadyStatePct90,  'o');
%plot(RC_Time, RC60,  'o');
%title('Steprespons for varmelegeme i sekunder');
%xlabel('Tid [sek]');
%ylabel('Temperatur [°C]');
%grid on;

%% I minutter

figure('name','Steprespons for varmelegme, minutter'); clf;
plot((T.Tid60/60),T.Temp60,'-o')
xlim([0,25]);
title('Steprespons for varmelegeme i minutter');
xlabel('Tid [min]');
ylabel('Temperatur [°C]');
grid on;


%% Sammenligning af data målinger

figure('name','Steprespons'); clf;
RC100 = interp1(rmmissing(T.Tid100), rmmissing(T.Temp100),  RC_Time);
RC80 = interp1(rmmissing(T.Tid80), rmmissing(T.Temp80),  RC_Time);
plot((T.Tid100),T.Temp100);
hold on; grid on;
plot((T.Tid80),T.Temp80);
plot((T.Tid60),T.Temp60);
plot(T_s_begin, steadyStatePct10,  'o');
plot(T_s_end, steadyStatePct90,  'o');
plot(RC_Time, RC60,  'o');
plot(RC_Time, RC80,  'o');
plot(RC_Time, RC100,  'o');
title('Sammenligning');
xlabel('Tid [m]');
ylabel('Temperatur [°C]');
legend('100%','80%','60%','Location',"best");
hold off;

steadyState100 = RC100/(1-exp(-1));
steadyState80 = RC80/(1-exp(-1));

%%
syms ss;
s = tf('s');
G60 = ((steadyState60)*(1/(RC_Time-100)))/((1/(RC_Time-100))+ss);

wn50=0.0049;
z50 = 0.9;
G50Own = (wn50^2)/((ss^2)+(2*z50*wn50*ss)+wn50^2)*(200-T.Temp50(1))/50+T.Temp50(1)/50;
G50Own_Bode = (wn50^2)/((s^2)+(2*z50*wn50*s)+wn50^2)*(200-T.Temp50(1))/50+T.Temp50(1)/50;

G50OwnOL = 1/(1/G50Own-1);
G50OwnOL_Bode = 1/(1/G50Own_Bode-1);

wn60=0.0043;
z60 = 0.85;
G60Own = (wn60^2)/((ss^2)+(2*z60*wn60*ss)+wn60^2)*(175-T.Temp60(1))/60+T.Temp60(1)/60;
G60Own_Bode = (wn60^2)/((s^2)+(2*z60*wn60*s)+wn60^2)*(175-T.Temp60(1))/60+T.Temp60(1)/60;

G60OwnOL = 1/(1/G60Own-1);
G60OwnOL_Bode = 1/(1/G60Own_Bode-1);

wn70=0.004;
z70 = 0.86;
G70Own = (wn70^2)/((ss^2)+(2*z70*wn70*ss)+wn70^2)*(172-T.Temp70(1))/70+T.Temp70(1)/70;
G70Own_Bode = (wn70^2)/((s^2)+(2*z70*wn70*s)+wn70^2)*(172-T.Temp70(1))/70+T.Temp70(1)/70;

G70OwnOL = 1/(1/G70Own-1);
G70OwnOL_Bode = 1/(1/G70Own_Bode-1);

figure('name','Steprespons'); clf;
%fplot((ilaplace(G60*1/ss))/(steadyState60-steadyStateMinReal60),[0,1500]);
fplot(ilaplace(G50Own*(50/ss)),[0,1800]);
hold on;
fplot(ilaplace(G60Own*(60/ss)),[0,1800]);
fplot(ilaplace(G70Own*70/ss),[0,1800]);
plot((T.Tid50),(T.Temp50));
plot((T.Tid60),(T.Temp60));
plot((T.Tid70),(T.Temp70));
hold off;
legend('epprox 50', 'epprox 60', 'epprox 70', '50', '60', '70', 'Location', 'southeast');
xlabel('Tid [s]');
ylabel('Temperatur [C]');
%ylim([0 1])

figure('name','Bodeplot'); clf;
bode(G50OwnOL_Bode);
hold on;
bode(G60OwnOL_Bode);
bode(G70OwnOL_Bode);
legend('50', '60', '70', 'Location', 'southeast');

%% Test plot af fit ved 100 % og frekvens plot
figure('name','Steprespons'); clf;
fplot(ilaplace(G60Own*1/ss),[0,1500]);
hold on;
plot((T.Tid100),(T.Temp100-steadyStateMinReal100)/280);

figure('name', 'Temp'); clf;
plot([50,60,70],[steadyState50,steadyState60,steadyState70]);

%%

ts = 10*60;

kp = 1/0.01-1;

figure('name','Bodeplot'); clf;
bode(G60OwnOL_Bode);

wBW = 0.0067503;
curMag = bode(G60OwnOL_Bode, wBW);
curMagdB = 20*log10(curMag);

propMagdB = -1*curMagdB;

propMag = 10^(propMagdB/20);

Gr_1 = G60OwnOL_Bode * propMag;

figure('name','Bodeplot'); clf;
margin(Gr_1);
H_1 = feedback(Gr_1,1);

figure('name', 'Stepresponse'); clf;
step(Gr_1/(1+Gr_1));
hold on;
step(G60OwnOL_Bode/(G60OwnOL_Bode+1));
%fplot(ilaplace((G60Own*0.6/ss))/(175-T.Temp60(1)),[0,1500]);
hold off;
%%
beta=(1-sin(45/360*2*pi))/(1+sin(45/360*2*pi));
Kc=sqrt(beta);
T1=1/(wBW*sqrt(beta));
Gl = (1/beta)*((s+1/T1)/(s+1/(beta*T1)))*Kc;
Gr_2 = Gr_1 * Gl;
%%
figure('name','Bodeplot for Lead'); clf;
bode(Gl)

figure('name','Bodeplot'); clf;
margin(Gr_2);

figure('name','Step'); clf;
step(Gr_2/(1+Gr_2));
hold on;
step(Gr_2);
hold off;
%%
T2 = 1.1/wBW;
a = 10^((40-7.3)/20);
GLag=(s+1/T2)/(s+1/(a*T2));
Gr_3 = Gr_2 * GLag;

figure('name','Bodeplot for Lead'); clf;
bode(GLag)

figure('name','Bodeplot'); clf;
margin(Gr_3);

figure('name','Step'); clf;
step(Gr_3/(1+Gr_3));
hold on;
step(G60Own_Bode/293);
hold off;
legend('reguleret system', 'ureguleret system');
hold off;

%%
wBW = 0.0067503;
curMag = bode(G60Own_Bode, wBW);
curMagdB = 20*log10(curMag);

propMagdB = -1*curMagdB;

Gp = 10^(propMagdB/20);

figure('name','step');
step((G60Own_Bode*Gp)/(1+G60Own_Bode*Gp));
figure('name','step');
step(G60Own_Bode);

