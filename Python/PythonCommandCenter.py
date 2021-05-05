import PySimpleGUI as sg
from theme import themesetup
from time import sleep
import serial
# import io

themesetup()
port = serial.Serial('COM5', baudrate=115200)
# Port Connection #
port.write_timeout = 0.1
port.read_timeout = 0.1
print(f'Port opened at {port.name}')

# Window Layout #

leftcol = [
           [sg.Radio('1', 1, True), sg.Radio('5', 1), sg.Radio('10', 1)],
           [sg.Listbox(values=['Placeholder', 'More Placeholder', 'etc.'], no_scrollbar=True, s=(18,9), enable_events=True, k='Preset')]
]

dirbuttons = [
              [sg.RealtimeButton('🡅', k='F', font=(None, 20), size=(3,1))],
              [sg.RealtimeButton('🡄', k='L', font=(None, 20), size=(3,1)),
                       sg.Button('⬤', k='H', font=(None, 20), size=(3,1)),
               sg.RealtimeButton('🡆', k='R', font=(None, 20), size=(3,1))],
              [sg.RealtimeButton('🡇', k='B', font=(None, 20), size=(3,1))]
]

layout = [
          [sg.Column(leftcol, vertical_alignment='bottom'),
           sg.Column(dirbuttons, element_justification='center'),
           sg.Column([[sg.Output(s=(10,10), echo_stdout_stderr = True)]])]
]

window = sg.Window('BMC Controller', layout)

# Main Loop #

while True:
    event, values = window.Read(timeout=100, timeout_key='')

    if event == sg.WIN_CLOSED:
        window.close()
        break

    if event != '': print(event)
    step = 'A' if values[0] == True else 'B' if values[1] == True else 'C'

    if event == 'F':
        try: port.write(f'SA{step}F'.encode())
        except serial.serialutil.SerialTimeoutException: pass

    if event == 'B':
        try: port.write(f'SA{step}B'.encode())
        except serial.serialutil.SerialTimeoutException: pass


    # port.read()
    # port.read_until('X'.encode())  # to be implemented on review
    sleep(0.05)