import argparse
import warnings
from scipy.io import wavfile
import numpy as np
from scipy.signal import hann
from scipy.stats import gmean
#uncomment this if you want to add the 4th parameter
#import parselmouth

# Function to extract pitch (fundamental frequency) from an audio file
#uncomment it if you want to add the 4th parameter
'''def extract_pitch(audio_file):
    snd = parselmouth.Sound(audio_file)
    pitch = snd.to_pitch()
    return pitch'''

def decision(peaks, greatest, average, pitch=0, weights=[1,1,1,0]):
    pitch_threshold = 80
    sum_peaks = peaks[0] + peaks[1]
    peaks[0] /= sum_peaks
    peaks[1] /= sum_peaks
    greatest[0] = np.log10(greatest[0])
    greatest[1] = np.log10(greatest[1])
    sum_average = average[0] + average[1]
    average[0] /= sum_average
    average[1] /= sum_average
    male_score = 0
    female_score = 0
    if(peaks[0] > peaks[1]):
        male_score += 1*weights[0]
    else:
        female_score += 1*weights[0]
    if(greatest[0] > greatest[1]):
        male_score += 1*weights[1]
    else:
        female_score +=1*weights[1]
    if(average[0] > average[1]):
        male_score += 1*weights[2]
    else:
        female_score += 1*weights[2]
    if(pitch < pitch_threshold):
        male_score+= 1*weights[3]
    else:
        female_score+= 1*weights[3]
    if(male_score > female_score):
        return 0
    else:
        return 1

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Audio gender classification')
    parser.add_argument('file_path', help='Path to the WAV file')
    args = parser.parse_args()
    file_path = args.file_path

    # Read the WAV file
    # Suppress all warnings temporarily
    with warnings.catch_warnings():
        warnings.simplefilter("ignore")
        sampling_rate, signal_raw = wavfile.read(file_path)
        #print(f'Sampling rate: {sampling_rate} Hz')

    # Check if the signal is mono or stereo
    num_channels = signal_raw.ndim
    if num_channels == 1:
        #print("Audio is MONO")
        signal = signal_raw
    elif num_channels > 1:
        #print("Audio is STEREO")
        # Convert stereo to mono by averaging the channels
        signal = np.mean(signal_raw, axis=1)  # Taking the mean across column

    #uncomment this if you want to add the 4th parameter
    '''pitch = extract_pitch(file_path)
    pitch_values = pitch.selected_array['frequency']
    mean_pitch = sum(pitch_values) / len(pitch_values)'''

    # Window parameters
    frame_size = min(int(0.55 * sampling_rate), len(signal))  # Using 550ms window size
    overlap = int(0.10 * sampling_rate)  # Using 100ms overlap
    downsampling_factor = 5
    threshold = 10

    # Define frequency ranges for male and female voices
    male_range = (55, 160)
    female_range = (170, 275)

    # Calculate number of frames
    num_samples = len(signal)
    num_frames = int(np.ceil((num_samples - frame_size) / overlap))

    male = 0
    female = 0
    max_one_male = 0
    max_one_female = 0
    avg_sup_male = 0
    avg_sup_female = 0

    # Apply Fourier Transform on each frame
    start = 0
    while start + frame_size < num_samples:
        # Extract the frame
        end = start + frame_size
        frame = signal[start:end]

        # Apply Hanning window to the frame
        window = hann(frame_size)
        windowed_frame = frame * window

        # Perform FFT on the windowed frame
        fft_result = np.fft.fft(windowed_frame)
        fft_result = np.abs(fft_result) / len(windowed_frame)  # Normalizing the FFT result

        # Generating frequency axis for plotting
        freqs = np.fft.fftfreq(len(windowed_frame), d=1/sampling_rate)
        positive_freqs = freqs[freqs >= 0]
        positive_signal = fft_result[freqs >= 0]

        # Initialize a copy of the original spectrum
        combined_spectrum = np.copy(fft_result)

        # Iterate over downsampling factors and multiply spectrums
        for factor in range(2, downsampling_factor):
            downsampled_spectrum = np.copy(fft_result[::factor])  # Downsampling the spectrum
            downsampled_spectrum = np.pad(downsampled_spectrum, (0, len(combined_spectrum) - len(downsampled_spectrum)), 'constant')  # Pad the array to match size
            combined_spectrum *= downsampled_spectrum  # Element-wise multiplication

        # Find the maximum frequency index and value in the combined spectrum
        combined_spectrum[0]=0
        max_freq_index = np.argmax(combined_spectrum)
        max_freq = positive_freqs[max_freq_index] if combined_spectrum[max_freq_index] > threshold else None

        # Find the indices within the male and female ranges
        male_indices = np.where((positive_freqs >= male_range[0]) & (positive_freqs <= male_range[1]))[0]
        female_indices = np.where((positive_freqs >= female_range[0]) & (positive_freqs <= female_range[1]))[0]

        # Find the maximum frequencies in the male and female ranges
        max_freq_male = positive_freqs[male_indices[np.argmax(combined_spectrum[male_indices])]]
        max_freq_female = positive_freqs[female_indices[np.argmax(combined_spectrum[female_indices])]]
        
        # Find the values corresponding to the maximum frequencies in the male and female ranges
        max_value_male = combined_spectrum[male_indices][np.argmax(combined_spectrum[male_indices])]
        max_value_female = combined_spectrum[female_indices][np.argmax(combined_spectrum[female_indices])]
        max_one_male = max(max_one_male, max_value_male)
        max_one_female = max(max_one_female, max_value_female)

        gender = -1
        # Count occurrences above and below frequency thresholds
        
        if max(max_value_male, max_value_female) > threshold:
            gender = 9
            if max_value_male > max_value_female:
                male += 1
            elif max_value_male < max_value_female:
                female += 1
            #Compute average amplitude for both ranges
            male_freq_avg = gmean(combined_spectrum[(freqs >= male_range[0]) & (freqs <= male_range[1])])
            female_freq_avg = gmean(combined_spectrum[(freqs >= female_range[0]) & (freqs <= female_range[1])])
            if male_freq_avg > female_freq_avg:
                avg_sup_male += 1
            else:
                avg_sup_female += 1
            
        start += overlap

    # Display stats
    '''print(f"Male frequencies ({male_range[0]} Hz - {male_range[1]} Hz): {male} times")
    print(f"Female frequencies ({female_range[0]} Hz - {female_range[1]} Hz): {female} times")
    print(f"Max one time male: {max_one_male}")
    print(f"Max one time female: {max_one_female}")
    print(f"Avg supports male: {avg_sup_male}")
    print(f"Avg suppots female: {avg_sup_female}")'''
    ##uncomment this if you want to see the 4th parameter
    #print(f"Mean pitch: {mean_pitch}")

    #Return the answer
    assigned_gender = decision([male, female], [max_one_male, max_one_female], [avg_sup_male, avg_sup_female])
    if(assigned_gender==0):
        print('M')
    else:
        print('K')
