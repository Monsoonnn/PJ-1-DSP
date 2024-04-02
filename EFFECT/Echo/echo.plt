# Define original signal with exponential decay and noise
original(x) = exp(-0.1 * x) + rand(0)

# Echo effect parameters
delay = 10
echo_factor = 0.5

# Define echoed signal with delay and scaling
echoed(x) = original(x) + echo_factor * original(x - delay)

# Plot original and echoed signals
plot original(x) title "Original Signal" with lines, \
     echoed(x) title "Echoed Signal" with lines

#dung cai nay cho nhanh