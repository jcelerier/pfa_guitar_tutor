#!/bin/sh

rep_courant=$(pwd)
nom='GuitarTutor.desktop'
cd ~/Bureau
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
	
