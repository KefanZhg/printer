filename = 'results/ring_tower_acc1_gyroid/speed500/adxl345_data_2024-3-21-16-17-1.txt';
data = readmatrix(filename);

index = data(:,1);
raw_acc = data(:, 2:end);
% raw_acc = raw_acc - avg;
ti = 3;

g = 9.81;
acc_m_s2 = raw_acc / 256 * g;

fs = 400;
t = index / fs;
t = t - t(1);
fc = 50;

% Plot
figure
subplot(3,2,1)
plot(t, acc_m_s2(:,1),"LineWidth",1.4)
title('X-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

subplot(3,2,3)
plot(t, acc_m_s2(:,2),"LineWidth",1.4)
title('Y-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

subplot(3,2,5)
plot(t, acc_m_s2(:,3),"LineWidth",1.4)
title('Z-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

% Power spectral density
% figure
for i = 1:3
    subplot(3,2,i*2)
    [pxx, f] = pwelch(acc_m_s2(:,i), [], [], [], fs);

    plot(f, 10*log10(pxx),"LineWidth",1.4)
    title(['PSD of axis ', num2str(i)])
    xlabel('Frequency (Hz)')
    ylabel('Power/Frequency (dB/Hz)')
    grid on
    xlim([0, 30])
    ylim([-40,3])
end

sgtitle("Speed = 500%, Raw")


% Low pass filter
[b, a] = butter(2, fc/(fs/2));
acc_m_s2_lp = filtfilt(b, a, acc_m_s2);

% Plot
figure
subplot(3,2,1)
plot(t, acc_m_s2_lp(:,1),"LineWidth",1.4)
title('X-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

subplot(3,2,3)
plot(t, acc_m_s2_lp(:,2),"LineWidth",1.4)
title('Y-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

subplot(3,2,5)
plot(t, acc_m_s2_lp(:,3),"LineWidth",1.4)
title('Z-axis')
xlabel('Time (s)')
ylabel('Acceleration (m/s^2)')
xlim([min(t), min(t)+ti])
grid on

% Power spectral density
% figure
for i = 1:3
    subplot(3,2,i*2)
    [pxx, f] = pwelch(acc_m_s2_lp(:,i), [], [], [], fs);

    plot(f, 10*log10(pxx),"LineWidth",1.4)
    title(['PSD of axis ', num2str(i)])
    xlabel('Frequency (Hz)')
    ylabel('Power/Frequency (dB/Hz)')
    grid on
    xlim([0, 30])
    ylim([-40,3])
end

sgtitle("Speed = 500%, Low-passed")
