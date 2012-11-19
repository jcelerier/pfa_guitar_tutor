#include "getfilename.hpp"

GetFileName::GetFileName()
{
}

int GetFileName::getdir(QString dir, QStringList &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir((char *)dir.data())) == NULL) {
        //cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    int j = 0;
    while ((dirp = readdir(dp)) != NULL) {
        // obtenir le nom des fichiers presents
        QString str = (QString)(dirp->d_name);
        // split pour virer les extensions
        QStringList list = str.split(".");
        for(int i = 0; i<files.size(); i++){
            if (list.at(0) == files[i])
                j++;
        }
        // eviter la repetition de deux fichier portant le meme nom (mais pas la meme extension
        if (j != 0){
			j = 0;
            break;
        }
        // si le fichier n'a pas encore ete ajoute, on le rajoute a la liste
        files << list.at(0);
    }
    closedir(dp);
    return 0;
}

const QStringList GetFileName::getFileName(QString &path)
{
    //string dir = string(path);
    QStringList *files = new QStringList();

    // on remplit files par effet de bord grace a la methode getdir
    getdir(path, *files);

    return *files;
}

// methode qui etait utile dans une premiere version, mais qui n'est finalement pas utilisee

int GetFileName::split(vector<string>& vecteur, string chaine, char separateur)
{
    vecteur.clear();
    string::size_type stTemp = chaine.find(separateur);
    while(stTemp != string::npos)
    {
        vecteur.push_back(chaine.substr(0, stTemp));
        chaine = chaine.substr(stTemp + 1);
        stTemp = chaine.find(separateur);
    }
    vecteur.push_back(chaine);
    return vecteur.size();
}
