cmd_/home/mike/otocam-drivers/ar0234/ar0234.mod := printf '%s\n'   ar0234.o | awk '!x[$$0]++ { print("/home/mike/otocam-drivers/ar0234/"$$0) }' > /home/mike/otocam-drivers/ar0234/ar0234.mod
