#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>

class databasehandler : public QObject {
  Q_OBJECT

public:
  explicit databasehandler(QObject *parent = nullptr);
  bool addAudioToDatabase(const QString &filePath, const QString &artist, const QString &album, const QString &trackName); // Updated to include trackName

private:
  QSqlDatabase db;
  bool initializeDatabase();
};

#endif // DATABASEHANDLER_H