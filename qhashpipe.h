#ifndef QHASHPIPE_H
#define QHASHPIPE_H

#include "qpipedevice.h"
#include <QCryptographicHash>

class QHashPipe : public QPipeDevice
{
	Q_OBJECT

public:
	explicit QHashPipe(QCryptographicHash::Algorithm algorithm, QObject *parent = nullptr);

	QByteArray hash() const;
	void resetHash();

protected:
	QByteArray process(QByteArray &&data) override;

private:
	QCryptographicHash _hash;
};

#endif // QHASHPIPE_H
