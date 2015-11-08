fcfs = load('test_distribution_fcfs_10G.bin');
ssf = load('test_distribution_ssf_10G.bin');
elevator = load('test_distribution_elevator_10G.bin');
track_count = 16383;
y_min = min( min(elevator(:,2)./elevator(:,3)), min (min(ssf(:,2)./ssf(:,3)), min(fcfs(:,2)./fcfs(:,3))) );
y_max = max( max(elevator(:,2)./elevator(:,3)), max (max(ssf(:,2)./ssf(:,3)), max(fcfs(:,2)./fcfs(:,3))) );
figure
subplot(2,2,1)
plot (elevator(:,1),elevator(:,2)./elevator(:,3),'.b')
xlim([0,track_count])
ylim([y_min, y_max])
title('Elevator algorithm')
xlabel('Track number')
ylabel('Average seek time')
subplot(2,2,2)
plot (ssf(:,1),ssf(:,2)./ssf(:,3),'.k')
xlim([0,track_count])
ylim([y_min, y_max])
title('SSF')
xlabel('Track number')
ylabel('Average seek time')
subplot(2,2,3)
plot (fcfs(:,1),fcfs(:,2)./fcfs(:,3),'.r')
xlim([0,track_count])
ylim([y_min, y_max])
title('FCFS')
xlabel('Track number')
ylabel('Average seek time')
subplot(2,2,4)
plot (fcfs(:,1),fcfs(:,3),'.g')
xlim([0,track_count])
title('IO on track')
xlabel('Track number')
ylabel('IO count')