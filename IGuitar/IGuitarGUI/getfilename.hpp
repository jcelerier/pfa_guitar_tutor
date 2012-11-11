#ifndef GETFILENAME_HPP
#define GETFILENAME_HPP

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <QString>
#include <iostream>
#include <QStringList>

using namespace std;

class GetFileName
{
public:
    GetFileName();
    int getdir(QString dir, QStringList &files);
    const QStringList getFileName(QString &);
    int split(vector<string> &, string, char);
};

#endif // GETFILENAME_HPP
