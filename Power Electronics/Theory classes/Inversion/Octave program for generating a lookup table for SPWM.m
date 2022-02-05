clear
fs = 12000; % the switching frequency of the inverter
fc = 72000000; % The clock frequency driving the timer
fm = 100; % frequency of modulation wave (output fundamental)
ami = 0.90; % amplitude modulation index (ratio of modwave to carrier)
fmi = fs/fm; % fmi = Frequency Modulation Index
Arr = fc/fs; % This number goes in to the Auto-Reload register

AngleStep = 2*pi/fmi;
Angle = 0:AngleStep:(2*pi-AngleStep);
Modwave = (ami*sin(Angle)+1)/2;
PWM = Arr*Modwave;
PWM = round(PWM); % if trunc is used instead of round there's a DC bias
csvwrite("pwm.h",PWM)
