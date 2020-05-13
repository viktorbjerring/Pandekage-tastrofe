

f = logspace(3,10,100000);
E = 1;
V0101 = CreateEMK(f,0.1,0.1,E);
V0501 = CreateEMK(f,0.5,0.1,E);
V0505 = CreateEMK(f,0.5,0.5,E);
V0510 = CreateEMK(f,0.5,1,E);
V1010 = CreateEMK(f,1,1,E);


figure; clf;
loglog(f,V0101);
hold on;
loglog(f,V0501);
loglog(f,V0505);
loglog(f,V0510);
loglog(f,V1010);
legend('l = 0.1 m, h = 0.1 m', ...
       'l = 0.5 m, h = 0.1 m', ...
       'l = 0.5 m, h = 0.5 m', ...
       'l = 0.5 m, h = 1 m', ...
       'l = 1 m, h = 1 m', ...
       'Location', 'southeast');
title('EMK induceret i strømsløjfer af forskellige størrelser (E=1 V/m)');
xlabel('Frekvens [Hz]');
ylabel('Induceret spænding [V]');
grid on
ax = gca;
ax.XAxis.TickValues = [10^3,10^4,10^5,10^6,10^7,10^8,10^9,10^10];
ax.YAxis.TickValues = [10^(-7),10^(-6),10^(-5),10^(-4),10^(-3),10^(-2),10^(-1),10^0,10^1];
ax.MinorGridLineStyle = 'none';
xlim([10^3,10^10]);
ylim([10^(-7),10^1]);