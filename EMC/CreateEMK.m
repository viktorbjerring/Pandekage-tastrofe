function Y = CreateEMK(f,l,h,E)
C = 299792458; %Speed of light
f_change = C/(l*2);

A = l*h; %Areal between gnd and line
mu_0 = 1.2566370621219*10^(-6); %Vacuum permeability

%Makes everything 0 until f_change then everything is 1
f_atChange = ceil(floor(f/f_change)/max(f));

%Changes f_val from f_change and up to f_change. To make sure that f_change
%is max.
f_val = abs(f_atChange-1).*f+f_atChange.*f_change;

%Calculates EMK
Y = 2 * pi * f_val * mu_0 * A * E/377;

end