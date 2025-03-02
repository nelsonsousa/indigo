# Guide to indigo_server and INDIGO Drivers
Revision: 12.01.2021 (draft)

Author: **Rumen G.Bogdanovski**

e-mail: *rumenastro@gmail.com*

## The INDIGO server: **indigo_server**
This is the help message of the indigo_server:
```
rumen@sirius:~ $ indigo_server -h
INDIGO server v.2.0-141 built on Jan 11 2021 01:06:15.
usage: indigo_server [-h | --help]
       indigo_server [options] indigo_driver_name indigo_driver_name ...
options:
       --  | --do-not-fork
       -l  | --use-syslog
       -p  | --port port                     (default: 7624)
       -b  | --bonjour name                  (default: hostname)
       -T  | --master-token token            (master token for devce access default: 0 = none)
       -a  | --acl-file file
       -b- | --disable-bonjour
       -u- | --disable-blob-urls
       -w- | --disable-web-apps
       -c- | --disable-control-panel
       -v  | --enable-info
       -vv | --enable-debug
       -vvv| --enable-trace
       -r  | --remote-server host[:port]     (default port: 7624)
       -x  | --enable-blob-proxy
       -i  | --indi-driver driver_executable
rumen@sirius:~ $
```

The **indigo_server** is highly configurable trough command line options at start and also when already started:

### -- | --do-not-fork
By default **indigo_server** will start a child process called **indigo_worker**, and the drivers will be run in the boundaries of this process. This is done for a reason, if a single driver crashes this will crash **indigo_worker** process indigo server will detect it and start **indigo_worker** again. This switch prevents this behavior and if a driver fails the whole server will crash without an attempt to recover. This is is used for faulty driver debugging.

```
indigo_server───indigo_worker─┬─{indigo_worker}
                              ├─{indigo_worker}
                              ├─{indigo_worker}
                              ├─{indigo_worker}
                              └─{indigo_worker}
```

### -l | --use-syslog
Debug and error messages are sent to syslog.

### -p | --port
Set listening port for the indigo_server.

### -b | --bonjour
INDIGO uses mDNS/Bonjour for service discovery. This will set the name of the service in the network, so that the clients can discover and automatically connect to the INDIGO server without providing host and port.

### -T | --master-token
Set the server master token for device access control. Please see [INDIGO_DEVICE_ACCESS_CONTROL_AND_LOCKING.md](https://github.com/indigo-astronomy/indigo/blob/master/indigo_docs/INDIGO_DEVICE_ACCESS_CONTROL_AND_LOCKING.md) for details.

### -a | --acl-file file
Use tokens for device access control from a file. Please see [INDIGO_DEVICE_ACCESS_CONTROL_AND_LOCKING.md](https://github.com/indigo-astronomy/indigo/blob/master/indigo_docs/INDIGO_DEVICE_ACCESS_CONTROL_AND_LOCKING.md) for details.

### -b- | --disable-bonjour
Do not announce the service with Bonjour. The client should enter host and port to connect.

### -u- | --disable-blob-urls
INDIGO provides 2 ways of BLOB (image) transfer. One is the legacy INDI style where the image is base64 encoded and sent to the client in plain text and the client decodes the data on its end. This makes the data volume approx. 30% larger and the encoding and decoding is CPU intensive. The second INDIGO style is to use binary image transfer over HTTP protocol avoiding encoding, decoding and data size overhead. By default the client can request any type of BLOB transfer. With this switch you can force the server to accept only legacy INDI style blob transfer.

### -w- | --disable-web-apps
This switch will disable INDIGO web applications like *Imager*, *Telescope control* etc.

### -c- | --disable-control-panel
This switch will disable the web based control panel.

### -v | --enable-info
Shows some information messages in the log.

### -vv | --enable-debug
Shows more verbose messages. Useful for debugging and troubleshooting.

### -vvv | --enable-trace
Shows a lot of messages, like low level driver-device communication and full INDIGO protocol chatter.

### -r | --remote-server
INDIGO servers can connect to other INDIGO servers and attach their buses to their own bus. This switch is used for providing host names and ports of the remote servers to be attached. This switch can be used multiple times, once per server.

### -x | --enable-blob-proxy
In case -r or --remote-server is used and BLOB URLs are enabled, this server will act as a BLOB proxy. This way all the BLOBs of the remote servers will be accessible through an URL pointing to this server. Otherwise BLOB URLs will point to their servers of origin. This feature is useful in case the remote server is in a network not accessible by the clients of this server. Proxied BLOBs are a bit slower to download compared to the direct download from their server of origin.

### -i | --indi-driver
Run drivers in separate processes. If a driver name is preceded by this switch it will be run in a separate process. This is the way to run INDI drivers in INDIGO. The drawback of this approach is that the driver communication will be in orders of magnitude slower than running the driver in the **indigo_worker** process and those driver can not be dynamically loaded and unloaded. This switch will load the executable version of the driver.

### indigo_driver_name
This is the recommended way to load drivers at startup. Loading a driver without **-i** switch will load the dynamically loadable version of the driver. It will be run in **indigo_worker** process and can be unloaded and loaded any time. Dynamic drivers provide huge performance benefit over executable drivers.

```
rumen@sirius:~ $ indigo_server indigo_ccd_asi -i indigo_ccd_simulator -i indigo_mount_simulator
```
In this case **indigo_ccd_asi** driver will be loaded in the **indigo_worker** process, but **indigo_ccd_simulator** and **indigo_mount_simulator** will have their own processes forked from **indigo_worker** as shown below:
```
indigo_server───indigo_worker─┬─indigo_ccd_simulator
                              ├─indigo_mount_simulator
                              ├─{indigo_worker}
                              ├─{indigo_worker}
                              ├─{indigo_worker}
                              ├─{indigo_worker}
                              ├─{indigo_worker}
                              ├─{indigo_worker}
                              └─{indigo_worker}
```

## INDIGO Drivers
The INDIGO drivers can be used by both **indigo_server** and clients. INDIGO does not require server to operate. Clients can load drivers and use them without the need of a server. In this case only locally attached devices can be accessed (much like ASCOM). Examples how to use INDIGO drivers without a server can be found in [indigo_examples/executable_driver_client.c](https://github.com/indigo-astronomy/indigo/blob/master/indigo_examples/executable_driver_client.c) and [indigo_examples/dynamic_driver_client.c](https://github.com/indigo-astronomy/indigo/blob/master/indigo_examples/dynamic_driver_client.c)

INDIGO applications can also be designed to act as a client and a server at the same time, exposing the locally connected devices to the distributed INDIGO bus.

### Driver Types
There are three versions of each INDIGO driver, compiled from the same source code, each coming with its benefits and drawbacks.

#### Dynamically Loaded Drivers
These drivers are loaded by default by the INDIGO server. They are loaded and run in **indigo_worker** process of **indigo_server**.
- PROS:
	* Extremely fast driver-client and driver-server data transfer.
	* Drivers can be dynamically loaded and unloaded at runtime.
- CONS:
	* A faulty driver can bring down the service or the application, however **indigo_server** can partly recover from this, as mentioned in the previous section.

While **indigo_server** is running the dynamically loaded drivers can be loaded and unloaded by modifying *Server.DRIVERS*, *Server.LOAD* and *Server.UNLOAD* properties as described in [PROPERTY_MANIPULATION.md](https://github.com/indigo-astronomy/indigo/blob/master/indigo_docs/PROPERTY_MANIPULATION.md)

The dynamic drivers use **.so** file extension on Linux and **.dylib** on MacOSX.

#### Static Drivers
These drivers are intended for use in the clients, **indigo_server** can not use them. They can not be dynamically loaded and unloaded. But can be enabled and disabled.
- PROS:
	* Extremely fast driver-client and driver-server data transfer.
	* As drivers are linked together with the application in a monolithic executable, there is no need to distribute the drivers as separate files.
- CONS:
	* Drivers can not be updated without application re linking.
	* A faulty driver can bring down the application.

These drivers have **.a** file extension.

#### Executable Drivers
These are the INDI style drivers, They are run in a separate sub process of **indigo_worker** process. In clients they also run in a separate sub-process. They can be native INDIGO or drivers for INDI.
- PROS:
	* A faulty driver will not bring down the service or the application. Only the attached to the driver devices will stop working. **indigo_server** can partly recover by reloading the driver.
	* This is how INDIGO runs INDI drivers.
- CONS:
	* Driver-client and driver-server data transfer is in orders of magnitude slower.
	* They require more resources to run.
	* No dynamic loading and unloading in **indigo_server**. These drivers can be loaded only at server startup.

These drivers are standard ELF executables and do not have file extension.

### Several Notes on Drivers

#### Optimizing Performance
Static drivers and dynamic drivers provide the best performance. But for ultimate performance client software should directly load drivers, this way the network layer is bypassed and shared memory is used to communicate with the driver. However this will work only with locally attached devices.

For remote devices, the best approach is to use dynamic drivers, loaded by **indigo_server**, accessed over a gigabit network.

#### Optimizing Robustness
There is a huge variety of different astronomical hardware and the developers have no access to all of it. This means that sometimes drivers are developed without access to the physical device and are not well tested. They are just reported to work by users. Sometimes the software development kit used to create the driver is unstable. There are many factors that can affect the driver's stability. This is why, in the driver's README, there is a "Status" section and a list of devices used to test the driver.

In case there is an instability in a particular driver and it needs to be used before it is fixed, it is advised to use the executable version of the driver, to ensure that if the driver crashes, only the devices handled by this particular driver will be affected. Everything else will continue to work without interruption.

It is a good practice to report any instability or crash to the developers providing a trace log from the server.
(TBD: Explain how!!!)

#### Hotplug vs Non Hotplug Drivers

Some devices support hotplug. If so, the chances are that the INDIGO driver will also support hotplug for this device.

Usually USB devices are hotplug devices, but not all of them.
Sometimes only the physical wiring is USB, but the device itself is basically a serial device. They manifest themselves as USB serial ports and there is no way to know what exactly is connected to these serial ports. In this case most likely the device can not be automatically identified by the driver, therefore a proper serial port name should be provided by the user in order to connect the driver to the device. Sometimes if the serial port manifests itself as a particular device, based on USB vendor ID and product ID, the driver can make a good guess.

The README of each driver can provide information if hotplug is supported or not.

#### USB to Serial Port Enumeration
On Linux most of the USB to serial devices will be named '/dev/ttyUSB0', '/dev/ttyUSB1' etc. or  '/dev/ttyACM0', '/dev/ttyACM1' etc. Each device will always be ttyUSB or ttyACM but there is no way to know the number. Therefore it is advised to connect and power up (if external power is required) all USB serial devices before booting up the Linux system (for example the Raspberry Pi). Then identify devices by connecting the INDIGO drivers to them one by one. Once identified, save each driver configuration to "Profile 0".
Next time you have to make sure that all USB serial devices are connected to the same USB ports and powered up before booting the Linux machine.

The same applies for MacOSX. The only difference is the device name on MacOSX of the USB to serial devices are usually called /dev/cu.usbserial.
