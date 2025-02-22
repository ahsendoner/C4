# Software-Defined Radio: Local Oscillator and User/Computer Interface

## Overview

This project focuses on a crucial subsystem of the SDR (Software-Defined Radio) which includes Local Oscillator and User/Computer Interface components of the Flexible Radio Transceiver (FLRTRX). This subsystem is responsible for generating the local oscillator (LO) signals required for both the transmitter (TX) and receiver (RX) chains of the radio. Additionally, it provides the interface for user and computer control, allowing the radio to be operated either manually via a front-panel interface or remotely via a computer using a USB connection.

The subsystem is divided into two main parts:
1. **Microcontroller-Controlled Oscillator**: Generates the LO signals and manages the TX/RX switch.
2. **TX/RX Switch**: Controls the flow of signals between the antenna and the TX/RX chains.

## Key Functions

### 1. **Local Oscillator (LO) Signal Generation**
   - The LO generates two quadrature signals (90° out of phase) at a frequency \( f_{LO} \), which are used by the quadrature mixers in both the TX and RX chains.
   - The frequency \( f_{LO} \) can be controlled either manually via a user interface (UI) or programmatically via a computer using the CAT (Computer-Aided Transceiver) protocol over a USB connection.
   - The LO signals are generated with a frequency accuracy of within 1000 Hz and are output as 3.3 Vpp unipolar signals.

### 2. **TX/RX Switch Control**
   - The TX/RX switch determines whether the radio is in transmit or receive mode.
   - The switch can be controlled either by the user interface or by the computer via the USB connection.
   - When in receive mode, the antenna is connected to the RX chain, and when in transmit mode, the antenna is connected to the TX chain.

### 3. **User and Computer Interface**
   - The subsystem supports a front-panel user interface (UI) that allows the user to manually select the LO frequency and switch between TX and RX modes.
   - The computer interface allows for remote control of the radio using the CAT protocol. The computer can set the LO frequency, switch between TX and RX modes, and query the current state of the radio.

## Electrical Interface

### Microcontroller-Controlled Oscillator
- **Output Signals**:
  - `LO_F1_0` and `LO_F1_90`: Quadrature oscillator signals, 90° apart, 3.3 Vpp unipolar at frequency \( f_{LO} \).
  - `/TXEN`: Active-low transmit-enable signal (3.3 V LVTTL), indicating whether the radio is in receive mode (high) or transmit mode (low).
- **Power Signals**: +5V and +3.3V.

### TX/RX Switch
- **Bidirectional Signal**:
  - `ANT`: Signal to/from the antenna.
- **Input Signals**:
  - `PA_OUT`: Signal from the power amplifier (TX chain).
  - `/TXEN`: Active-low transmit-enable signal (3.3 V LVTTL).
- **Output Signal**:
  - `RX_STG`: Signal to the receiver (RX chain).
- **Power Signal**: +5V.

## Mechanical Specifications

### Subsystem C.1 (Microcontroller-Controlled Oscillator)
- **Board Size**: 1500 mil × 1500 mil (38.1 mm × 38.1 mm).
- **Connectors**:
  - `J9` and `J10`: 4-pin Molex connectors for interfacing with the mainboard.
  - Additional connectors for programming the microcontroller, connecting the PLL module, and serial interface with a PC via a USB-serial module.

### Subsystem C.2 (TX/RX Switch)
- **Board Size**: 1500 mil × 1500 mil (38.1 mm × 38.1 mm).
- **Connectors**:
  - `J11`, `J12`, and `J13`: 4-pin Molex connectors for interfacing with the mainboard.

## Power Architecture

- The mainboard provides regulated power to all subsystems.
- Subsystem C requires +5V and +3.3V power rails.
- The mainboard regulates the input DC voltage (12-18 VDC) to the required levels.

## CAT Protocol for Computer Control

The CAT protocol is used to control the FLRTRX via a computer. The following key commands are implemented:

### Key CAT Commands
- **TX**: Set or query the transmit/receive state.
  - `TX1;`: Set to transmit mode.
  - `TX0;`: Set to receive mode.
  - `TX;`: Query the current state.
- **FA**: Set or query the LO frequency.
  - `FA000010000;`: Set LO frequency to 10 kHz.
  - `FA;`: Query the current LO frequency.
- **FB**: Set or query the second LO frequency (not used in this implementation but must respond to queries).
- **IF**: Query the system status.
  - `IF;`: Returns the current LO frequency and other status information.

### Example CAT Exchange
```
>TX;
TX0;
>TX1;
>TX;
TX1;
```

In this example, the computer queries the TX/RX state, sets the radio to transmit mode, and then confirms the state change.

## Additional CAT Commands (Optional)
- **AI**: Similar to TX but does not change the state.
- **ST**: Behaves similarly to AI.
- **ID**: Returns the string `ID0650;`.
- **MDO**: Returns the string `MDOC;`.
- **SHO**: Returns the string `SH0000;`.
- **NAO**: Returns the string `NA00;`.
- **IF**: Returns a 28-character string with the current LO frequency.

## References

1. [CAT Operation Reference Manual, Yaesu](https://www.yaesu.com/downloadFile.cfm?FileID=13370&FileCatID=158&FileName=FT%2D991A%5FCAT%5FOM%5FENG%5F1711%2DD.pdf&FileContentType=application%2Fpdf#page=1&zoom=auto,-214,61)
2. [Newark: Chip adapter, FTDI friend](https://canada.newark.com/adafruit/284/adapter-ftdi-ft232rl-chip/dp/53W5825)
3. [element14: ATmega324P-20PU](https://my.element14.com/microchip/atmega324p-20pu/mcu-8bit-atmega-20mhz-dip-40/dp/1455110)

## Conclusion

Subsystem C is a vital part of the FLRTRX, providing the necessary LO signals and control interfaces for both manual and computer-controlled operation. By following the specifications outlined in this document, the subsystem can be successfully implemented and integrated into the overall radio system.
