FILE_NAME=main_input

avrdude -p atmega32 -c usbasp -U flash:w:${FILE_NAME}.hex  &&
echo "execution program done"


