import instaloader
import tkinter as tk
from tkinter import messagebox

def download_post():
    username=entry_username.get()

    try:
        bot=instaloader.Instaloader()

        profile=instaloader.Profile.from_username(bot.context,username)

        posts=profile.get_posts()

        for index,post in enumerate(posts,1):
            bot.download_post(post,target=f"{profile.username}_{index}")
            
        messagebox.showinfo("successfully downloaded")
    
    except Exception as e:
        messagebox.showerror("Error",str(e))




root=tk.Tk()
root.title="instagram post downloader"
root.geometry("300x200")

label=tk.Label(root,text="username: ")
label.pack(pady=10)

entry_username=tk.Entry(root)
entry_username.pack()

download_button = tk.Button(root,text="download",command=download_post)
download_button.pack()



root.mainloop()



