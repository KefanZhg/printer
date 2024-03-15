filename = 'data/adxl345_data_2024-3-13-18-40-13.txt';
data = readmatrix(filename);
filename = 'data/adxl345_data_2024-3-13-18-40-13.txt';
benchmark = readmatrix(filename);
avg = mean(benchmark(:,2:end));

index = data(:,1);
raw_acc = data(:, 2:end);
% raw_acc = raw_acc - avg;
ti = 10;

g = 9.81;
acc_m_s2 = raw_acc / 256 * g;

fs = 400;
t = index / fs;
fc = 50;

% Plot
figure
subplot(3,1,1)
plot(t, acc_m_s2(:,1),"LineWidth",1.4)
title('X-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

subplot(3,1,2)
plot(t, acc_m_s2(:,2),"LineWidth",1.4)
title('Y-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

subplot(3,1,3)
plot(t, acc_m_s2(:,3),"LineWidth",1.4)
title('Z-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

% Power spectral density
figure
for i = 1:3
    [pxx, f] = pwelch(acc_m_s2(:,i), [], [], [], fs);
    subplot(3,1,i)
    plot(f, 10*log10(pxx),"LineWidth",1.4)
    title(['PSD of axis ', num2str(i)])
    xlabel('Frequency (Hz)')
    ylabel('Power/Frequency (dB/Hz)')
    grid on
    xlim([0, 30])
end


% Low pass filter
[b, a] = butter(2, fc/(fs/2));
acc_m_s2_lp = filtfilt(b, a, acc_m_s2);

% Plot
figure
subplot(3,1,1)
plot(t, acc_m_s2_lp(:,1),"LineWidth",1.4)
title('X-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

subplot(3,1,2)
plot(t, acc_m_s2_lp(:,2),"LineWidth",1.4)
title('Y-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

subplot(3,1,3)
plot(t, acc_m_s2_lp(:,3),"LineWidth",1.4)
title('Z-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

% Power spectral density
figure
for i = 1:3
    [pxx, f] = pwelch(acc_m_s2_lp(:,i), [], [], [], fs);
    subplot(3,1,i)
    plot(f, 10*log10(pxx),"LineWidth",1.4)
    title(['PSD of axis ', num2str(i)])
    xlabel('Frequency (Hz)')
    ylabel('Power/Frequency (dB/Hz)')
    grid on
    xlim([0, 30])
end
