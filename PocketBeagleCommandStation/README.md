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

