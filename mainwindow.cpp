#include "mainwindow.h"
#include "ui/ui_mainwindow.h"
#include "addPhotos.h"
#include <QDir>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <iostream>
#include <QPushButton>

QString MainWindow::mediaFolder = QDir::homePath() + "/mmpp";
MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow),fileWatcher(new QFileSystemWatcher(this)) {
    ui->setupUi(this);
    setupDatabase();
    //Populate tables
    loadTableData("movies",ui->moviesTable);
    loadTableData("music",ui->musicTable);
    loadTableData("shows",ui->showsTable);
    loadTableData("photos",ui->photoTable);
    fileWatcher->addPath(mediaFolder);
    connect(fileWatcher, &QFileSystemWatcher::directoryChanged, this, &MainWindow::onFolderChanged);
    connect(ui->addPhotosButton, &QPushButton::clicked, [this]() { addNew("Photos"); });
}

MainWindow::~MainWindow() {
    db.close();
    delete ui;
    delete fileWatcher;
}
void MainWindow::setupDatabase() {
    createMediaFolder();
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = mediaFolder + "/media.db";

    std::cout << "Attempting to create or open database at " << dbPath.toStdString() << std::endl;
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        std::cerr << "Database Error: " << db.lastError().text().toStdString() << std::endl;
        return;
    } else {
        std::cout << "Database opened successfully at " << dbPath.toStdString() << std::endl;
    }



    // Execute SQL files for tables
    executeSql("./sql/shows.sql");
    executeSql("./sql/movies.sql");
    executeSql("./sql/music.sql");
    executeSql("./sql/photos.sql");

    // Create media subfolders
    addFolder("shows");
    addFolder("movies");
    addFolder("music");
    addFolder("photos");


}

void MainWindow::loadTableData(const QString &tableName, QTableView *tableView) {
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable(tableName);
    model->select();  // Fetch all data from the table

    tableView->setModel(model);

    // Resize all columns to fit content
    for (int col = 0; col < model->columnCount(); ++col) {
        tableView->resizeColumnToContents(col);
    }
}
void MainWindow::addFolder(const QString &folderName) {
    QDir newDir(mediaFolder + "/" + folderName);
    if (!newDir.exists() && !newDir.mkpath(".")) {
        std::cout << "ERROR\n";
    }
}
void MainWindow::executeSql(const QString &filePath){
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        std::cout << "File Error could not open .sql file\n";
        return;
    }
    QSqlQuery query;
    QString sql = file.readAll();
    if (!query.exec(sql)) {
        std::cerr << "SQL ERROR!!!!\n";
        return;
    }
}

void MainWindow::createMediaFolder() {
    QDir mediaDir(mediaFolder);
    if (!mediaDir.exists()){
        if (!mediaDir.mkpath(".")) {
            std::cout << "Error: Failed to create media folder @ " << mediaFolder.toStdString() << std::endl;
        } else {
            std::cout << "Media Folder Created Successfully " << mediaFolder.toStdString() << std::endl;
        }
    } else {

    }
}

void MainWindow::onFolderChanged(const QString &path) {
    std::cout << "Feature not yet implemented\n";
}

void MainWindow::connectToEmby() {
    std::cout << "Feature not yet implemented\n";
}

void MainWindow::addNew(const QString &category) {
    if (category == "Photos") {
        QString photosFolder = QDir::homePath() + "/mmpp/photos";
        addPhotos dialog(photosFolder,this);
        dialog.exec();
    } else {
        std::cerr <<"????\n";
    }
}



