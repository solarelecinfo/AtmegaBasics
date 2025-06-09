FILE_NAME=main

#Commande pour programmer la memoire flash du microcontroleur à partir du fichier .hex
avrdude -p atmega32 -c usbasp -U flash:w:${FILE_NAME}.hex  &&
echo "execution program done"


