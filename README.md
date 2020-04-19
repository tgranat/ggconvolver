# ggconvolver
Simple VST3 impulse response plugin with a few hard coded impulse responses. It uses the JUCE convolution engine https://docs.juce.com/master/classdsp_1_1Convolution.html

The IR wav files are added to the JUCE project as binary resources. JUCE automatically converts it to binary data that can be used instead of reading the files in runtime (since I want the IRs to be hard coded).

The impulse responses are either recorded by me or found on the internet and given away
for free without any reservations (at least I couldn't find any). The reason for me to 
hard code the IRs (other than to learn plugin development with JUCE) is that I already
have several plugins where I can load IR files. I wanted a simple speaker plugin that
suits me and works out of the box.

I have only tested the plugin on Reaper.

v0.1:
* Basic convolving with hardcoded impulse responses.
* Equalizer with low and high shelf filter and mid filter with variable frequency and bandwidth.
* Parameters are stored so they are saved when the DAW project is saved.

This version under development:
* Fancy graph plotting the EQ curve to visualize the EQ settings

Todo:

* Would like some VU meters (like I have in the https://github.com/tgranat/vstplugin_dev/tree/master/ggconvolver that I made using the VST SDK)
* Add more bandwidth options. Will probably use a dropdown instead of radio buttons
* Plot the frequency response for the loaded IR
* In the future: load IR from file (but keep hardcoded options)