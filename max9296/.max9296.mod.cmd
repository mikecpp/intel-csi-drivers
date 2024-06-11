cmd_/home/mike/otocam-drivers/max9296/max9296.mod := printf '%s\n'   max9296.o | awk '!x[$$0]++ { print("/home/mike/otocam-drivers/max9296/"$$0) }' > /home/mike/otocam-drivers/max9296/max9296.mod
