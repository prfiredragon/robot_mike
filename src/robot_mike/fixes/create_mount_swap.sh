#/bin/bash
echo "Creating the swapfile on /mnt/"
dd if=/dev/zero of=/mnt/swapfile bs=1M count=1600
echo "Converting the swapfile to swap fs"
mkswap /mnt/swapfile
echo "Fixing permisions"
chmod 0600 swapfile
echo "Mounting the swapfile"
swapon /mnt/swapfile
echo "Done ..."
