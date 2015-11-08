results = load('test_convergence.bin');
figure
plot(results(:,3),results(:,2),'.r',results(:,5),results(:,4),'.k',results(:,7),results(:,6),'.b')
legend('FCFS','SSF','Elevator algorithm','Location','southeast')
title('Convergence for various algorithms')
xlim([results(1,1) results((length(results(:,1)) + 1)/2,1)])
%xlim([results(1,1) results(length(results(:,1)),1)])
xlabel('IO count')
ylabel('Average seek time')