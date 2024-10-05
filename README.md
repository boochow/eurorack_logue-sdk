# eurorack_logue-sdk
This is the logue-sdk port of Braids, the Mutable Instrument's macro oscillator.

Due to a file size limitation of 32Kb for the oscillator module, wavetable-based oscillators were omitted, and the set of macro ocillators are divided into five different unit files:

- **VA**:
  Analog oscillators, including a ring modulator and saw swarm.

- **DG**:
  Various digital-filtered waveforms.

- **FM**:
  Includes voice/formant synthesis, additive synthesis, and FM synthesis.

- **RS**:
  Resonator-based synthesis, featureing pluck, string, wind, bell, and percussion sounds.

- **NZ**:
  Noise generators.

## Features

All oscillators share the following parameters:

- **Timbre**(Shape): Modifies the waveform.
- **Color**(Shift-Shape): Also modifies the waveform, but in a different way.

  
- **Synth Model**:	Selects one of the sound synthesis models shown in the next section.
- **Mod Target**:	Sets the LFO shape modulation target from timbre, color, or none.
- **Mod Delay**:	Sets the delay time between a note-on and the LFO shape modulation.
- **Pitch Offset**:	Used for fine-tuning of the oscillator's pitch.
- **Bit Depth**:	Selects the quantization bits from options of 2, 3, 4, 6, 8, 12, or 16 bits.
- **Sample Rate**:	Chooses the sampling rate from 4, 6, 8, 16, 24, or 48KHz.
