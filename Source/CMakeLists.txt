set (BaseTargetName ${PROJECT_NAME})

# Set VST3_COPY_DIR to directory if you want result to be copied
set (Copy_Plugin "FALSE")
if ( VST3_COPY_DIR )
  set(Copy_Plugin "TRUE")
  message("VST3_COPY_DIR=${VST3_COPY_DIR}")
endif()

juce_add_plugin("${BaseTargetName}"
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "GranatGuitar"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD "${Copy_Plugin}"   # Set to TRUE if built runtime should be copied to VST3_COPY_DIR
        VST3_COPY_DIR "${VST3_COPY_DIR}"
        PLUGIN_MANUFACTURER_CODE GgFx
        PLUGIN_CODE GgOd
        FORMATS AU VST3 Standalone
        VST3_CATEGORIES
        PRODUCT_NAME "Simple Speaker Simulator")

juce_generate_juce_header(${BaseTargetName})

target_sources(${BaseTargetName} PRIVATE
		Analyser.cpp
		Analyser.cpp
		EqComponent.cpp
		EqComponent.h
		IRComponent.cpp
		IRComponent.h
		LookAndFeelHelp.h
        PluginProcessor.cpp
        PluginProcessor.h
        PluginEditor.cpp
        PluginEditor.h)

target_compile_definitions(${BaseTargetName} PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${BaseTargetName} PRIVATE 
		StaticIR
        juce::juce_audio_utils
        juce::juce_dsp
		juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)