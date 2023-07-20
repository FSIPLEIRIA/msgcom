# MSGCOM

A ros2 node meant to export some messages from the lart ros2 environment into a more raw format for it to be parsed by the eletronics department. ***THIS PACKAGE IS NOT PRODUCTION READY, AND WAS DEVELOPED WITH TESTING/PROFILING IN MIND***

### Getting started

This is a ros2 package, as in all other packages start by cloning this repo on `$ros2_workspace/src` then head to your ros2_workspace and build your packages using `colcon build` if everything goes  according to plan and nothing fails source it by `source ./install/setup.sh` (you can use whatever shell you want) now you can just
`ros2 launch msgcom startup.launch.xml` this will launch the packet with default params although you should definetely check config and if you are in a profile or testing environment you should definetely create your own launch and config files.

### Uart

See termios, `man termios` on your terminal should do it for now. To understand how linux operates when it comes to devices, and even connections like serial you should understand the basis of Unix of "everything is a file" (except on mondays when you use socket(floating file descriptors dont count as files...)).
But basically when you connect anything to the hardware of a linux OS you should see a new file be created under /dev.
The new file should be named something like ttyUSB0, ttyACM0, ttyserial? or if you are using this in a SBC the serials will be dedicated so use the ttyS#(# represents a number dummy).

#### Grep it if you can, diff it if you cant:

If `ls /dev | grep something` has gotten you nowhere, just do this neet yet slow method:

`ls /dev >> /tmp/trash1`
Then insert/remove the device

`ls /dev >> /tmp/trash2`
Then just:
`diff /tmp/trash1 /tmp/trash2` this should give you which files were created/destroyed during the insertion/removal of the device ie: most likely youll get the serial ports you can use(if you insert something complex of course you will get other "files" use your intuition to figure out which to use)

#### Yes, in linux you just have to write to a file to send a serial signal...

if you write something like echo "blah" >> /dev/ttyUSB0 you will get a blah on the serial line, that is if you have perms, landed on the baud rate, etc...

In the first iterations we didnt even use a dedicated serial library or special flags, we just wrote to the file, unfortunately the mini car works at 9600 Baud rate and the only way to change that would be to use external programs to alter the default behaviour. This is also the reason there is a mismatch between cpp functions and C ones in the future a migration to some library that better integrates with cpp would be desired.
