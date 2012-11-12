#!/bin/bash

zenity --info --text="Lancement de l'installation"

rep_courant=$(zenity --title="Veuillez choisir un répertoire d'installation" --file-selection --directory)

#Pour savoir si c'est Desktop ou Bureau

#IF 0
if [ -d ~/Bureau ];then
Bureau=Bureau
else
Bureau=Desktop
fi
#FI 0
echo $rep_courant

cd $rep_courant

zenity --info --text="Les librairies suivantes vont être instalées :\nlibqt4-dev\nportaudio19-dev\nlibsndfile1-dev"

zenity --question --text="Acceptez vous l'installation ?"

#IF 1
if [ $?==0 ]
then
gksudo echo a
(
echo "10"
echo "# Installation de libqt4-dev en cours. \nCeci peut prendre quelques minutes" 
sudo apt-get -y install libqt4-dev
echo "20"
gksudo echo b
echo "21"
echo "# Installation de portaudio19-dev en cours. \nCeci peut prendre quelques minutes" 
sudo apt-get -y  install portaudio19-dev
echo "32"
gksudo echo c
echo "33"
echo "# Installation de libsndfile1-dev en cours. \nCeci peut prendre quelques minutes" 
sudo apt-get -y install libsndfile1-dev
echo "44"

#FI 2
if [ -f IGuitar/Release/libIGuitar.a ]; then
echo "librairie installee"
else
cd $rep_courant/IGuitar/Release/
echo "# Installation de libIGuitar en cours. \nCeci peut prendre quelques minutes" 
make clean
make all
echo "66"
fi
#FI 2

#IF 3
if [ -f IGuitarGUI/IGuitarGUI ]; then
echo "IGuitarGUI installe"
else
cd $rep_courant/IGuitarGUI/
echo "# Installation de IGuitarGUI en cours. \nCeci peut prendre quelques minutes" 
make clean
#on garde le makefile en attendant que les changements de code d'Alexia marche
#rm Makefile
#qmake
make all
echo "78"

#IF 4
if zenity --question --text="Voulez-vous créer un raccourci sur le $Bureau ?"
then
nom='IGuitarGUI.desktop'
cd ~/$Bureau
touch $nom

echo [Desktop Entry] >> $nom 
echo Version=1.0 >> $nom 
echo Type=Application >> $nom 
echo Name=GuitarTutor >> $nom 
echo Comment= >> $nom 
echo TryExec=$rep_courant/IGuitarGUI/IGuitarGUI >> $nom  
echo Exec=$rep_courant/IGuitarGUI/IGuitarGUI >> $nom 
echo Icon=$rep_courant/icone/guitare.png >> $nom 
echo Actions=Gallery;Create; >> $nom 
echo >> $nom 
echo [Desktop Action Gallery] >> $nom 
echo Exec=$rep_courant/IGuitarGUI/IGuitarGUI \-\-gallery >> $nom  
echo Name=Browse Gallery >> $nom 
echo >> $nom 
echo [Desktop Action Create] >> $nom 
echo Exec=$rep_courant/IGuitarGUI/IGuitarGUI \-\-create-new >> $nom  
echo Name=Play GuitarTutor! >> $nom 
echo Icon=$rep_courant/icone/guitare.png >> $nom

chmod +x $nom

fi
#FI 4

cd $rep_courant
fi
#FI 3

#IF 5
if [ -f PlayAnalysisGUI/PlayAnalysisGUI ]; then
echo "PlayAnalysisGUI installe"
else
cd $rep_courant/PlayAnalysisGUI/
echo "# Installation de PlayAnalysisGUI en cours. \nCeci peut prendre quelques minutes" 
make clean
rm Makefile
qmake
make all
echo "99"

#IF 6
if zenity --question --text="Voulez-vous créer un raccourci sur le $Bureau ?"
then
nom='PlayAnalysis.desktop'
cd ~/$Bureau
touch $nom

echo [Desktop Entry] >> $nom 
echo Version=1.0 >> $nom 
echo Type=Application >> $nom 
echo Name=PlayAnalysisGUI >> $nom 
echo Comment= >> $nom 
echo TryExec=$rep_courant/PlayAnalysisGUI/PlayAnalysisGUI >> $nom  
echo Exec=$rep_courant/PlayAnalysisGUI/PlayAnalysisGUI >> $nom 
echo Icon=$rep_courant/icone/partition.png >> $nom 
echo Actions=Gallery;Create; >> $nom 
echo >> $nom 
echo [Desktop Action Gallery] >> $nom 
echo Exec=$rep_courant/PlayAnalysisGUI/PlayAnalysisGUI \-\-gallery >> $nom  
echo Name=Browse Gallery >> $nom 
echo >> $nom 
echo [Desktop Action Create] >> $nom 
echo Exec=$rep_courant/PlayAnalysisGUI/IGuitarGUI \-\-create-new >> $nom  
echo Name=Play GuitarTutor! >> $nom 
echo Icon=$rep_courant/icone/guitare.png >> $nom

chmod +x $nom

fi
#FI 6

cd $rep_courant
fi
#FI 5

echo "100"
) |
zenity --progress \
  --title="Installation des librairies" \
  --text="Initialisation..." \
  --percentage=0 \
  --auto-close
else
exit
fi
#FI 1
zenity --info --text="Installation réussie"
