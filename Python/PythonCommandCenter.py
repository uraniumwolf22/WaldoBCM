import PySimpleGUI as sg
from theme import themesetup
from time import sleep
import socket
import json
# import io

themesetup()

# Port Connection #

HOST = 'localhost'    # The remote host
PORT = 8002            # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

# Window Layout #

leftcol = [
           [sg.Text('Step Size'), sg.Spin([i for i in range(1,360)], initial_value=15, size=(6,1))],
           [sg.Text('Step Speed'), sg.Spin([i for i in range(1,500)], initial_value=250, size=(5,1))],
           # [sg.Listbox(values=['Placeholder', 'More Placeholder', 'etc.'], no_scrollbar=True, s=(19,8), enable_events=True, k='Preset')]
           [sg.Listbox(values=['A', 'B', 'C', 'D', 'E', 'F'], font=('Consolas', 12), size=(7,6), no_scrollbar=True, select_mode='multiple'),
            sg.Listbox(values=['G', 'H', 'I', 'J', 'K', 'L'], font=('Consolas', 12), size=(7,6), no_scrollbar=True, select_mode='multiple')]
]

rightcol = [
            # [sg.Radio('Step', 2, True), sg.Radio('Servo', 2)],
            [sg.Output(s=(14,11), echo_stdout_stderr=True)]
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
    command = (':'+':'.join([pos, dir, time, step])+':')
    print(command)
    s.sendall(json.dumps(command).encode())


while True:
    event, values = window.Read(timeout=100, timeout_key='')

    if event == sg.WIN_CLOSED:
        window.close()
        break

    if event != '': print(event)
    motors = values[2] + values[3]
    time = str(values[1])
    step = str(values[0])

    if event == 'F':
        for i in motors: motorMove(i, 'F', time, step)
        sleep(int(time)/1000)
    if event == 'B':
        for i in motors: motorMove(i, 'B', time, step)
        sleep(int(time)/1000)