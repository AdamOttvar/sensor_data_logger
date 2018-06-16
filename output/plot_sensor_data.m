filename = 'sensor_data.txt';
delimiterIn = ',';
headerlinesIn = 1;
A = importdata(filename,delimiterIn,headerlinesIn);
time = A.data(:,1);
%%
figure
hold on
plot(time,A.data(:,2))
plot(time,A.data(:,3))
plot(time,A.data(:,4))

title('Accelerometer data')
xlabel('Time [ms]')
ylabel('Acc [g]')
legend(A.colheaders{2},A.colheaders{3},A.colheaders{4})

%%
figure
hold on
plot(time,A.data(:,5))
plot(time,A.data(:,6))
plot(time,A.data(:,7))

title('Gyro data')
xlabel('Time [ms]')
ylabel('Gyro [deg/s]')
legend(A.colheaders{5},A.colheaders{6},A.colheaders{7})

%%
figure
hold on
plot(time,A.data(:,8))
plot(time,A.data(:,9))
plot(time,A.data(:,10))

title('Magnetometer data')
xlabel('Time [ms]')
ylabel('Mag [Gauss]')
legend(A.colheaders{8},A.colheaders{9},A.colheaders{10})
