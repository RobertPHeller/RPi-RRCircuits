# PocketBeagleCommandStation

Through hole version of the PocketBeagle LCC/DCC Command Station. This is a
DCC command station that is a LCC node. It implements the OpenLCB train
protocol node over LCC and converts that to DCC commands. It includes a
booster and puts the DCC signal on the LCC bus (pins 4 and 5). It implements a
programming track and implements Railcom. The Command Station uses a
PocketBeagle as the processing element and uses the PRUs (Programmable
Realtime Units) to generate the DCC signal. (See
[PocketBeagleCommandStation_SMD](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PocketBeagleCommandStation_SMD)
for a SMD version.)

* [Mouser Cart](https://www.mouser.com/ProjectManager/ProjectDetail.aspx?AccessID=514917dd80)
* [Schematic PDF](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/PocketBeagleCommandStation.pdf)
* [Board Rendering](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/PocketBeagleCommandStation.png)
* [Node Software](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/BBBCommandStationOpenMRN)
* [Kit Booklet](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/KitBooklet.pdf)
* Heat sink files:
  * [Main panel](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/HeatSinkMainPanel.pdf)
  * [Fin 1](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/HeatSinkFin1Panel.pdf)
  * [Fin 2](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/HeatSinkFin2Panel.pdf)
  * [Cut page 1:1 (SVG)](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/HeatSinkLaserCutPage-nocps-cutsonly.svg)
  * [Cut and bend 1:1 (SVG)](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/HeatSinkLaserCutPage-nocps.svg)
  * [Cut and bend 1:1 (SVG, with center points)](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/HeatSinkLaserCutPage.svg)
  * [FreeCAD Python script](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/PocketBeagleCommandStation/HeatSink.py)

