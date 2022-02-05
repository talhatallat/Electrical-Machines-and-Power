clear
close all
Timer_Frequency=1000000; % 1MHz clock frequency
Output_Frequency=50; % Want to output a 50Hz sine
Amplitude_Modulation_Index=1.4; % Amplitude of mod wave of Ampl of Carrier wave
Frequency_Modulation_Index=50; % Frequency of carrier wave over mod wave
NumCycles = 5; % How many cycles to simulate
DCBridgeVoltage=1;
Carrier_Frequency=Output_Frequency*Frequency_Modulation_Index;
modulating_wave=0;
c=-1;
%
anglestep = (2*pi)/Frequency_Modulation_Index;
angle=0;
% make space in the various arrays
block_length=floor(NumCycles*Timer_Frequency/Output_Frequency+0.5);
OutputData=zeros(1,1+block_length);
carrier=zeros(1,1+block_length);
modulating_wave=zeros(1,1+block_length);
time=zeros(1,1+block_length);
% Generate the waveforms
index=1;
for t=0:1/Timer_Frequency:NumCycles/Output_Frequency
	c = c+2*Carrier_Frequency/Timer_Frequency;
	if (c >= 1)
		c = -1;
	end  	
	angle=angle+anglestep;
% record all the data for plotting later
  carrier(index) = c;
	modulating_wave(index)=Amplitude_Modulation_Index*sin(2*pi*Output_Frequency*t);
  % Compare modulating wave to carrier and scale to +/-1
	OutputData(index)=2*(modulating_wave(index)>carrier(index))-1;
	time(index)=t;
	index=index+1;
end

hold off;
clf;
subplot(2,1,1);
plot(time,modulating_wave,'b');
hold on
plot(time,carrier,'g');
subplot(2,1,2);
plot(time,OutputData);
axis([0,t,-1.1,1.1]);
hold on
figurehandle=gcf();
set(figurehandle,"name","Time domain");
% Now plot the spectrum of the output signal
figure()
figurehandle=gcf();
ft=abs(fft(OutputData))*2/(length(OutputData)-1);
SpecPlotLen=floor(Carrier_Frequency/(2*NumCycles));
Freqs=0:1:SpecPlotLen-1;
FirstFewHarmonics=ft(1:SpecPlotLen);

%plot(abs(fft(OutputData))*2/(length(OutputData)-1))
plot(Freqs*Output_Frequency/NumCycles,DCBridgeVoltage*FirstFewHarmonics)
set(figurehandle,"name","Spectrum");




