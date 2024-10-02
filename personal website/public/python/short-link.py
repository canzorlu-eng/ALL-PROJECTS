import tkinter as tk
from tkinter import messagebox
import requests
import pyperclip


def shorten_url():
    long_url = entry.get()
    response = requests.get(f'http://tinyurl.com/api-create.php?url={long_url}')
    short_url = response.text
    result_label.config(text=f'Kısaltılmış Link:{short_url}')
    copy_button.config(state=tk.NORMAL)


def copy_to_clipboard():
    short_url= result_label.cget("text")[17:] 
    pyperclip.copy(short_url)
    messagebox.showinfo("Kopyalandı", "Kısa URL Kopyalandı")


#tkinter alanı


app = tk.Tk()
app.title("Link Kısaltıcı")

#uzun adreslerin girileceği yer

label = tk.Label(app, text="Uzun Linki Giriniz:")
label.pack(pady=10)
entry = tk.Entry(app, width=40)
entry.pack()

#url kısaltma düğmesi
shorten_button = tk.Button(app, text="Kısalt", command=shorten_url)
shorten_button.pack()

#kısa url adresinin görüneceği yer
result_label = tk.Label(app, text="")
result_label.pack()

#kopyala butonu

# Kopyala düğmesi
copy_button = tk.Button(app, text="Kopyala", command=copy_to_clipboard, state=tk.DISABLED)
copy_button.pack(pady=5)

# Pencereyi çalıştır
app.mainloop()







app.mainloop()


