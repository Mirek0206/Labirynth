import serial
import threading
import tkinter as tk
from tkinter.scrolledtext import ScrolledText

class SerialReader(threading.Thread):
    def __init__(self, port='COM4', baudrate=9600, text_widget=None):
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.text_widget = text_widget
        self.stop_event = threading.Event()
        self.serial_connection = None

    def run(self):
        try:
            self.serial_connection = serial.Serial(self.port, self.baudrate)
            while not self.stop_event.is_set():
                if self.serial_connection.in_waiting > 0:
                    try:
                        data = self.serial_connection.readline().decode('utf-8', errors='ignore').strip()
                        if self.text_widget:
                            self.text_widget.insert(tk.END, data + '\n')
                            self.text_widget.yview(tk.END)
                    except UnicodeDecodeError as e:
                        if self.text_widget:
                            self.text_widget.insert(tk.END, f'Decode error: {e}\n')
        except serial.SerialException as e:
            if self.text_widget:
                self.text_widget.insert(tk.END, f'Error: {e}\n')
        finally:
            self.close_connection()

    def stop(self):
        self.stop_event.set()
        self.close_connection()

    def close_connection(self):
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()

def start_serial_reader():
    global serial_reader
    serial_reader = SerialReader(port_entry.get(), int(baudrate_entry.get()), text_area)
    serial_reader.start()

def stop_serial_reader():
    global serial_reader
    if serial_reader:
        serial_reader.stop()
        serial_reader.join()

def on_closing():
    stop_serial_reader()
    root.destroy()

root = tk.Tk()
root.title("Serial Port Listener")

frame = tk.Frame(root)
frame.pack(padx=10, pady=10)

port_label = tk.Label(frame, text="Port:")
port_label.grid(row=0, column=0, padx=5, pady=5)
port_entry = tk.Entry(frame)
port_entry.grid(row=0, column=1, padx=5, pady=5)
port_entry.insert(0, 'COM4')

baudrate_label = tk.Label(frame, text="Baudrate:")
baudrate_label.grid(row=1, column=0, padx=5, pady=5)
baudrate_entry = tk.Entry(frame)
baudrate_entry.grid(row=1, column=1, padx=5, pady=5)
baudrate_entry.insert(0, '9600')

start_button = tk.Button(frame, text="Start", command=start_serial_reader)
start_button.grid(row=2, column=0, columnspan=2, pady=10)

text_area = ScrolledText(root, wrap=tk.WORD, width=200, height=20)
text_area.pack(padx=10, pady=10)

root.protocol("WM_DELETE_WINDOW", on_closing)

serial_reader = None
root.mainloop()
