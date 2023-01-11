#!/bin/bash

# Script which automates the start-up of the Leap device
sudo systemctl daemon-reload;
sudo service leapd start;
sudo leapd & 
