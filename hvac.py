import socket
import threading
import tkinter as tk
from tkinter import ttk

class HVACServer:
    def __init__(self, host='127.0.0.1', port=32234):
        # Konfiguracja UDP
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((host, port))
        self.running = True

        self.current_temperature = 20.0
        self.set_temperature = 0.0

        # GUI
        self.root = tk.Tk()
        self.root.title("HVAC Server")
        self.root.geometry("500x300")

        # Lista urządzeń
        self.device_list = ttk.Treeview(self.root, columns=("Name", "IP", "Status", "Type"), show="headings")
        self.device_list.heading("Name", text="Device Name")
        self.device_list.heading("IP", text="IP Address")
        self.device_list.heading("Status", text="Status")
        self.device_list.heading("Type", text="Type")
        self.device_list.pack(fill='both', expand=True, padx=10, pady=10)

        # Temperatury
        self.set_temp_label = tk.Label(self.root, text=f"Set Temperature: {self.set_temperature}°C")
        self.set_temp_label.pack(pady=5)
        self.current_temp_label = tk.Label(self.root, text=f"Current Temperature: {self.current_temperature}°C")
        self.current_temp_label.pack(pady=5)

        # Start wątku nasłuchującego
        self.listen_thread = threading.Thread(target=self.listen_for_connections)
        self.listen_thread.daemon = True
        self.listen_thread.start()

    def update_device(self, name, ip, status, device_type):
        """Aktualizuje listę urządzeń lub dodaje nowe urządzenie."""
        found = False
        for item in self.device_list.get_children():
            values = self.device_list.item(item, "values")
            if values[1] == ip:  # Sprawdzamy, czy urządzenie już istnieje na liście
                self.device_list.item(item, values=(name, ip, status, device_type))
                found = True
                break

        if not found:
            self.device_list.insert("", "end", values=(name, ip, status, device_type))

        self.root.update_idletasks()  # Odświeżenie GUI

    def update_temperature_labels(self, set_temperature, current_temperature):
        """Aktualizuje temperatury na ekranie i wykonuje logikę sterowania HVAC."""
        self.set_temperature = set_temperature
        self.current_temperature = current_temperature
        self.set_temp_label.config(text=f"Set Temperature: {set_temperature}°C")
        self.current_temp_label.config(text=f"Current Temperature: {current_temperature}°C")

        self.root.update_idletasks()  # Odświeżenie GUI

        # Sterowanie HVAC
        if current_temperature > set_temperature:
            print("Włączanie AC, wyłączanie Heater...")
        elif current_temperature < set_temperature:
            print("Włączanie Heater, wyłączanie AC...")
        else:
            print("Wyłączanie HVAC...")

    def listen_for_connections(self):
        """Nasłuchuje połączeń i aktualizuje GUI na podstawie otrzymanych danych."""
        while self.running:
            try:
                data, addr = self.sock.recvfrom(1024)  # Odbieranie danych przez UDP
                if data:
                    parts = data.decode('utf-8').split(";")

                    if len(parts) == 4:  # Otrzymano dane o urządzeniu
                        name, ip, status, device_type = parts
                        self.root.after(0, self.update_device, name, ip, status, device_type)

                    elif len(parts) == 2:  # Otrzymano dane o temperaturze
                        set_temperature, current_temperature = map(float, parts)
                        self.root.after(0, self.update_temperature_labels, set_temperature, current_temperature)

            except Exception as e:
                print(f"Błąd: {e}")

    def run(self):
        try:
            self.root.mainloop()
        finally:
            self.running = False
            self.sock.close()

if __name__ == "__main__":
    server = HVACServer()
    server.run()
