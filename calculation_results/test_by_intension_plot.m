results = load('test_by_intension');
plot(results(:,1),results(:,2))
hold on;
plot(results(:,1),results(:,6))
plot(results(:,1),results(:,10))
hold off;
plot(results(1:23,1),results(1:23,2))
hold on;
plot(results(1:23,1),results(1:23,6))
plot(results(1:23,1),results(1:23,10))
hold off;