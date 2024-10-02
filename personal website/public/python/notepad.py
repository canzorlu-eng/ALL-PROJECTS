import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication,qApp,QAction,QMainWindow
from PyQt5.QtWidgets import QFileDialog
import os
class Pencere(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.init_ui()

    def init_ui(self):
        self.yazma_alanı=QtWidgets.QTextEdit()
        self.temizle=QtWidgets.QPushButton("temizle")
        self.ac=QtWidgets.QPushButton("aç")
        self.kaydet=QtWidgets.QPushButton("kaydet")
        h_box=QtWidgets.QHBoxLayout()
        h_box.addWidget(self.temizle)
        h_box.addWidget(self.ac)
        h_box.addWidget(self.kaydet)

        v_box=QtWidgets.QVBoxLayout()
        v_box.addWidget(self.yazma_alanı)
        v_box.addLayout(h_box)
        self.setLayout(v_box)


        self.temizle.clicked.connect(self.sil)
        self.ac.clicked.connect(self.dosya_ac)
        self.kaydet.clicked.connect(self.dosya_kaydet)


    def sil(self):
        self.yazma_alanı.clear()

    def dosya_ac(self):
        dosya_ismi = QFileDialog.getOpenFileName(self,"DOSYA AÇ",os.getenv("HOME"))#DESKTOP da yazılabilir(seçilen dosyanın bilgisayardaki yolunu tuple olarak döndürür)
        with open(dosya_ismi[0],"r",encoding="UTF-8") as file:
            self.yazma_alanı.setText(file.read())

    def dosya_kaydet(self):
        dosya_ismi = QFileDialog.getSaveFileName(self,"dosya kaydet",os.getenv("HOME"))
        with open(dosya_ismi[0],"w",encoding="UTF-8") as file:
            file.write(self.yazma_alanı.toPlainText())


class Menu(QMainWindow):
    def __init__(self):
        super().__init__()

        self.pencere=Pencere()

        self.setCentralWidget(self.pencere)

        self.menuleri_olustur()
    def menuleri_olustur(self):
        menubar = self.menuBar()
        dosya = menubar.addMenu("Dosya")
        
        dosya_ac = QAction("dosya aç",self)
        dosya_ac.setShortcut("Ctrl+O")

        dosya_kaydet = QAction("dosya kaydet",self)
        dosya_kaydet.setShortcut("Ctrl+S")

        temizle = QAction("temizle",self)
        temizle.setShortcut("Ctrl+D")

        cikis = QAction("çıkış",self)
        cikis.setShortcut("Ctrl+Q")

        dosya.addAction(dosya_ac)
        dosya.addAction(dosya_kaydet)
        dosya.addAction(temizle)
        dosya.addAction(cikis)
        

        dosya.triggered.connect(self.response)
        self.setWindowTitle("notepad")        
        self.show()


    def response(self,action):
        if action.text() == "dosya aç":
            self.pencere.dosya_ac
        elif action.text() == "dosya kaydet":
            self.pencere.dosya_kaydet()
        elif action.text()=="temizle":
            self.pencere.sil()
        elif action.text() == "çıkış":
            qApp.quit()


app = QtWidgets.QApplication(sys.argv)
menu = Menu()
sys.exit(app.exec_())