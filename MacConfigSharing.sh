#!/bin/bash

# This script permanently sets the Mac's Internet Sharing preferences
# so that the Mac will receive the IP address 192.168.2.1 and the
# attached device will always be assigned 192.168.2.2.
# REQUIRES OS X Yosemite (10.10) or newer
# Internet Sharing must be turned on and off once before this script
# is executed.

CLEAR="no"
SHOW="no"

for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	printf "%s [SWITCHES]   \n" $0
	printf "  --help, -h         \n"
	printf "  --clear, -c        \n"
	printf "  --show, -s         \n"
	exit 0;
    elif [ "${ARGI}" = "--clear" -o "${ARGI}" = "-c" ] ; then
	CLEAR="yes"
    elif [ "${ARGI}" = "--show" -o "${ARGI}" = "-s" ] ; then
	SHOW="yes"
    else
	printf "Bad Argument: %s \n" $ARGI
	exit 1
    fi
done

if [ "${CLEAR}" = "yes" ] ; then
    echo "Clearing /var/db/dhcp_clients"
    rm -f /var/db/dhcpd_leases
    exit 0
fi

if [ "${SHOW}" = "yes" ] ; then
    cat /var/db/dhcpd_leases
    exit 0
fi

sudo defaults write /Library/Preferences/SystemConfiguration/com.apple.nat NAT -dict-add SharingNetworkNumberStart 192.168.2.1
sudo defaults write /Library/Preferences/SystemConfiguration/com.apple.nat NAT -dict-add SharingNetworkNumberEnd 192.168.2.2
sudo defaults write /Library/Preferences/SystemConfiguration/com.apple.nat NAT -dict-add SharingNetworkMask 255.255.255.0
echo "Internet sharing now configured to assign 192.168.2.2"

