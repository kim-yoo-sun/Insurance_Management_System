#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListWidget>

class AdminMainWindow : public QWidget {
    Q_OBJECT

public:
    explicit AdminMainWindow(const QString& name, const QString& id, QWidget *parent = nullptr);
    QListWidget *applyList;
    QList<QList<QString>> allApplications;
};


class UserMainWindow : public QWidget {
    Q_OBJECT

public:
    explicit UserMainWindow(const QString& name, const QString& id, QWidget *parent = nullptr);
    void loadUserApplications(const QString &userId);

private:
    QListWidget *applyList;
    QString m_userId;

private slots:
    void onEditButtonClicked();
};

#endif // MAINWINDOW_H
