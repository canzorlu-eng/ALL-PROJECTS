import tkinter as tk
from tkinter import ttk, messagebox


def hesapla():
    try:
        resolution = resolution_var.get()
        duration = int(duration_var.get())

        #Bit hızlarını çözünürlüklerine göre ayarlayalım(kbps)
        bit_rates = {
            "360p":750,
            "480p":1500,
            "720p":3000,
            "1080":4500,
        }
        bit_rate = bit_rates[resolution]

        #Dosya boyutuna göre hesaplama(MB Cinsinden)
        file_size_mb = bit_rate * duration * 60 / 8 / 1024
        result_label.config(text=f"Video Boyutu : {file_size_mb:.2f} MB")
    except ValueError:
        messagebox.showerror("Hata","Geçerli Bir Süre Giriniz.")


#ana pencereyi oluştur
root = tk.Tk()
root.title("Video Veri Kullanımı Hesaplayıcısı")
root.geometry("450x450")
root.resizable(False,False)

#Stil ayarları
style = ttk.Style()
style.configure("TLabel" , font=("Helvetica" ,14))
style.configure("TButton" , font=("Helvetica" ,14))
style.configure("TCombobox" , font=("Helvetica" ,14))

#Buton stili
style.map("TButton",foreground=[('active','black'),('!disabled','black')],
          background=[('active','#45a049') , ('!disabled','#45a049')])

#çözünürlük seçimi
resolution_label = ttk.Label(root, text="Çözünürlük :")
resolution_label.pack(pady=10)
resolution_var = tk.StringVar()
resolution_combobox = ttk.Combobox(root,textvariable=resolution_var , state='readonly')
resolution_combobox['values'] = ("350p", "480p" , "720p" , "1080p")
resolution_combobox.current(0)
resolution_combobox.pack(pady=10)


#süre girişi
duration_label = ttk.Label(root , text="Video Süresü (Dakika):")
duration_label.pack(pady=10)
duration_var = tk.StringVar()
duration_entry = ttk.Entry(root,textvariable=duration_var)
duration_entry.pack(pady=10)

#hesapla butonu
calculate_buton = ttk.Button(root, text="Hesapla", command=hesapla)
calculate_buton.pack(pady=20)
#sonuc etiketi
result_label = ttk.Label(root , text="Video Boyutu: ")
result_label.pack(pady=20)



#ana döngü

root.mainloop()
