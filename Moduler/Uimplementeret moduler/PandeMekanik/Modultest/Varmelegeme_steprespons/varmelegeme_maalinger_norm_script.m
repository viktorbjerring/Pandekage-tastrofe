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
hold on;
plot(T_s_begin, steadyStatePct10,  'o');
plot(T_s_end, steadyStatePct90,  'o');
plot(RC_Time, RC60,  'o');
title('Steprespons for varmelegeme i sekunder');
xlabel('Tid [sek]');
ylabel('Temperatur [°C]');
grid on;

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
G60 = ((steadyState60-steadyStateMinReal60)*(1/(RC_Time-100)))/((1/(RC_Time-100))+ss);
wn=0.0048;
z = 0.93;
G60Own = (wn^2)/((ss^2)+(2*z*wn*ss)+wn^2);
figure('name','Steprespons'); clf;
ezplot((ilaplace(G60*1/ss))/(steadyState60-steadyStateMinReal60),[0,1500]);
hold on;
ezplot(ilaplace(G60Own*1/ss),[0,1500]);
plot((T.Tid50),(T.Temp60-steadyStateMinReal50)/(steadyState50-steadyStateMinReal50));
plot((T.Tid60),(T.Temp60-steadyStateMinReal60)/(steadyState60-steadyStateMinReal60));
plot((T.Tid70),(T.Temp70-steadyStateMinReal70)/(steadyState70-steadyStateMinReal70));
hold off;
legend('1. ordens approx step', '2. ordens approx step', 'Step ved 50% duty', 'Step ved 60% duty', 'Step ved 70% duty', 'Location', 'southeast');
ylim([0 1])

%% Test plot af fit ved 100 % og frekvens plot
figure('name','Steprespons'); clf;
ezplot(ilaplace(G60Own*1/ss),[0,1500]);
hold on;
plot((T.Tid100),(T.Temp100-steadyStateMinReal100)/280);

figure('name', 'Temp'); clf;
plot([50,60,70],[steadyState50,steadyState60,steadyState70]);

%%

ts = 10*60;

kp = 1/0.01-1;
syms ss;
s = tf('s');

G60Own = (wn^2)/((s^2)+(2*z*wn*s)+wn^2);

figure('name','Bodeplot'); clf;
bode(G60Own);

wBW = 0.0067503;
curMag = bode(G60Own, wBW);
curMagdB = 20*log10(curMag);

propMagdB = -1*curMagdB;

propMag = 10^(propMagdB/20);

Gr_1 = G60Own * propMag;

figure('name','Bodeplot'); clf;
margin(Gr_1);
H_1 = feedback(Gr_1,1);

figure('name', 'Stepresponse'); clf;
step(H_1);
hold on;
syms ss;
G60OwnSyms = (wn^2)/((ss^2)+(2*z*wn*ss)+wn^2);
ezplot(ilaplace(G60OwnSyms*1/ss),[0,1500]);
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

%%
T2 = 1.1/wBW;
a = 10^((40-7.3)/20);
GLag=(s+1/T2)/(s+1/(a*T2));
Gr_3 = Gr_2 * GLag;
G60Own_syms = (wn^2)/((ss^2)+(2*z*wn*ss)+wn^2);

figure('name','Bodeplot for Lead'); clf;
bode(GLag)

figure('name','Bodeplot'); clf;
margin(Gr_3);

figure('name','Step'); clf;
step(Gr_3/(1+Gr_3));
hold on;
fplot(ilaplace(G60Own_syms*1/ss),[0,1500],'-r');
plot(T.Tid100,(T.Temp100-steadyStateMinReal100)/(200-steadyStateMinReal100));
legend('reguleret system', 'ureguleret system');
hold off;