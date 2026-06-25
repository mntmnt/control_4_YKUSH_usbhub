#!/usr/bin/env bash
set -Eeuo pipefail

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

USR_LIB=/usr/lib/udev/rules.d
ETC_LIB=/etc/udev/rules.d
FILENAME=98-ykshusbhub-udev.rules

#[ -d "/usr/lib" ] && echo "DIR EXISTS" || echo "DOES NOT EXISTS"
function checkRuleExists() {
	if [ -e "$DIR/$FILENAME" ]; then 
		return 0
	else
		echo "NO $FILENAME in current directory"
		echo "It's content should be simple. Like below:"
		echo 'SUBSYSTEMS=="usb", ATTRS{idVendor}=="04D8", MODE="0666"'
		echo
		echo 'read README for details'
		return 1
	fi
}

function installRule() {
	local RULES_FOLDER=""

	if [ -d "$ETC_LIB" ]; then
		RULES_FOLDER=$ETC_LIB
	elif [ -d "$USR_LIB" ]; then
		RULES_FOLDER=$USR_LIB
	else
		echo "no common udev-rule folder '$USR_LIB' or '$ETC_LIB'"
		return 1
	fi
	install -m 0644 "$DIR/$FILENAME" "$RULES_FOLDER/"
}

function uninstall() {
	[ -e "$USR_LIB/$FILENAME" ] && rm -f "$USR_LIB/$FILENAME"
	[ -e "$ETC_LIB/$FILENAME" ] && rm -f "$ETC_LIB/$FILENAME"
	return 0
}

function restartRules() {
	udevadm control --reload && udevadm trigger
}

if ! [ -z "${1+x}" ]; then
	if [ "$1" == "uninstall" ]; then
		uninstall && restartRules
		exit 0
	fi
fi
checkRuleExists && installRule && restartRules

