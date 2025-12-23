#!/bin/bash

# Exit on error
set -e

echo "Creating 100MB random file..."
dd if=/dev/urandom of=test.bin bs=1M count=100 status=progress

echo "\nRunning hill cipher..."
# Assuming 'make run' handles the execution via qemu as configured
make run

echo "Computing hashes..."
sha256sum test.bin*
