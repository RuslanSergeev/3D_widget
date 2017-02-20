#include "SX_3D_Client.h"
#include "SX_3D_Widget.h"
#include <QApplication>
#include <QFile>
#include <QByteArray>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SX_3D_Client client;
    client.show();

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile("teapot.obj",
                                             aiProcess_Triangulate|aiProcess_GenNormals);
    if(!scene)
    {
        qDebug() << "cannot read model";
    }
    else
    {
        qDebug() << "Scene created!";
        qDebug() << QString("meshes: ") + QString::number(scene->mNumMeshes);
        qDebug() << QString("0 faces: ") + QString::number(scene->mMeshes[0]->mNumFaces);
        qDebug() << QString("0 vertoces: ") + QString::number(scene->mMeshes[0]->mNumVertices);
        qDebug() << QString("0 norm: ") +
                    QString::number(scene->mMeshes[0]->mNormals[0].x) + " " +
                QString::number(scene->mMeshes[0]->mNormals[0].x) + " " +
                QString::number(scene->mMeshes[0]->mNormals[0].x);
        qDebug() << QString("0 Indices: ") +
                    QString::number(scene->mMeshes[0]->mFaces[5].mNumIndices) + " " +
                QString::number(scene->mMeshes[0]->mFaces[5].mIndices[0]) + " " +
                QString::number(scene->mMeshes[0]->mFaces[5].mIndices[1]) + " " +
                QString::number(scene->mMeshes[0]->mFaces[5].mIndices[2]);

        qDebug() << QString("materials: ") + QString::number(scene->mNumMaterials);
        qDebug() << QString("materials 0: ") + QString::number(scene->mMaterials[0]->GetTextureCount(aiTextureType_AMBIENT));
        qDebug() << QString("materials 0: ") + QString::number(scene->mMaterials[0]->GetTextureCount(aiTextureType_DIFFUSE));
        qDebug() << QString("materials 0: ") + QString::number(scene->mMaterials[0]->GetTextureCount(aiTextureType_SPECULAR));
        qDebug() << QString("materials 0: ") + QString::number(scene->mMaterials[0]->GetTextureCount(aiTextureType_NORMALS));
        qDebug() << QString("materials 0: ") + QString::number(scene->mMaterials[0]->GetTextureCount(aiTextureType_HEIGHT));


    }

    return a.exec();
}
