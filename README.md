# ggconvolver
Simple VST3 impulse response plugin with a few hard coded impulse responses. It uses the JUCE convolution engine https://docs.juce.com/master/classdsp_1_1Convolution.html

The IR wav files are added to the JUCE project as binary resources. JUCE automatically converts it to binary data that can be used instead of reading the files in runtime (since I want the IRs to be hard coded).

Work in progress:
The output level is low, need to figure out how to handle that in a good way.
Add some eq controls.
Improve the GUI, make my own editor (now I'm using JUCE GenericAudioProcessorEditor)
Would like some VU meters (like I have in the https://github.com/tgranat/vstplugin_dev/tree/master/ggconvolver that I made using the VST SDK)