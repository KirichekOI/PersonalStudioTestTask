# PersonalStudioTestTask
#ServerDevice
количество каналов устанавливается переменной CHANNEL_CNT в файле multimeterconstants.h
лог-файл будет сгенерирован в той же директории, где находится исполняемый файл. Имя файла в формате dmY.log (день, месяц, год).
Сборка:
система сборки qmake.
команды:
qmake ServerDevice.pro
make

#ClientDevice
количество каналов устанавливается переменной CHANNEL_CNT в файле localsocket.h
Сборка:
система сборки qmake.
команды:
qmake ClientDevice.pro
make

