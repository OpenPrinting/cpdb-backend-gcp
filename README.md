# Google Cloud Print Common Print Dialog Backend

This repository hosts the code for the Google Cloud Print **C**ommon **P**rint **D**ialog **B**ackend. This backend manages and provides information about Google Cloud Print printing destinations to the printing dialog.

## Background

The [Common Printing Dialog](https://wiki.ubuntu.com/CommonPrintingDialog) project aims to provide a uniform, GUI toolkit independent printing experience on Linux Desktop Environments.

## Dependencies

### For building the backend

- [cpdb-libs](https://github.com/OpenPrinting/cpdb-libs) : Version >= 1.2.0

- GLIB 2.0:
`sudo apt install libglib2.0-dev`

- REST 0.7:
`sudo apt install librest-dev`

- JSON-GLIB 1.0:
`sudo apt install libjson-glib-dev`

- GOA 1.0:
`sudo apt install libgoa-1.0-dev`

### For setting up the GNOME online account

- GNOME Control Center and GNOME Online Accounts
sudo apt install gnome-control-center gnome-online-accounts

## Build and installation

    $ ./autogen.sh
    $ ./configure
    $ make
    $ sudo make install

## Following the development and updating

The current source code you find on the [OpenPrinting GitHub](https://github.com/OpenPrinting/cpdb-backend-gcp).

## Running

Set up your GNOME online account for Google access by following the
first answer to [this user
question](https://askubuntu.com/questions/838956/ubuntu-16-04-set-up-with-google-online-account-but-no-drive-folder-in-nautilus). Then
the backend will list your Google Cloud Print printers and your Google
Drive in the print dialog as long as you are logged in. For users not
setting up their GNOME online account the backend will not show any
destination.

The backend is auto-activated when a frontend runs; So no need to run it explicitly.

## More info

[Abhijeet Dubey's Google Summer of Code 2017 Final Report](https://github.com/dracarys09/gcp-backend/wiki/1.-Google-Summer-of-Code-2017-%7C-Common-Printing-Dialog)
