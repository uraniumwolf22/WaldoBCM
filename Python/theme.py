import PySimpleGUI as sg

def themesetup(default=False):
    if default == True:
        sg.theme('SystemDefaultForReal')
    else:
        ui_back, ui_mid, ui_front, ui_text = '#373737', '#404040', '#505050', '#A4A4A4'
        sg.theme_background_color(ui_mid)
        sg.theme_element_background_color(ui_mid)
        sg.theme_input_background_color(ui_back)
        sg.theme_text_element_background_color(ui_mid)
        sg.theme_button_color((ui_text, ui_back))
        sg.theme_element_text_color('white')
        sg.theme_input_text_color('white')
        sg.theme_element_text_color(ui_text)
        sg.theme_input_text_color(ui_text)
        sg.theme_text_color(ui_text)
        sg.theme_slider_color(ui_front)
        sg.theme_border_width(1)
