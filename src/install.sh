# install the binary in /usr/bin/print-backends
echo "installing the gcp backend executable in /usr/bin/print-backends .."
mkdir -p /usr/bin/print-backends/
cp server /usr/bin/print-backends/gcp

echo "installing interfaces in /usr/share/dbus-1/interfaces .."
cp ../interface/org.openprinting.Backend.xml /usr/share/dbus-1/interfaces

echo "installing service file org.openprinting.Backend.CUPS.service in /usr/share/dbus-1/services .."
cp ../aux/org.openprinting.Backend.GCP.service /usr/share/dbus-1/services

#this is the folder which lets frontends know what backends are there and what are their object paths
echo "installing configuration files in /usr/share/print-backends .."
mkdir -p /usr/share/print-backends
cp ../aux/org.openprinting.Backend.GCP /usr/share/print-backends

echo "creating directory for log file at  ~/.print-backends/logs/"
mkdir -p ~/.print_backends/logs/

echo "removing binary files from current directory"
make clean

echo "Done."
