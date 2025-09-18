import socket
import threading
import tkinter as tk
from tkinter import ttk

class RGBLampServer:
    def __init__(self, host='127.0.0.1', port=32234):
        # Network setup
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((host, port))
        self.running = True

        # GUI setup
        self.root = tk.Tk()
        self.root.title("RGB Lamp Server")
        self.root.geometry("300x400")

        # Color display
        self.color_panel = tk.Canvas(self.root, width=200, height=200)
        self.color_panel.pack(pady=20)
        self.color_rect = self.color_panel.create_rectangle(0, 0, 200, 200, fill='black')

        # Status labels
        self.status_frame = ttk.LabelFrame(self.root, text="Lamp Status", padding=10)
        self.status_frame.pack(fill='x', padx=10)

        self.rgb_label = ttk.Label(self.status_frame, text="RGB: (0, 0, 0)")
        self.rgb_label.pack()

        self.brightness_label = ttk.Label(self.status_frame, text="Brightness: 0")
        self.brightness_label.pack()

        self.state_label = ttk.Label(self.status_frame, text="State: OFF")
        self.state_label.pack()

        # Start listening thread
        self.listen_thread = threading.Thread(target=self.listen_for_commands)
        self.listen_thread.daemon = True
        self.listen_thread.start()

    def update_display(self, r, g, b, brightness, state):
        # Adjust color based on brightness
        brightness_factor = brightness / 255.0
        display_r = int(r * brightness_factor)
        display_g = int(g * brightness_factor)
        display_b = int(b * brightness_factor)
        
        # Convert RGB to hex color
        color = f'#{display_r:02x}{display_g:02x}{display_b:02x}'
        
        # Update GUI elements
        self.color_panel.itemconfig(self.color_rect, fill=color)
        self.rgb_label.config(text=f"RGB: ({r}, {g}, {b})")
        self.brightness_label.config(text=f"Brightness: {brightness}")
        self.state_label.config(text=f"State: {'ON' if state else 'OFF'}")

    def listen_for_commands(self):
        print(f"Listening on port {self.sock.getsockname()[1]}...")
        while self.running:
            try:
                data, addr = self.sock.recvfrom(1024)
                if len(data) >= 5:
                    r, g, b, brightness, state = data[:5]
                    print(f"\nReceived command from {addr}:")
                    print(f"RGB: ({r}, {g}, {b})")
                    print(f"Brightness: {brightness}")
                    print(f"State: {'ON' if state else 'OFF'}")

                    # Update GUI in thread-safe way
                    self.root.after(0, self.update_display, r, g, b, brightness, state)

                    # Send acknowledgment
                    self.sock.sendto(b'\x06', addr)
            except Exception as e:
                print(f"Error: {e}")

    def run(self):
        try:
            self.root.mainloop()
        finally:
            self.running = False
            self.sock.close()

if __name__ == "__main__":
    server = RGBLampServer()
    server.run()