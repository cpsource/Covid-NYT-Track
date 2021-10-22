#!/bin/bash
# This is a hack for vmware to mount a share
sudo vmhgfs-fuse .host:/ /mnt/hgfs -o allow_other
