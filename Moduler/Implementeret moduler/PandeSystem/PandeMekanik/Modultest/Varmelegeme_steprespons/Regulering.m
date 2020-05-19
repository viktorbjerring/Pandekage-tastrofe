%% Load Data
clear
clc
close all
T = readtable("varmelegeme_maalinger.xlsx");

%%
syms ss;
s = tf('s');

wn50=0.0049;
z50 = 0.9;
G50Own = (wn50^2)/((ss^2)+(2*z50*wn50*ss)+wn50^2)*(200-T.Temp50(1))/0.5+T.Temp50(1)/0.5;
G50Own_Bode = (wn50^2)/((s^2)+(2*z50*wn50*s)+wn50^2)*(200-T.Temp50(1))/0.5+T.Temp50(1)/0.5;

G50OwnOL = 1/(1/G50Own-1);
G50OwnOL_Bode = 1/(1/G50Own_Bode-1);

wn60=0.0043;
z60 = 0.85;
G60Own = (wn60^2)/((ss^2)+(2*z60*wn60*ss)+wn60^2)*(175-T.Temp60(1))/0.6+T.Temp60(1)/0.6;
G60Own_Bode = (wn60^2)/((s^2)+(2*z60*wn60*s)+wn60^2)*(175-T.Temp60(1))/0.6+T.Temp60(1)/0.6;

G60OwnOL = 1/(1/G60Own-1);
G60OwnOL_Bode = 1/(1/G60Own_Bode-1);

wn70=0.004;
z70 = 0.86;
G70Own = (wn70^2)/((ss^2)+(2*z70*wn70*ss)+wn70^2)*(172-T.Temp70(1))/0.7+T.Temp70(1)/0.7;
G70Own_Bode = (wn70^2)/((s^2)+(2*z70*wn70*s)+wn70^2)*(172-T.Temp70(1))/0.7+T.Temp70(1)/0.7;

G70OwnOL = 1/(1/G70Own-1);
G70OwnOL_Bode = 1/(1/G70Own_Bode-1);

figure('name','Steprespons'); clf;
%fplot((ilaplace(G60*1/ss))/(steadyState60-steadyStateMinReal60),[0,1500]);
fplot(ilaplace(G50Own*(0.5/ss)),[0,1800]);
hold on;
fplot(ilaplace(G60Own*(0.6/ss)),[0,1800]);
fplot(ilaplace(G70Own*0.7/ss),[0,1800]);
plot((T.Tid50),(T.Temp50));
plot((T.Tid60),(T.Temp60));
plot((T.Tid70),(T.Temp70));
hold off;
legend('epprox 50', 'epprox 60', 'epprox 70', '50', '60', '70', 'Location', 'southeast');
%ylim([0 1])

figure('name','Bodeplot'); clf;
bode(G50OwnOL_Bode);
hold on;
bode(G60OwnOL_Bode);
bode(G70OwnOL_Bode);
legend('50', '60', '70', 'Location', 'southeast');
%% Temp to Duty from mesure:

%DUTY 70%
meas70 = [199.5 199.8 199.8 199.9 200.0 200.0 200.0 200.0 200.0 200.0 200.0 199.9 200.0 200.0 200.0];
%DUTY 65%
meas65 = [187.2 187.1 187.7 187.3 187.3 186.5 187.4 186.6 187.4 187.4 187.1 186.4 188.6 187.2 187.2];

%DUTY 60%
meas60 = [176.7 177.1 176.9 177.0 177.0 177.0 176.6 177.1 176.6 176.3 175.7 176.0 176.2 176.0 176.2];

%DUTY 55%
meas55 = [173.5 172.9 174.3 174.2 173.8 174.2 173.8 173.9 173.8 174.2 174.3 173.7 174.5 173.8 174.1];

%DUTY 50%
meas50 = [172.2 172.3 172.2 172.6 172.6 172.5 172.5 172.3 172.4 172.1 172.5 172.0 172.3 172.1 172.0];

%DUTY 45%
meas45 = [164.1 164.0 163.0 162.5 163.5 163.0 163.5 163.3 163.0 162.8 161.8 162.9 163.0 162.3 162.6];

%DUTY 35%
meas35 = [144.8 144.7 144.9 145.1 144.9 143.9 143.9 144.4 144.3 143.6 143.7 144.4 143.2 143.6 143.8];

allData = [meas35(:);meas45(:);meas50(:);meas60(:);meas65(:);meas70(:)];
allDuty = [ones(length(meas35),1)*0.35;ones(length(meas45),1)*0.45;ones(length(meas50),1)*0.50;ones(length(meas60),1)*0.60;ones(length(meas65),1)*0.65;ones(length(meas70),1)*0.7];

f = LinearModel.fit(allDuty,allData,'b*x^m');

figure('name','Temp over duty-cycle');
X = [0.35 0.45 0.50 0.55 0.60 0.65 0.70];
plot(allDuty,allData,'.');
[a,b] = lineFit(allDuty,allData);
hold on;
plot(X,a*X+b);
Y = a*X+b;
X_ny = (Y-b)/a;
fplot(f);

%% Temp to Duty from tf:
syms t;
X_tf = 0:0.1:1;
T_f = ilaplace(G60Own*X_tf/ss);
Y_tf = double(limit(T_f, t,inf));
[a_tf,b_tf] = lineFit(X_tf,Y_tf);

figure('name','Temp over duty-cycle');
plot(allDuty,allData,'.');
hold on;
plot(X_tf,a*X_tf+b);
plot(X_tf,a_tf*X_tf+b_tf);

%%