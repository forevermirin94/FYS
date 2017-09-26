#!/bin/sh

#Script voor het uploaden van een still van een RPI camera
#lftp op https://github.com/lavv17/lftp.git
#deps: autoconf automake gettext libreadline-dev zlib-dev libtool-dev gnulib libssl-dev $

while :
do
        echo Uploading picture...
        DATE=$(date +"%Y-%m-%d_%T")

        raspistill -vf -hf -w 1280 -h 1024 -q 10 -o /home/pi/stills/$DATE.jpg
        #uploaden duurt vrij lang, ongeveer 5 seconde
        lftp -c "open -u upload,WACHTWOORD sftp://get.rekt.info:9877;cd /html/FYS/stills;put /home/pi/stills/$DATE.jpg;exit" # WACHTWOORD was te informeel ...

        ./sql $DATE

        sudo rm -rf /home/pi/stills/*

        #De bedrijfsregel is om de 30 seconde een foto opname ...
        #opnames maken duurt ongeveer  7s met een marge van 1s
        #Dus vandaar 23s sleep
        echo Camera is  sleeping  now...
        sleep 23
done