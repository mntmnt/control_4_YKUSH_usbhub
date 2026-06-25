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




