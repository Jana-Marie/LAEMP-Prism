# LAEMP-Obelisk

The LÄMP-Obelisk is an aesthetically pleasing and colorful effect lamp that can be used in your home. It can be controlled using Zigbee (e.g., Philips Hue) and has RGBY + WW/CW LEDs, providing a wide dynamic range and fantastic color representation. The lamp can be soldered together at home or assembled by JLCPCB.

The project consists of nine PCBs that are cleverly interlocked with pinheaders and capacitors. The obelisk-like shape, combined with its 291 LEDs, projects a slightly changing dotted pattern into its environment.

<table>
  <tbody>
    <tr>
      <td>
        <img src="images/laemp_9.jpg"/>
      </td>
      <td>
        <img src="images/laemp_4.jpg"/>
      </td>
    </tr>
  </tbody>
</table>

This project is an attempt to add some colored lights and interesting patterns to ones home. I hope you enjoy it! :3

<table>
  <tbody>
    <tr>
      <td>
        <img src="images/laemp_2.jpg"/>
      </td>
      <td>
        <img src="images/laemp_1.jpg"/>
      </td>
    </tr>
  </tbody>
</table>

### V2

Please note that the E75-2G4M20S module is End of Life and is hard to obtain. I'm working on a V2.0 with STM32WB (see [MiniBee](https://github.com/Jana-Marie/MiniBee)). Hence the Hex-Base-Board is subject to be updated, all other boards shouldnt change.

## Concept

The overall concept was developed in OpenSCAD and a pseudo-random pattern of circles was choosen for the walls. If one wants to put effort into this project, different walls can be used. The standard base-to-wall-interface also allows for much different and custom walls. E.g. hexagonal holes, shorter or taller walls, bended walls, ...

![full open](/images/full_open.png)

## Build

This build is definitely time-consuming, as each LAEMP Obelisk features 291 LEDs, and therefore requires at least 291 100nF capacitors. Additionally, the wall PCBs are rather thin and long, making reflow difficult and assembly time-consuming.

Once all eight boards are assembled (the top board has no components ^⁻^), they can be interlocked with one another. To do this, begin by plugging the LED-Base-Board into the Base-Board, and then carefully plugging wall after wall into the baseboard. Try to put the Top-Board onto the build after assembling five boards. The board with the Type-C cutout should not be the last board to assemble. Finally, try to plug in the last board. This step can be quite tricky and may require a few attempts. Once all boards are assembled, align all walls with each other and place the whole board onto the 3D printed base.

<table>
  <tbody>
    <tr>
      <td>
        <img src="images/laemp_8.jpg"/>
      </td>
      <td>
        <img src="images/laemp_7.jpg"/>
      </td>
    </tr>
    <tr>
      <td>
        <img src="images/laemp_6.jpg"/>
      </td>
      <td>
        <img src="images/laemp_5.jpg"/>
      </td>
    </tr>
  </tbody>
</table>

## Firmware

There are two firmwares to be flashed, one for the ESP32 and one for the E75-2G4M20S.

### ESP32

This is a simple Arduino project and can be flashed via Arduino.

### E75

WIP

## License

[CERN Open Hardware Licence Version 2 - Strongly Reciprocal](/LICENSE)