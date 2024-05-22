#include "databasemanager.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

DatabaseManager::DatabaseManager() {}

QSqlDatabase DatabaseManager::GetDatabase() {
    return m_db;
}

void DatabaseManager::OpenConnection() {
    // Read JSON data from file
    QByteArray val;
    QFile fileWindows(QDir::toNativeSeparators("C:/Users/boyan/Desktop/LSPro/auth.json"));
    QFile fileMac(QDir::toNativeSeparators("/Users/boyankiovtorov/Desktop/LSPro/auth.json"));

    if (!fileWindows.exists()) {
        qDebug() << "JSON file does not exist at path:" << fileWindows.fileName();
    }
    if (!fileWindows.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open JSON file at path:" << fileWindows.fileName();
        return;
    } else {
        val = fileWindows.readAll();
        fileWindows.close();
    }

    // Parse JSON data
    QJsonDocument document = QJsonDocument::fromJson(val);
    QJsonObject object = document.object();
    const auto settingsObject = object["web"].toObject();

    // Extract database connection parameters from JSON
    const QString host = settingsObject["host"].toString();
    const int port = settingsObject["port"].toInt();
    const QString dbName = settingsObject["database_name"].toString();
    const QString user = settingsObject["user"].toString();
    const QString password = settingsObject["password"].toString();

    // Configure the database connection parameters
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);

    // Open the database connection and handle success or failure
    if (m_db.open() && m_db.isOpen()) {
        qDebug() << "Database opened successfully";
    } else {
        // Handle connection error
        qDebug() << "Database failed to open";
        qDebug() << m_db.lastError().text();
    }
}


void DatabaseManager::CloseConnection() {
    // Close the database connection
    m_db.close();
}
