#ifndef SIGNALBUS_H
#define SIGNALBUS_H

#include <QObject>

class SignalBus : public QObject
{
    Q_OBJECT
public:
    explicit SignalBus(QObject *parent = nullptr);

    static SignalBus *signalBus();

signals:

    void switchToSampleCard(QString, int);
};

#endif  // SIGNALBUS_H
