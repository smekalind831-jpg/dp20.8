import os
import sys
import re
import shutil
from subprocess import Popen, PIPE
from glob import glob

def print_err(string):
	sys.stderr.write(string + "\n")
	return

# Проверка на версию python
if sys.version_info < (3,3,0):
	print_err("Для работы данного скрипта необходима версия python не ниже 3.3.0")
	exit(1)

# Проверка на количество входных аргументов
if len(sys.argv) < 3:
	print_err("")
	print_err("Порядок использования: " + os.path.basename(__file__) + " ew_dir ewp_dir")
	print_err("")
	print_err("     ew_dir  - папка установки IAR EW")
	print_err("     ewp_dir - папка проекта IAR")
	print_err("")
	exit(1)
else:
# Папка установки IAR
	iar_path = sys.argv[1]
# Папка проекта IAR, содержащая ewp-файл
	app_dir = sys.argv[2]

# Получаем путь до папки со скриптами
script_path = os.path.dirname(os.path.realpath(__file__))
	
# Получаем путь до утилиты bin2prg
bin2prg_file = script_path + "\\bin2prg.exe"

# Получаем путь до утилиты ielftool
ielftool_file = iar_path + "\\arm\\bin\\ielftool.exe"
if not os.path.exists(ielftool_file):
	print_err("")
	print_err("Ошибка: Не найден файл " + ielftool_file)
	print_err("Проверьте правильность пути " + iar_path)
	print_err("")
	exit(1)
	
# Проверяем на наличие app_dir
if not os.path.exists(app_dir):
	print_err("Ошибка: Не найдена указанная папка проекта " + app_dir)
	exit(1)

# Базовое имя проекта
app_name = os.path.basename(app_dir)
app_name_split=os.path.splitext(app_name)


# Конвертируем выходной файл в prg-файл

# Формируем команду для добавления заливки пустых областей 0xFF-ми и контрольных сумм
cmd_str = "\"" + ielftool_file + "\""
cmd_str += " --fill \"0xFF;__checksum_begin1-__checksum_end1\" "
cmd_str += "--fill \"0xFF;__checksum_begin2-__checksum_end2\" "
cmd_str += "--checksum \"__checksum1:4,crc32:1mi,0xffffffff;__checksum_begin1-__checksum_end1\" "
cmd_str += "--checksum \"__checksum2:4,crc32:1mi,0xffffffff;__checksum_begin2-__checksum_end2\" "
cmd_str += "--verbose \"" + app_dir + "\" \"" + app_dir + "\""
#print(cmd_str)

# Вызываем сформированную команду
cmd_status = Popen(cmd_str, stdout=PIPE, shell=True)
# Ожидаем окончания выполнения команды
cmd_status.wait()
# Контролируем код возврата
if cmd_status.returncode > 0:
	print_err("Ошибка при вызове команды:")
	print_err(cmd_str)
	exit(cmd_status.returncode)
app_dir=os.path.dirname(app_dir)
	
# Формируем команду для преобразования out-файла в bin-файл
bin_full = app_dir + "\\" + app_name_split[0] + ".bin"
cmd_str = "\"" + ielftool_file + "\""
cmd_str += " --bin --verbose \"" + app_dir + "\\" + app_name + "\" \"" + bin_full + "\""
#print(cmd_str)
	
# Вызываем сформированную команду
cmd_status = Popen(cmd_str, stdout=PIPE, shell=True)
# Ожидаем окончания выполнения команды
cmd_status.wait()
# Контролируем код возврата
if cmd_status.returncode > 0:
	print_err("Ошибка при вызове команды:")
	print_err(cmd_str)
	exit(cmd_status.returncode)
#print("\n")
"""
# Формируем команду для преобразования bin-файла в prg-файл
cmd_str = "\"" + bin2prg_file + "\"" + " \"" + bin_full + "\""
#print(cmd_str)

# Вызываем сформированную команду
cmd_status = Popen(cmd_str, stdout=PIPE, shell=True)
# Ожидаем окончания выполнения команды
cmd_status.wait()
# Контролируем код возврата
if cmd_status.returncode > 0:
	exit(cmd_status.returncode)
#print("\n")
"""
"""
# Склеиваем с загрузчиком
bin_full_boot = app_dir + "\\" + app_name_split[0] + "_w_boot" + ".bin"
bootloader = app_dir + "\\..\\..\\bootloader\\bootloader.bin"
destination = open(bin_full_boot,'wb')
shutil.copyfileobj(open(bootloader,'rb'), destination)
shutil.copyfileobj(open(bin_full,'rb'), destination)
destination.close()
"""

	
exit(0)

#python "$PROJ_DIR$\..\..\..\core\scripts\post_build.py" "$EW_DIR$" "$EXE_DIR$\ogm240_16_3_01.out"