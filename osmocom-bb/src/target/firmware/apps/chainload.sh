#!/bin/bash

/home/andrew/GSMem/osmocom-bb/src/host/osmocon/osmocon -p /dev/ttyUSB0 -m c123xor -c /home/andrew/GSMem/osmocom-bb/src/target/firmware/board/compal_e88/$1.highram.bin
