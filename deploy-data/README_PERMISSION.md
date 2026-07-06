# Permission issues

Sometimes this USB application can see a device but cannot open a connection because of missing permissions.

This application uses HIDAPI/libudev on Linux. A regular non-admin user may not have permission to open, read, or write the `/dev/hidraw*` device node. The main sign of this issue is that the application detects the device but cannot connect to it.

You can either 
- Start the application with `sudo`
- Add permissions such as `0666` to the corresponding udev device node, usually `/dev/hidraw*`
- Add a udev rule

## udev rule

On most distributions, local udev rules should be placed in:

- `/etc/udev/rules.d/`

Some distributions also provide vendor/package rules in:

- `/usr/lib/udev/rules.d/`

Create a new rule or add the following line to an existing rule file:

```udev
KERNEL=="hidraw*", ATTRS{idVendor}=="04d8", ATTRS{idProduct}=="f2f7", MODE="0666"
```

The hexadecimal values must be lowercase because udev compares them against sysfs strings exactly.

This command shows udev device information and attributes that can be used in rules:

```sh
udevadm info --path=path/to/device --attribute-walk
```

There is a `98-ykshusbhub-udev.rules` file in this directory. Copy it into one of the locations above, then reload udev or restart the computer.

To reload udev:

```sh
sudo udevadm control --reload && sudo udevadm trigger
```

For convenience, `installRule.sh` installs or uninstalls the rule file. Run it with root privileges:

```sh
sudo ./installRule.sh
```

To uninstall the rule:

```sh
sudo ./installRule.sh uninstall
```


## Useful Commands

```sh
dmesg -w
```

If device is detected as "hidraw*" the output may be the following
```
[ 1741.552496] usb 1-13.1: new full-speed USB device number 17 using xhci_hcd
[ 1741.639685] usb 1-13.1: New USB device found, idVendor=0483, idProduct=3efb, bcdDevice= 3.28
[ 1741.639702] usb 1-13.1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[ 1741.639708] usb 1-13.1: Product: (dev) pVNA 6G j701HID
[ 1741.639713] usb 1-13.1: Manufacturer: pocketvna.com
[ 1741.639716] usb 1-13.1: SerialNumber: 205A3681574B
[ 1741.646947] hid-generic 0003:0483:3EFB.0007: hiddev3,hidraw6: USB HID v1.11 Device [pocketvna.com (dev) pVNA 6G j701HID] on usb-0000:00:14.0-13.1/input0
```

Otherwise the latest `hid-generic...` line would be missing. In this case device usually can be opened using libusb backend only!


