# Jakub Dudziak
# !/bin/bash
# Testowanie na localhost - jeden czujnik przypadek alarmowy

gnome-terminal -x bash -c './Test_DETECTOR_2.sh; exec bash' &
gnome-terminal -x bash -c './Test_MANAGER.sh; exec bash'
exit
