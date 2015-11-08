results = load('test_by_intension.bin');
x = results(1:23,1);
figure
plot(x,results(1:23,2),'*r',x,results(1:23,6),'.k',x,results(1:23,10),'ob')
legend('FCFS','SSF','Elevator algorithm','Location','northwest')
title('Average seek time for various algorithms')
xlim([results(1,1)-10 results(23,1)+10])
xlabel('Poisson intension')
ylabel('Average seek time')