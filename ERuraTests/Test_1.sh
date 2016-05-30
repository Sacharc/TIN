# Jakub Dudziak
# Testowanie na localhost - jeden czujnik przypadek alarmowy

gnome-terminal -x bash -c './Test_DETECTOR_1.sh; exec bash' &
gnome-terminal -x bash -c './Test_MANAGER.sh; exec bash'
exit
