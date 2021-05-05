import PySimpleGUI as sg
from theme import themesetup
from time import sleep
import serial
# import io

themesetup()

port = serial.Serial('COM5', baudrate=115200)
port.write_timeout = 0.1
print(f'Port opened at {port.name}')

dirbuttons = [
              [sg.RealtimeButton('^', k='f', size=(4,2))],
              [sg.RealtimeButton('<', k='l', size=(4,2)), sg.RealtimeButton('o', k='h', size=(4,2)), sg.RealtimeButton('>', k='r', size=(4,2))],
              [sg.RealtimeButton('v', k='b', size=(4,2))]
]

layout = [
          [sg.Frame('', dirbuttons, relief='flat', element_justification='center')],
          [sg.Listbox(values=['Placeholder', 'More Placeholder'], no_scrollbar=True, size=(22,2))]
]

window = sg.Window('BMC Controller', layout)

while True:
    event, values = window.Read(timeout=100, timeout_key='\n')
    print(event)
    if event == sg.WIN_CLOSED:
        window.close()
        break

    if event == 'f':
        try: port.write('SAAF'.encode())
        except serial.serialutil.SerialTimeoutException: pass

    if event == 'b':
        try: port.write('SAAB'.encode())
        except serial.serialutil.SerialTimeoutException: pass

    sleep(0.01)
