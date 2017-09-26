#!/bin/sh

#Bashscript voor het starten van het totale project in parallel
#moet gerund worden als root, anders doen openvpn en main het niet

ozwarg='-p 5555'

screen -S openvpn -d -m openvpn --config /home/pi/openvpn.ovpn
screen -S openzwave -d -m /home/pi/controlpanel/open-zwave-control-panel/ozwcp $ozwarg
screen -S main ./main