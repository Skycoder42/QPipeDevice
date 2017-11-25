#ifndef QTEEPIPE_H
#define QTEEPIPE_H

#include "qpipedevice.h"

class QTeePipe : public QPipeDevice
{
	Q_OBJECT

	Q_PROPERTY(QIODevice* teeDevice READ teeDevice WRITE setTeeDevice)

public:
	explicit QTeePipe(QObject *parent = nullptr);

	QIODevice* teeDevice() const;

public slots:
	void setTeeDevice(QIODevice* teeDevice);

signals:
	void finished();//TODO move to base

protected:
	QByteArray process(QByteArray data) override;
	void end() override;

private:
	QIODevice* _teeDevice;
};

#endif // QTEEPIPE_H
