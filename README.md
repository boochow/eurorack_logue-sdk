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

## List of available synthesis models
(names from the source code)

- **VA**
1. CSAW
2. MORPH
3. SAW SQUARE
4. SINE TRIANGLE
5. BUZZ
6. SQUARE SUB
7. SAW SUB
8. SQUARE SYNC
9. SAW SYNC
10. TRIPLE SAW
11. TRIPLE SQUARE
12. TRIPLE TRIANGLE
13. TRIPLE SINE
14. TRIPLE RING MOD
15. SAW SWARM
- **DG**
16. SAW COMB
17. TOY
18. DIGITAL FILTER LP
19. DIGITAL FILTER PK
20. DIGITAL FILTER BP
21. DIGITAL FILTER HP
- **FM**
22. VOSIM
23. VOWEL
24. VOWEL FOF
25. HARMONICS
26. FM
27. FEEDBACK FM
28. CHAOTIC FEEDBACK FM
- **RS**
29. PLUCKED
30. BOWED
31. BLOWN
32. FLUTED
33. STRUCK BELL
34. STRUCK DRUM
35. KICK
36. CYMBAL
37. SNARE
- **NZ**
42. FILTERED NOISE
43. TWIN PEAKS NOISE
44. CLOCKED NOISE
45. GRANULAR CLOUD
46. PARTICLE NOISE
47. DIGITAL MODULATION
