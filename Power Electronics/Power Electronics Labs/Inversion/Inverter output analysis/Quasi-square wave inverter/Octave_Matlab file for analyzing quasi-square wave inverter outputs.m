% Modelling quasi square wave inverter output_max_field_width
clear
close all
Timer_Frequency=1000000;
Output_Frequency=50;
Output_Duty=0.99
CycleCount=8;
Quarter_Cycle_Low_Clocks=(1-Output_Duty)*Timer_Frequency/(4*Output_Frequency);
Quarter_Cycle_High_Clocks=(Output_Duty)*Timer_Frequency/(4*Output_Frequency);
SingleCycleOutput=zeros(1,Timer_Frequency/Output_Frequency);

start=1;
stop=Quarter_Cycle_Low_Clocks-1;
SingleCycleOutput(start:stop)=0;

start=stop+1;
stop=start+Quarter_Cycle_High_Clocks-1;
SingleCycleOutput(start:stop)=1;

start=stop+1;
stop=start+Quarter_Cycle_High_Clocks-1;
SingleCycleOutput(start:stop)=1;

start=stop+1;
stop=start+Quarter_Cycle_Low_Clocks-1;
SingleCycleOutput(start:stop)=0;



start=stop+1;
stop=start+Quarter_Cycle_Low_Clocks-1;
SingleCycleOutput(start:stop)=0;

start=stop+1;
stop=start+Quarter_Cycle_High_Clocks-1;
SingleCycleOutput(start:stop)=-1;

start=stop+1;
stop=start+Quarter_Cycle_High_Clocks-1;
SingleCycleOutput(start:stop)=-1;

start=stop+1;
stop=start+Quarter_Cycle_Low_Clocks;
SingleCycleOutput(start:stop)=0;

MultiCycleOutput=SingleCycleOutput;
for count=2:CycleCount
  MultiCycleOutput=[MultiCycleOutput SingleCycleOutput];
endfor
EndTime=CycleCount/Output_Frequency-1/Timer_Frequency;
t=0:1/Timer_Frequency:EndTime;
plot(t,MultiCycleOutput);
axis([0,EndTime,-1.1,1.1]);
CurrentFigure=gcf();
set(CurrentFigure,"name","Time domain");
figure();
ft=abs(fft(MultiCycleOutput))*2/length(MultiCycleOutput);
SpecPlotLen=1000;
Freqs=0:1:SpecPlotLen-1;
FirstFewHarmonics=ft(1:SpecPlotLen);
CurrentFigure=gcf();
set(CurrentFigure,"name","Spectrum");
plot(Freqs*Output_Frequency/CycleCount,FirstFewHarmonics);
square_sum=0
for harmonic=CycleCount+2:length(FirstFewHarmonics)
  square_sum=square_sum+FirstFewHarmonics(harmonic)*FirstFewHarmonics(harmonic);
endfor
THD=sqrt(square_sum)/FirstFewHarmonics(CycleCount+1)
