#include "qmlprojectitem.h"
#include "filefilteritems.h"
#include "qmlprojectfileformat.h"
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QtTest>

//TESTED_COMPONENT=src/plugins/qmlprojectmanager/fileformat

using namespace QmlProjectManager;

class tst_FileFormat : public QObject
{
    Q_OBJECT
public:
    tst_FileFormat();

private slots:
    void testFileFilter();
    void testMatchesFile();
    void testLibraryPaths();
};

tst_FileFormat::tst_FileFormat()
{
    QmlProjectFileFormat::registerDeclarativeTypes();
}

QString testDataDir = QLatin1String(SRCDIR "/data");

void tst_FileFormat::testFileFilter()
{
    //
    // Search for qml files in directory + subdirectories
    //
    QString projectFile = QLatin1String(
            "import QmlProject 1.0\n"
            "Project {\n"
            "  QmlFiles {"
            "  }"
            "}\n");

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent component(&engine);
        component.setData(projectFile.toUtf8(), QUrl());
        if (!component.isReady())
            qDebug() << component.errorString();
        QVERIFY(component.isReady());

        QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
        QVERIFY(project);

        project->setSourceDirectory(testDataDir);

        QStringList expectedFiles(QStringList() << testDataDir + "/file1.qml"
                                                << testDataDir + "/file2.qml"
                                                << testDataDir + "/subdir/file3.qml");
        QCOMPARE(project->files().toSet(), expectedFiles.toSet());
    }

    //
    // search for all qml files in directory
    //
    projectFile = QLatin1String(
            "import QmlProject 1.0\n"
            "Project {\n"
            "  QmlFiles {\n"
            "    recursive: false\n"
            "  }\n"
            "}\n");

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent component(&engine);
        component.setData(projectFile.toUtf8(), QUrl());
        if (!component.isReady())
            qDebug() << component.errorString();
        QVERIFY(component.isReady());

        QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
        QVERIFY(project);

        project->setSourceDirectory(testDataDir);

        QStringList expectedFiles(QStringList() << testDataDir + "/file1.qml"
                                                << testDataDir + "/file2.qml");
        QCOMPARE(project->files().toSet(), expectedFiles.toSet());
    }

    //
    // search for all qml files in subdirectory
    //
    projectFile = QLatin1String(
            "import QmlProject 1.0\n"
            "Project {\n"
            "  QmlFiles {\n"
            "    directory: \"subdir\"\n"
            "  }\n"
            "}\n");

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent component(&engine);
        component.setData(projectFile.toUtf8(), QUrl());
        QVERIFY(component.isReady());

        QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
        QVERIFY(project);

        project->setSourceDirectory(testDataDir);

        QStringList expectedFiles(QStringList() <<  testDataDir + "/subdir/file3.qml");
        QCOMPARE(project->files().toSet(), expectedFiles.toSet());
    }

    //
    // multiple entries
    //
    projectFile = QLatin1String(
            "import QmlProject 1.0\n"
            "Project {\n"
            "  QmlFiles {\n"
            "    directory: \".\"\n"
            "    recursive: false\n"
            "  }"
            "  QmlFiles {\n"
            "    directory: \"subdir\"\n"
            "  }\n"
            "}\n");

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent component(&engine);
        component.setData(projectFile.toUtf8(), QUrl());
        if (!component.isReady())
            qDebug() << component.errorString();
        QVERIFY(component.isReady());

        QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
        QVERIFY(project);

        project->setSourceDirectory(testDataDir);

        QStringList expectedFiles(QStringList() << testDataDir + "/file1.qml"
                                                << testDataDir + "/file2.qml"
                                                << testDataDir + "/subdir/file3.qml");
        QCOMPARE(project->files().size(), 3);
        QCOMPARE(project->files().toSet(), expectedFiles.toSet());
    }

    //
    // include specific list
    //
    projectFile = QLatin1String(
            "import QmlProject 1.0\n"
            "Project {\n"
            "  QmlFiles {\n"
            "    paths: [ \"file1.qml\",\n"
            "\"file2.qml\" ]\n"
            "  }\n"
            "}\n");

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent component(&engine);
        component.setData(projectFile.toUtf8(), QUrl());
        if (!component.isReady())
            qDebug() << component.errorString();
        QVERIFY(component.isReady());

        QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
        QVERIFY(project);

        project->setSourceDirectory(testDataDir);

        QStringList expectedFiles(QStringList() << testDataDir + "/file1.qml"
                                                << testDataDir + "/file2.qml");
        QCOMPARE(project->files().toSet(), expectedFiles.toSet());
    }

    //
    // include specific list
    //
    projectFile = QLatin1String(
            "import QmlProject 1.0\n"
            "Project {\n"
            "  ImageFiles {\n"
            "    directory: \".\"\n"
            "  }\n"
            "}\n");

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent component(&engine);
        component.setData(projectFile.toUtf8(), QUrl());
        if (!component.isReady())
            qDebug() << component.errorString();
        QVERIFY(component.isReady());

        QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
        QVERIFY(project);

        project->setSourceDirectory(testDataDir);

        QStringList expectedFiles(QStringList() << testDataDir + "/image.gif");
        qDebug() << project->files().toSet() << expectedFiles.toSet();
        QCOMPARE(project->files().toSet(), expectedFiles.toSet());
    }

    //
    // use wildcards
    //
    projectFile = QLatin1String(
            "import QmlProject 1.0\n"
            "Project {\n"
            "  ImageFiles {\n"
            "    filter: \"?mage.[gf]if\"\n"
            "  }\n"
            "}\n");

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent component(&engine);
        component.setData(projectFile.toUtf8(), QUrl());
        if (!component.isReady())
            qDebug() << component.errorString();
        QVERIFY(component.isReady());

        QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
        QVERIFY(project);

        project->setSourceDirectory(testDataDir);

        QStringList expectedFiles(QStringList() << testDataDir + "/image.gif");
        qDebug() << project->files().toSet() << expectedFiles.toSet();
        QCOMPARE(project->files().toSet(), expectedFiles.toSet());
    }

    //
    // use Files element (1.1)
    //
    projectFile = QLatin1String(
            "import QmlProject 1.1\n"
            "Project {\n"
            "  Files {\n"
            "    filter: \"image.gif\"\n"
            "  }\n"
            "}\n");

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent component(&engine);
        component.setData(projectFile.toUtf8(), QUrl());
        if (!component.isReady())
            qDebug() << component.errorString();
        QVERIFY(component.isReady());

        QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
        QVERIFY(project);

        project->setSourceDirectory(testDataDir);

        QStringList expectedFiles(QStringList() << testDataDir + "/image.gif");
        qDebug() << project->files().toSet() << expectedFiles.toSet();
        QCOMPARE(project->files().toSet(), expectedFiles.toSet());
    }
}

void tst_FileFormat::testMatchesFile()
{
    //
    // search for qml files in local directory
    //
    QString projectFile = QLatin1String(
            "import QmlProject 1.0\n"
            "Project {\n"
            "  QmlFiles {"
            "    recursive: true"
            "  }"
            "  JavaScriptFiles {"
            "    paths: [\"script.js\"]"
            "  }"
            "}\n");

    QDeclarativeEngine engine;
    QDeclarativeComponent component(&engine);
    component.setData(projectFile.toUtf8(), QUrl());
    if (!component.isReady())
        qDebug() << component.errorString();
    QVERIFY(component.isReady());

    QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
    QVERIFY(project);

    project->setSourceDirectory(testDataDir);

    QVERIFY(project->matchesFile(testDataDir + "/file1.qml"));
    QVERIFY(project->matchesFile(testDataDir + "/notyetexistingfile.qml"));
    QVERIFY(project->matchesFile(testDataDir + "/subdir/notyetexistingfile.qml"));
    QVERIFY(project->matchesFile(testDataDir + "/script.js"));
    QVERIFY(!project->matchesFile(testDataDir + "/script.css"));
}

void tst_FileFormat::testLibraryPaths()
{
    //
    // search for qml files in local directory
    //
    QString projectFile = QLatin1String(
            "import QmlProject 1.0\n"
            "Project {\n"
            "  importPaths: [ \"../otherLibrary\", \"library\" ]\n"
            "}\n");

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent component(&engine);
        component.setData(projectFile.toUtf8(), QUrl());
        if (!component.isReady())
            qDebug() << component.errorString();
        QVERIFY(component.isReady());

        QmlProjectItem *project = qobject_cast<QmlProjectItem*>(component.create());
        QVERIFY(project);

        project->setSourceDirectory(testDataDir);

        QStringList expectedPaths(QStringList() << SRCDIR "/otherLibrary"
                                                << SRCDIR "/data/library");
        qDebug() << expectedPaths << project->importPaths();
        QCOMPARE(project->importPaths().toSet(), expectedPaths.toSet());
    }
}


QTEST_MAIN(tst_FileFormat);
#include "tst_fileformat.moc"
