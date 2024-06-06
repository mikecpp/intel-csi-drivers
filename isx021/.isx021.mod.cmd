cmd_/home/mike/otocam-drivers/isx021/isx021.mod := printf '%s\n'   isx021.o | awk '!x[$$0]++ { print("/home/mike/otocam-drivers/isx021/"$$0) }' > /home/mike/otocam-drivers/isx021/isx021.mod
