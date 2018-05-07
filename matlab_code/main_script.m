%% Description
% this code was run in Matlab 2012 ver.

%% Initialize
clc;
clear all;

%% Parameters 
A = 2;                  % check value from arduino

waveFile = 'test.wav';  % sound file
fs = 44100;             % sampling rate
duration = 2;		    % recording time
nbits = 16;             % using 16 bits
ard_1st = 'COM3';
ard_2st = 'COM5';
ard_baud_rate = 9600;

%% 
while 1
    arduino = serial(ard_1st, 'BaudRate', ard_baud_rate);
    fopen(arduino);
    A = fscanf(arduino, '%s');                    % read from 1st arduino
    fclose(arduino);
    
    if (A == '0')                                 % run when arduino sends 0
        fprintf('Recording...\n');
        y = wavrecord(duration*fs, fs);           % recording 
        fprintf('Finished recording.\n');
%         fprintf('Press any key to save the sound data to %s...\n', waveFile); % pause and save manually
        wavwrite(y, fs, nbits, waveFile);         % save into wavefile in sampling rate fs, nbits
        fprintf('Finished writing %s\n', waveFile);
        
        [file, Fs] = wavread('test.wav');         % read wav (amplitude and sampling rate)
        B = size(file(:,1));              
        N = B(1);                             
        f = -1 * (Fs/2) + (Fs/N):(Fs/N):(Fs/2);   % range of frequency
        f_p = 0:(Fs/N):(Fs/2-Fs/N);               % 1/2 of frequeny
        t = 0:1:B-1;                              % time range
        
        f_file = fft(file);                       % do fft
        f_file = abs(f_file);                       
        f_file_shift = fftshift(f_file);          % domain shift
        f_file_shift_p = zeros((N-1)/2,1);         
        
        for i = 1:1:(N/2)
            f_file_shift_p(i,1) = f_file(i,1);
        end                                       % delete f below 0
        
        subplot(2,1,1)
        plot(t,file)                              
        subplot(2,1,2)
        plot(f_p,f_file_shift_p)
        xlim([0 1500])
        ylim([0 3000])
        a = 2;
        
        for j = 100:1:240                         % in 100 ~ 240Hz
            if (f_file_shift_p((N/Fs)*j,1) >= 1500)     % check if the amplitude over 1500
                a = 1;                            % estimate the garbage as plastic
                break;
            end            
        end
        
        for j = 500:1:1200                        % in 500~1200Hz
            if (f_file_shift_p((N/Fs)*(j),1) >= 1000)   % check if the amplitude over 1500
                a = 1;                            % estimate the garbage as plastic
                break;
            end
        end
        
        arduino2 = serial(ard_2nd, 'BaudRate', ard_baud_rate);
        fopen(arduino2);
        for k=1:1:400000000                 % wait few seconds
        end                             
        fprintf(arduino2,'%s\n',a);         % send to 2nd arduino
        fclose(arduino2);
    end
end