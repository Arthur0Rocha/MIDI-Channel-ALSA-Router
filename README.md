# MIDI-Channel-ALSA-Router


## TODO: 

1 - Rename project to pisynth?
2 - Workflow:
    2.1 - Start and config Jack
    2.2 - Start Fluidsynth and run config script (load all soundfonts at the same time?)
    2.3 - Make MIDI connections and Audio connections (watch and reconnect?)
    2.4 - Run main loop routine
3 - Main routine would consist of:
    3.1 - MIDI channel redirecting (seamlessly for user)
    3.2 - Minimize prog change commands (make use of same banks)
    3.3 - Watching for hanging notes and applying noteoffs to previous channels
