import PySimpleGUI as sg
from theme import themesetup
from time import sleep
import serial
# import io

themesetup()

# Port Connection #

port = serial.Serial('COM5', baudrate=115200)
port.write_timeout = 0.05
port.read_timeout = 0.1
print(f'Port opened at {port.name}')

# Window Layout #

leftcol = [
           [sg.Text('Step Size'), sg.Spin([i for i in range(1,360)], initial_value=15, size=(6,1))],
           [sg.Text('Step Speed'), sg.Spin([i for i in range(1,500)], initial_value=250, size=(5,1))],
           [sg.Listbox(values=['Placeholder', 'More Placeholder', 'etc.'], no_scrollbar=True, s=(19,8), enable_events=True, k='Preset')]
]

rightcol = [
            [sg.Radio('Step', 2, True), sg.Radio('Servo', 2)],
            [sg.Output(s=(14,9), echo_stdout_stderr=True)]
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
           sg.Column(rightcol)]
]

window = sg.Window('BMC Controller', layout, font=('Consolas', 10))

# Main Loop #

def motorMove(pos, dir, time, step):
    try:
        command = (':'+':'.join([pos, dir, time, step])+':')
        print(command)
        port.write(command.encode())
        sleep(int(time)/1000)  # port.read_until('X'.encode())

    except serial.serialutil.SerialTimeoutException: print('Timed out!')

while True:
    event, values = window.Read(timeout=100, timeout_key='')

    if event == sg.WIN_CLOSED:
        window.close()
        break

    if event != '': print(event)
    pos = 'A' if values[2] else 'G'
    time = str(values[1])
    step = str(values[0])

    if event == 'F': motorMove(pos, 'F', time, step)
    if event == 'B': motorMove(pos, 'B', time, step)
