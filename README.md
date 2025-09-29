# What is AudioPlatform?

![](/Documentation/Images/AudioPlatformPhotoImage.png)

### _A Canvas for Sound_
**AudioPlatform _for Teensy 4.1_** is a complete, fully-assembled and self-contained unit supporting the development of a wide range of professional-quality audio systems and devices, from simple to sophisticated.
Based on PJRC’s Teensy® 4.1 Development Board and the associated TeensyDuino / Arduino tools and libraries, AudioPlatform features a complete inventory of the hardware components necessary for Teensy-based audio projects, as well as additional components providing extended memory and I/O resources.

AudioPlatform consists of two primary elements:
- A compact and portable hardware platform for experimentation and development of stand-alone audio applications.
- A highly-structured, adaptable, and well-documented software application, **_apApp_**, utilizing the resources of the hardware platform to demonstrate the power and flexibility of audio processing within the Teensy environment.
    
### _A Community Effort_
Since the introduction in 2014 of the Teensy Audio Library, there have been numerous additions to that code base by a large open-source community of highly-motivated members whose ideas for engaging audio projects have resulted in a considerable body of impressive and useful software tools. PJRC and the Teensy community have shown that it is not only possible to produce and process high-definition audio, but that the present-day capabilities to do so are at a level on par with audio systems that until recently were far beyond the reach of inexpensive, user-friendly devices. Without this solid base to build upon, AudioPlatform would not be possible. 
### _A Gap Bridged_
But despite the potential of Teensy audio, only a small number of complete hardware devices built upon these tools have emerged. And while many of these efforts are quite impressive, they generally are centered around a specific function or task – for example a synthesizer or MIDI controller – with hardware and software unique to the device, rather than a general purpose platfrom that can play host to a variety of audio system designs. AudioPlatform aims to address that deficit, embodying an open and flexible hardware design–usable out-of-the box, no soldering or wiring required–that offers a significant base of resources for audio applications of all sorts.

## AudioPlatform Hardware

The components of the hardware are based on both the elements of the original PJRC Teensy Audio Shield and additional components that fully extend the storage and IO capacity available for Teensy audio applications:
- Black Acrylic-Matte Housing and Base
- 800x480 Pixel RGB LCD Capacitive Touch Screen
- Teensy 4.1 @600 MHz with 1 MB RAM
- SGTL5000 Audio Codec
- 8 MB Non-Volatile Program Flash
- 16 MB PSRAM
- 128 MB Non-Volatile Serial Flash Storage
- Teensy 4.1 microSD Card I/O
- USB I/O on both MIDI Host and USB-C Connections
- MIDI I/O on both USB and MIDI DIN Ports
- Audio Line In/Out on 3.5 mm Female Connectors
- Audio In/Out over USB
- Headphone Out on 3.5mm Female Connector
- 4 Rotary Encoders for Menu and Device Parameter Settings
- Dimensions: 8.7” Width x 6.2” Height x 2.3” Depth (including encoders)
- Weight: 1 lb 12 oz

## AudioPlatform Software Application

_apApp_, the open-source AudioPlatform software application, is installed and shipped with the AudioPlatform hardware. Application features include:

- Five Multi-Voice Sound Players: Dexed FM Synthesizer, SD WAV File Player, Serial Flash RAW File Player, Analog-Style Synthesizer, Stereo Audio Input
- SD WAV File Player Plays Stereo WAV Soundfiles Stored on microSD Card, with 8-Voice PolyPhony
- Serial Flash RAW Player Plays Mono RAW (headerless) Soundfiles Stored in Internal Non-Volatile Flash Storage, with 8-Voice Polyphony
- Analog-Style Synthesizer Plays Sounds Programmed By Users, with 12-Voice Polyphony
- FM Synthesizer Features 512 Voice Presets in 16 Banks of 32 Voices, Including All Voice Banks of the Original DX7, On Which Dexed Is Based; 10 Banks Stored in Non-Volatile Program Memory; 6 User Banks Loadable from SD Card at Startup
- Stereo Reverb and Stereo Ping-Pong Delay
- All Players and Effects Simultaneously Active or Inactive Based on Preset Settings (no Mutually-Exclusive Devices)
- Player and Effects Parameters Stored in Recallable Preset Banks, 32 Presets per Bank, Unlimited Bank Storage on microSD Card with Instant Recall
- Flexible System Architecture with Individual Components (Players/Effects, Other Elements) Modifiable and/or Replaceable
- Extensive Display and System Support with C++ Libraries, Many of Which Are Application- Independent
- TouchScreen and Encoder Support for Screen Selection, Editing and Selection of Player, Effects, and System Parameters
- Real-Time Clock Support
- Real-Time Display of Audio Levels, CPU and Memory Usage
- User Utilities for File Viewing and Management

> [!NOTE]
> Of particular note is the fact that use of the _apApp_ software application is not a requirement for user designs. Developers who wish to adapt the hardware elements—and, optionally, elements of the open-source software—in their own  designs and projects will find AudioPlatform's resources to be fully supportive of their efforts, freeing them to proceed without the need to acquire or construct additional circuit components.

## Hardware and Application Documentation

Detailed documentation for AudioPlatform is available in the Documentation folder of this repository. Included are the following PDF documents:

- Volume 1: _Introduction_ A basic overview of AudioPlatform’s hardware and software features, including discussions of its design motivations and development process.
- Volume 2: _Getting Started Guide_ An initial guide for users of AudioPlatform – what's in the box, setup and IO connections, application screens, menus and basic navigation – with an emphasis on exploring its capabilities via numerous demonstration sounds and presets included in the installed _apApp_ software application.
- Volume 3: _apApp User Guide_ Detailed descriptions of all aspects of the _apApp_ application – screens, navigation, players, effects, system components, device settings, presets, system utilities and other operations.
- Volume 4: _Hardware Reference_ A survey of the AudioPlatform hardware – PCB layout, components, connections, input and output – intended for programmers and designers who wish to understand details AudioPlatform’s design and layout.
- Volume 5: _apApp Application Reference_ An exhaustive reference of _apApp_ software and architecture – intended for programmers who want to understand details of the application's design and operations, and who may wish to extend, modify, or utilize components of the application in other projects.

## Availability

AudioPlatform hardware and accessories are available at [ProtoSupplies.com](https://www.protosupplies.com/). The system comes pre-installed with _apApp_ and includes a microSD card with numerous banks of presets and audio samples. The complete source code and documentation of _apApp_ are included in this Github repository.
