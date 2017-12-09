#ifndef QTEEPIPE_H
#define QTEEPIPE_H

#include "qpipedevice.h"

class QTeePipe : public QPipeDevice
{
	Q_OBJECT

	Q_PROPERTY(QIODevice* teeDevice READ teeDevice WRITE setTeeDevice)

public:
	explicit QTeePipe(QIODevice* teeDevice, QObject *parent = nullptr);
	explicit QTeePipe(QObject *parent = nullptr);

	QIODevice* teeDevice() const;

public slots:
	void setTeeDevice(QIODevice* teeDevice);

protected:
	void init(bool buffered) override;
	QByteArray process(QByteArray &&data) override;
	void end() override;

private:
	QIODevice* _teeDevice;
};

#endif // QTEEPIPE_H
